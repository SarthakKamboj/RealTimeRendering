#pragma once

#include "glad/glad.h"
#include "vbo.h"

struct vao_t {
	vao_t();
	void attach_vbo(const vbo_t& vbo, GLuint index, int num_floats_per_descriptor, int stride, int offset_in_bytes);
	void bind();
	void unbind();
	GLuint id;
};
