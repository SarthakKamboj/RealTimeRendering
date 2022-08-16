#pragma once

#include "glad/glad.h"

struct ebo_t {
	ebo_t();
	void set_data(unsigned int* indicies, int size_of_indices, GLenum usage);
	void bind();
	void unbind();
	GLuint id;
};
