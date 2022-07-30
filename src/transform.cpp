#include "transform.h"

void Transform::getModelMatrix(glm::mat4& model) {
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(scale, scale, scale));
}