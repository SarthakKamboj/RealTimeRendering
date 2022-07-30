#pragma once

#include <glm/glm.hpp>
#include <string>
#include "renderer/shaderProgram.h"
#include "renderer/meshRenderer.h"
#include "loadModel.h"

class DirectionalLight {
public:
	DirectionalLight(const glm::vec3& color, const glm::vec3& dir, float multiplier);
	void setDirectionalLightInShader(ShaderProgram& shaderProgram, int idx);
	void debugRender();

	ShaderProgram shaderProgram;
	glm::vec3 dir;
	glm::vec3 color;
	glm::vec3 debugPos{};
	float multiplier;

private:
	Model model;
};