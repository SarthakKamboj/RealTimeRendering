#pragma once

#include <iostream>
#include <tiny_obj_loader.h>
#include <unordered_map>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include "renderer/vao.h"
#include "renderer/vbo.h"
#include "renderer/ebo.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;

	bool operator==(const Vertex& other) const {
		return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
	}
};

struct Model {
	VAO vao;
	VBO vbo;
	EBO ebo;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indicies;

	bool indexEnabled = true;

	void render();
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
};

// void loadModel(const std::string& modelPath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
void loadModel(const std::string& modelPath, Model& model);
