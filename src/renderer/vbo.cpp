#include "vbo.h"

vbo_t::vbo_t() {
	glGenBuffers(1, &id);
}

void vbo_t::set_data(float* vertices, int byte_size_of_vertices, GLenum usage) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, byte_size_of_vertices, vertices, usage);
	unbind();
}

void vbo_t::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void vbo_t::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}