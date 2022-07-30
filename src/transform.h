#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"

struct Transform {
	glm::vec3 pos{};
	float scale = 1.0f;
	glm::vec3 rot{};

	void getModelMatrix(glm::mat4& modelMatrix);
};
