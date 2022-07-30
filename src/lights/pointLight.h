#pragma once

#include <glm/glm.hpp>
#include <string>
#include "renderer/shaderProgram.h"
#include "renderer/meshRenderer.h"
#include "loadModel.h"

class PointLight {
public:
	PointLight(const glm::vec3& color, const glm::vec3& position, float maxDist, float multiplier);
	void setPointLightInShader(ShaderProgram& shaderProgram, int idx);
	void debugRender();

	ShaderProgram shaderProgram;
	glm::vec3 position;
	glm::vec3 color;
	float maxDist;
	float multiplier;

private:
	Model model;
};
