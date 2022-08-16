#pragma once

#include <glm/glm.hpp>
#include <string>
#include "renderer/shaderProgram.h"
#include "renderer/meshRenderer.h"
#include "renderer/lightFrameBuffer.h"
#include "loadModel.h"

struct dir_light_t {
	dir_light_t(const glm::vec3& color, const glm::vec3& dir, float multiplier);
	void set_dir_light_in_shader(shader_program_t& shaderProgram, int idx);
	void debug_render();

	shader_program_t shader_program;
	glm::vec3 dir;
	glm::vec3 color;
	glm::vec3 debug_pos{};
	float multiplier = 1.0f;
	float x_extent = 10.0f;
	float dir_y_pos = 10.0f;

	glm::mat4 dir_light_view;
	glm::mat4 dir_light_proj;

};