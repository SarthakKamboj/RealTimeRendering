#include "texture.h"
#include "stb_image.h"
#include "glad/glad.h"

/*
Texture::Texture() {
	texUnit = -1;
	sprintf_s(filePath, "");
}

Texture::Texture(const char* _filePath, int _texUnit) {
	texUnit = _texUnit;
	updateTextureFilePath(_filePath);
}

void Texture::updateTextureFilePath(const char* _filePath) {

	if (createdTexture) {
		glDeleteTextures(1, &texture);
	}
	createdTexture = true;

	stbi_set_flip_vertically_on_load(true);

	// load image data
	int imgWidth, imgHeight, nrChannels;
	unsigned char* data = stbi_load(_filePath, &imgWidth, &imgHeight, &nrChannels, 0);

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// sets the row alignment to 1, so rows are aligned every 1 byte, which is basically no space between rows
	// glPixelStorei(GL_UNPACK_ALIGNMENT, 8) would set alignment to 8, so rows always start at memory location that is multiple of 8
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (nrChannels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else if (nrChannels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	// create mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	sprintf_s(filePath, _filePath);
}

void Texture::bind() {
	if (texture == -1) return;
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unbind() {
	if (texture == -1) return;
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

*/