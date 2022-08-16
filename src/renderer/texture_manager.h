#include <vector>
#include <string>
#include "stb_image.h"
#include "glad/glad.h"

struct texture_bind_data_t {
	unsigned int texture_id;
	unsigned int tex_unit;
};

struct texture_manager_t {
	std::vector<texture_bind_data_t> textures_bind_data;
	std::vector<std::string> tex_paths;	
};

void add_texture(texture_manager_t& texture_manager, const std::string& file_path, unsigned int tex_unit);