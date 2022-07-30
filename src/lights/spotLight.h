#pragma once

#include <glm/glm.hpp>
#include <string>
#include "renderer/shaderProgram.h"
#include "renderer/meshRenderer.h"
#include "loadModel.h"

class SpotLight {
public:
	SpotLight(const glm::vec3& color, const glm::vec3& position, glm::vec3& dir, float multiplier, float umbra, float prenumbra);
	void setSpotLightInShader(ShaderProgram& shaderProgram, int idx);
	void debugRender();

	ShaderProgram shaderProgram;
	glm::vec3 dir;
	glm::vec3 pos;
	glm::vec3 color;
	float umbra;
	float prenumbra;
	float multiplier;

private:
	Model model;
};