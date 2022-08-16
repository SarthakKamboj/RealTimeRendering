#pragma once

#include "glad/glad.h"

struct vbo_t {
	vbo_t();
	void set_data(float* vertices, int byteSizeOfVertices, GLenum usage);
	void bind() const;
	void unbind() const;
	GLuint id;
};
