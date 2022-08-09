#pragma once

#include "renderer/vbo.h"
#include "renderer/vao.h"
#include "renderer/ebo.h"
#include "renderer/texture.h"
#include "renderer/shaderProgram.h"
#include "loadModel.h"
#include <vector>

struct MeshRenderer {
	Texture texture;
	Model model;

	MeshRenderer(const std::string& modelPath, const std::string& texPath);

	void render();
};
