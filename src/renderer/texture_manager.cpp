#include "texture_manager.h"

void add_texture(texture_manager_t& texture_manager, const std::string& file_path, unsigned int tex_unit) {
	
	texture_bind_data_t texture_data{};
	texture_data.tex_unit = tex_unit;
	
	const std::vector<std::string>::iterator path_it = std::find(texture_manager.tex_paths.begin(), texture_manager.tex_paths.begin(), file_path);

	if (path_it != texture_manager.tex_paths.cend()) {
		int idx = std::distance(texture_manager.tex_paths.begin(), path_it);
		texture_data.texture_id = texture_manager.textures_bind_data[idx].texture_id;
		texture_manager.textures_bind_data.push_back(texture_data);
		return;
	}
	
	stbi_set_flip_vertically_on_load(true);

	// load image data
	int img_width, img_height, num_channels;
	unsigned char* data = stbi_load(file_path.c_str(), &img_width, &img_height, &num_channels, 0);	

	glGenTextures(1, &texture_data.texture_id);
	glActiveTexture(GL_TEXTURE0 + tex_unit);
	glBindTexture(GL_TEXTURE_2D, texture_data.texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// sets the row alignment to 1, so rows are aligned every 1 byte, which is basically no space between rows
	// glPixelStorei(GL_UNPACK_ALIGNMENT, 8) would set alignment to 8, so rows always start at memory location that is multiple of 8
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (num_channels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else if (num_channels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	// create mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	texture_manager.tex_paths.push_back(std::string(file_path));
	texture_manager.textures_bind_data.push_back(texture_data);
}