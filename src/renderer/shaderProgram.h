#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include "model.h"

struct shader_program_t {
	models_manager_t models_manager;
	GLuint program_id;
	void set_mat_4(const char* var_name, const glm::mat4& mat);
	void set_int(const char* var_name, int val);
	void set_float(const char* var_name, float val);
	void set_vec3(const char* var_name, const glm::vec3& vec3);
	void bind();
	void unbind();
	void render_models();
};

void create_shader_program(shader_program_t& shader_program, const std::string& vertex_path, const std::string& fragment_path);
GLuint create_shader(const std::string& path, GLenum shader_type);
