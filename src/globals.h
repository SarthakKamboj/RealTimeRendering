struct texture_manager_t;
struct shader_program_t;
struct models_data_t;

struct globals_t {
	texture_manager_t* texture_manager;
	// std::vector<shader_program_t*> shader_programs;
	shader_program_t* default_shader_program;
	models_data_t* models_data;
};