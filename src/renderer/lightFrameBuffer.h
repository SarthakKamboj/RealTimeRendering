#pragma once

#include "light.h"

struct light_frame_buffer_t {

	enum LightType {
		SPOTLIGHT, DIRECTIONAL
	};

	light_frame_buffer_t();
	void bind();
	void unbind();

	unsigned int id;
	unsigned int color_tex;
	unsigned int depth_tex;

	static glm::mat4 get_light_view_mat(const glm::vec3& lightPos, const glm::vec3& lightDir);
	static glm::mat4 get_dir_light_proj(float xExtent);
	static glm::mat4 get_spot_light_mat(float angle);
};
