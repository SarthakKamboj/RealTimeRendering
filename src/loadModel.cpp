#include "loadModel.h"

/*
void Model::render() {
	vao.bind();
	if (indexEnabled) {
		glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
	vao.unbind();
}

void loadModel(const std::string& modelPath, Model& model) {

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
		std::string fullError = warn + err;
		throw std::exception(fullError.c_str());
	}

	std::vector<unsigned int>& indicies = model.indicies;
	std::vector<Vertex>& vertices = model.vertices;
	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index],
				attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<unsigned int>(model.vertices.size());
				vertices.push_back(vertex);
			}

			indicies.push_back(uniqueVertices[vertex]);
		}
	}

	EBO& ebo = model.ebo;
	VBO& vbo = model.vbo;
	VAO& vao = model.vao;

	ebo.setData((unsigned int*)&indicies[0], indicies.size() * sizeof(indicies[0]), GL_STATIC_DRAW);
	vbo.setData((float*)&vertices[0], vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

	vao.bind();
	ebo.bind();

	vao.attachVBO(vbo, 0, 3, sizeof(Vertex), offsetof(Vertex, pos));
	vao.attachVBO(vbo, 1, 3, sizeof(Vertex), offsetof(Vertex, normal));
	vao.attachVBO(vbo, 2, 2, sizeof(Vertex), offsetof(Vertex, texCoord));

	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}
*/