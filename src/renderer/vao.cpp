#include "vao.h"

vao_t::vao_t() {
	glGenVertexArrays(1, &id);
}

void vao_t::attach_vbo(const vbo_t& vbo, GLuint index, int num_floats_per_descriptor, int stride, int offset_in_bytes) {
	glEnableVertexAttribArray(index);
	vbo.bind();
	glVertexAttribPointer(index, num_floats_per_descriptor, GL_FLOAT, GL_FALSE, stride, (void*)offset_in_bytes);
	vbo.unbind();
}

void vao_t::bind() {
	glBindVertexArray(id);
}

void vao_t::unbind() {
	glBindVertexArray(0);
}