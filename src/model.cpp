#include "model.h"
#include <glm/gtx/hash.hpp>
#include "globals.h"
#include "renderer/shaderProgram.h"

extern globals_t globals;

struct vertex_t {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tex_coord;

	bool operator==(const vertex_t& other) const {
		return pos == other.pos && normal == other.normal && tex_coord == other.tex_coord;
	}
};

namespace std {
	template<> struct hash<vertex_t> {
		size_t operator()(vertex_t const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.tex_coord) << 1);
		}
	};
};

int load_model(shader_program_t& shader_program, const std::string& file_path) {

	models_data_t& models_data = *globals.models_data;
	models_manager_t& model_manager = shader_program.models_manager;
	std::vector<std::string>& file_paths = globals.models_data->file_paths;
	const std::vector<std::string>::iterator path_it = std::find(file_paths.begin(), file_paths.end(), file_path);
	if (path_it != file_paths.cend()) {
		int model_idx = std::distance(file_paths.begin(), path_it);

		transform_t transform{};
		transform.scale = 1.0f;
		model_manager.transforms.push_back(transform);
		model_manager.model_textures_data.push_back(std::vector<texture_bind_data_t>());
		model_manager.model_vertex_idxs.push_back(model_idx);

		shader_program.add_shader_parameters_map();
		return model_manager.model_textures_data.size() - 1;
	}

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_path.c_str())) {
		std::string full_error = warn + err;
		throw std::exception(full_error.c_str());
	}

	std::vector<unsigned int> indicies;
	std::vector<vertex_t> vertices;
	// look into other possible data structures
	std::unordered_map<vertex_t, uint32_t> unique_verts{};
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			vertex_t vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.tex_coord = {
				attrib.texcoords[2 * index.texcoord_index],
				attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			if (unique_verts.count(vertex) == 0) {
				unique_verts[vertex] = static_cast<unsigned int>(vertices.size());
				vertices.push_back(vertex);
			}

			indicies.push_back(unique_verts[vertex]);
		}
	}

	ebo_t ebo{};
	vao_t vao{};
	vbo_t vbo{};

	ebo.set_data((unsigned int*)&indicies[0], indicies.size() * sizeof(indicies[0]), GL_STATIC_DRAW);
	vbo.set_data((float*)&vertices[0], vertices.size() * sizeof(vertex_t), GL_STATIC_DRAW);

	vao.bind();
	ebo.bind();

	vao.attach_vbo(vbo, 0, 3, sizeof(vertex_t), offsetof(vertex_t, pos));
	vao.attach_vbo(vbo, 1, 3, sizeof(vertex_t), offsetof(vertex_t, normal));
	vao.attach_vbo(vbo, 2, 2, sizeof(vertex_t), offsetof(vertex_t, tex_coord));

	vao.unbind();
	vbo.unbind();
	ebo.unbind();

	model_vertex_data_t model_vertex_data{};
	model_vertex_data.ebo = ebo.id;
	model_vertex_data.vao = vao.id;
	model_vertex_data.num_indices_to_render = indicies.size();

	models_data.vertices_data.push_back(model_vertex_data);
	models_data.file_paths.push_back(file_path);

	int model_vert_idx = models_data.vertices_data.size() - 1;

	transform_t transform{};
	transform.scale = 1.0f;
	model_manager.transforms.push_back(transform);

	model_manager.model_textures_data.push_back(std::vector<texture_bind_data_t>());
	model_manager.model_vertex_idxs.push_back(model_vert_idx);

	shader_program.add_shader_parameters_map();

	return model_manager.model_textures_data.size() - 1;
}

void attach_texture_to_model(models_manager_t& model_manager, const std::string file_path, int model_idx, unsigned int tex_unit) {
	int texture_id = add_texture(*globals.texture_manager, file_path);
	texture_bind_data_t texture_bind_data{};
	texture_bind_data.texture_id = texture_id;
	texture_bind_data.tex_unit = tex_unit;
	model_manager.model_textures_data[model_idx].push_back(texture_bind_data);
}