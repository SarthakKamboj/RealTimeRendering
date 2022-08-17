#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "transform.h"
#include <string>
#include <tiny_obj_loader.h>
#include <unordered_map>
#include "renderer/vao.h"
#include "renderer/vbo.h"
#include "renderer/ebo.h"
#include "renderer/texture_manager.h"

struct model_vertex_data_t {
	unsigned int vao;
	unsigned int ebo;
	unsigned int num_indices_to_render;
};

struct models_manager_t {
	std::vector<int> model_vertex_idxs;
	std::vector<transform_t> transforms;
	std::vector<std::vector<texture_bind_data_t>> model_textures_data;
};

struct models_data_t {
	std::vector<model_vertex_data_t> vertices_data;
	std::vector<std::string> file_paths;
};

struct shader_program_t;
int load_model(shader_program_t& shader_program, const std::string& file_path);
void attach_texture_to_model(models_manager_t& model_manager, const std::string file_path, int model_idx, unsigned int tex_unit);