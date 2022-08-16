#pragma once

#include "glm/glm.hpp"

struct frame_buffer_t {
	frame_buffer_t();
	void bind();
	void unbind();

	static void clear_frame_buffer(const glm::vec3& color);

	unsigned int color_tex;
	unsigned int id;
};
