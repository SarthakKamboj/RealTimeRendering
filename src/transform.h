#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"

struct transform_t {
	glm::vec3 pos;
	float scale;
	glm::vec3 rot;

	void get_model_matrix(glm::mat4& modelMatrix) const;
};
