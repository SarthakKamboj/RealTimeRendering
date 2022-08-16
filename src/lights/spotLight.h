#pragma once

#include <glm/glm.hpp>
#include <string>
#include "renderer/shaderProgram.h"
#include "renderer/meshRenderer.h"
#include "loadModel.h"

struct spot_light_t {
	spot_light_t(const glm::vec3& color, const glm::vec3& position, glm::vec3& dir, float multiplier, float umbra, float prenumbra);
	void set_spot_light_in_shader_prog(shader_program_t& shader_program, int idx);
	void debug_render();

	shader_program_t shader_program;
	glm::vec3 dir;
	glm::vec3 pos;
	glm::vec3 color;
	float umbra;
	float prenumbra;
	float multiplier;

	glm::mat4 spot_light_view;
	glm::mat4 spot_light_proj;
};