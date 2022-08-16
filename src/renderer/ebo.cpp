#include "ebo.h"

ebo_t::ebo_t() {
	glGenBuffers(1, &id);
}

void ebo_t::set_data(unsigned int* indicies, int size_of_indices, GLenum usage) {
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_of_indices, indicies, usage);
	unbind();
}

void ebo_t::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void ebo_t::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}