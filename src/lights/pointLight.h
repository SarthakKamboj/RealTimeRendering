#pragma once

#include <glm/glm.hpp>
#include <string>
#include "renderer/shaderProgram.h"
#include "renderer/meshRenderer.h"
#include "loadModel.h"

struct point_light_t {
	point_light_t(const glm::vec3& color, const glm::vec3& position, float maxDist, float multiplier);
	void set_point_light_in_shader(shader_program_t& shaderProgram, int idx);
	void debug_render();

	shader_program_t shader_program;
	glm::vec3 position;
	glm::vec3 color;
	float max_dist;
	float multiplier;
};
