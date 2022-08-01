#pragma once

#include "renderer/vbo.h"
#include "renderer/vao.h"
#include "renderer/ebo.h"
#include "renderer/texture.h"
#include "renderer/shaderProgram.h"
#include "loadModel.h"
#include <vector>

class MeshRenderer {
public:
	Texture texture;
	ShaderProgram shaderProgram;
	Model model;

	MeshRenderer(const std::string& modelPath, const std::string& texPath, const ShaderProgram& _shaderProgram);

	void render();
};
