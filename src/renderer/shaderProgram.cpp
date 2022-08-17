#include "shaderProgram.h"
#include "globals.h"

#define MAX_ERR_LEN 1024

extern globals_t globals;

shader_program_t::shader_program_t(const std::string& vertex_path, const std::string& fragment_path, const std::vector<std::string>& _parameter_names) {

	GLuint vertex_id = create_shader(vertex_path, GL_VERTEX_SHADER);
	if (vertex_id == -1) {
		program_id = -1;
		return;
	}
	GLuint frag_id = create_shader(fragment_path, GL_FRAGMENT_SHADER);
	if (frag_id == -1) {
		program_id = -1;
		return;
	}
	program_id = glCreateProgram();

	glAttachShader(program_id, vertex_id);
	glAttachShader(program_id, frag_id);
	glLinkProgram(program_id);

	glDeleteShader(vertex_id);
	glDeleteShader(frag_id);

	parameter_names.resize(_parameter_names.size());
	for (const std::string& name : _parameter_names) {
		parameter_names.push_back(name);
	}
}

void shader_program_t::add_shader_parameters_map() {
	shader_parameters.push_back(std::map<std::string, int>());
	auto& map = shader_parameters[shader_parameters.size() - 1];
	for (const std::string& name : parameter_names) {
		map[name] = 0;
	}
}

GLuint shader_program_t::create_shader(const std::string& path, GLenum shader_type) {
	// get shader file contents
	std::string line, file_contents;
	std::ifstream file(path.c_str());

	while (std::getline(file, line)) {
		file_contents += line + "\n";
	}

	// compile shader and make sure there aren't any errors other return shader id
	const char* file_code_char = file_contents.c_str();
	GLuint shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &file_code_char, NULL);
	glCompileShader(shader_id);

	GLint success = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		std::cout << (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compilation failed" << std::endl;
		int maxErrorLength = MAX_ERR_LEN;
		char errorInfo[MAX_ERR_LEN];
		glGetShaderInfoLog(shader_id, maxErrorLength, &maxErrorLength, errorInfo);
		std::cout << "ERROR: " << errorInfo << std::endl;
		return -1;
	}

	return shader_id;
}

void shader_program_t::set_mat_4(const char* var_name, const glm::mat4& mat) {
	if (program_id == -1) return;
	bind();
	GLint loc = glGetUniformLocation(program_id, var_name);
	if (loc == -1) {
		// std::cout << var_name << " doesn't exist" << std::endl;
	}
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
	unbind();
}

void shader_program_t::set_float(const char* var_name, float val) {
	if (program_id == -1) return;
	bind();
	GLint loc = glGetUniformLocation(program_id, var_name);
	if (loc == -1) {
		// std::cout << var_name << " doesn't exist" << std::endl;
	}
	glUniform1f(loc, val);
	unbind();
}

void shader_program_t::set_vec3(const char* var_name, const glm::vec3& vec3) {
	if (program_id == -1) return;
	bind();
	GLint loc = glGetUniformLocation(program_id, var_name);
	if (loc == -1) {
		// std::cout << var_name << " doesn't exist" << std::endl;
	}
	glUniform3fv(loc, 1, glm::value_ptr(vec3));
	unbind();
}

void shader_program_t::set_int(const char* var_name, int val) {
	if (program_id == -1) return;
	bind();
	GLint loc = glGetUniformLocation(program_id, var_name);
	if (loc == -1) {
		// std::cout << var_name << " doesn't exist" << std::endl;
	}
	glUniform1i(loc, val);
	unbind();
}

void shader_program_t::bind() {
	glUseProgram(program_id);
}

void shader_program_t::unbind() {
	glUseProgram(0);
}

void shader_program_t::render_models() {

	const std::vector<transform_t>& transforms = models_manager.transforms;
	const models_data_t& models_data = *globals.models_data;

	for (int i = 0; i < transforms.size(); i++) {
		glm::mat4 model_mat(1.0f);
		transforms[i].get_model_matrix(model_mat);
		set_mat_4("model", model_mat);

		for (const std::pair<std::string, int>& elem : shader_parameters[i]) {
			if (elem.first == "selected" && elem.second == 1) {
				std::cout << "selected registered" << std::endl;
			}
			set_int(elem.first.c_str(), elem.second);
		}

		bind();

		int model_vert_idx = models_manager.model_vertex_idxs[i];
		const model_vertex_data_t& model = models_data.vertices_data[model_vert_idx];
		const std::vector<texture_bind_data_t>& textures_data = models_manager.model_textures_data[i];

		for (const texture_bind_data_t& texture_data : textures_data) {
			glActiveTexture(GL_TEXTURE0 + texture_data.tex_unit);
			glBindTexture(GL_TEXTURE_2D, texture_data.texture_id);
		}

		glBindVertexArray(model.vao);
		glDrawElements(GL_TRIANGLES, model.num_indices_to_render, GL_UNSIGNED_INT, 0);
		unbind();
	}
}

