#include "directionalLight.h"

dir_light_t::dir_light_t(const glm::vec3& _color, const glm::vec3& _dir, float _multiplier) {
	multiplier = _multiplier;
	color = _color;
	dir = _dir;

	const std::string vertPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightDebug.vert";
	const std::string fragPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightDebug.frag";
	create_shader_program(shader_program, vertPath, fragPath);
	// shader_program = ShaderProgram(vertPath.c_str(), fragPath.c_str());

	std::string cubePath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\arrow.obj";
}

void dir_light_t::set_dir_light_in_shader(shader_program_t& shader_program, int idx) {

	dir_light_view = light_frame_buffer_t::get_light_view_mat(glm::vec3(0, 0, 0) - (dir_y_pos * dir), dir);
	dir_light_proj = light_frame_buffer_t::get_dir_light_proj(x_extent);

	std::string lightDirVar = "directionalLights[" + std::to_string(idx) + "].dir";
	std::string lightColorVar = "directionalLights[" + std::to_string(idx) + "].pointColor";
	std::string lightMultVar = "directionalLights[" + std::to_string(idx) + "].multiplier";
	std::string lightSUVar = "directionalLights[" + std::to_string(idx) + "].samplerUnit";
	std::string lightProjVar = "directionalLights[" + std::to_string(idx) + "].proj";
	std::string lightViewVar = "directionalLights[" + std::to_string(idx) + "].view";

	shader_program.set_vec3(lightDirVar.c_str(), dir);
	shader_program.set_vec3(lightColorVar.c_str(), color);
	shader_program.set_float(lightMultVar.c_str(), multiplier);
	shader_program.set_int(lightSUVar.c_str(), 2);
	shader_program.set_mat_4(lightProjVar.c_str(), dir_light_proj);
	shader_program.set_mat_4(lightViewVar.c_str(), dir_light_view);
}

void dir_light_t::debug_render() {
	glm::mat4 modelMat(1.0f);
	modelMat = glm::translate(modelMat, debug_pos);
	modelMat = glm::scale(modelMat, glm::vec3(0.2f, 0.2f, 0.2f));
	shader_program.set_mat_4("model", modelMat);
	shader_program.set_vec3("color", color);

	shader_program.bind();
	// model.render();

	/*
	const std::vector<transform_t>& transforms = shader_program.models.transforms;
	for (int i = 0; i < transforms.size(); i++) {
		glm::mat4 model_mat(1.0f);
		shader_program.models.transforms[i].get_model_matrix(model_mat);
		shader_program.set_mat_4("model", model_mat);
		shader_program.bind();

		const model_vertex_data_t& model = shader_program.models.vertices_data[i];
		const std::vector<texture_bind_data_t>& textures_data = shader_program.models.model_textures_data[i];

		for (const texture_bind_data_t& texture_data : textures_data) {
			glActiveTexture(GL_TEXTURE0 + texture_data.tex_unit);
			glBindTexture(GL_TEXTURE_2D, texture_data.texture_id);
		}

		glBindVertexArray(model.vao);
		glDrawElements(GL_TRIANGLES, model.num_indices_to_render, GL_UNSIGNED_INT, 0);
		shader_program.unbind();
	}
	*/

	shader_program.unbind();
}