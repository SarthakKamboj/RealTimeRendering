#include "spotLight.h"
#include "renderer/lightFrameBuffer.h"
#include "renderer/shaderProgram.h"

spot_light_t::spot_light_t(const glm::vec3& _color, const glm::vec3& _position, glm::vec3& _dir, float _multiplier, float _umbra, float _prenumbra) {
	multiplier = _multiplier;
	color = _color;
	pos = _position;
	dir = _dir;
	umbra = _umbra;
	prenumbra = _prenumbra;

	const std::string vertPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightDebug.vert";
	const std::string fragPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightDebug.frag";

	create_shader_program(shader_program, vertPath, fragPath);

	std::string cubePath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\cube.obj";
}

void spot_light_t::set_spot_light_in_shader_prog(shader_program_t& shaderProgram, int idx) {

	spot_light_view = light_frame_buffer_t::get_light_view_mat(pos, dir);
	spot_light_proj = light_frame_buffer_t::get_spot_light_mat(umbra);

	std::string lightPosVar = "spotLights[" + std::to_string(idx) + "].pos";
	std::string lightColorVar = "spotLights[" + std::to_string(idx) + "].pointColor";
	std::string lightDirVar = "spotLights[" + std::to_string(idx) + "].dir";
	std::string lightMultiplierVar = "spotLights[" + std::to_string(idx) + "].multiplier";
	std::string lightUmbraVar = "spotLights[" + std::to_string(idx) + "].umbra";
	std::string lightPrenumbraVar = "spotLights[" + std::to_string(idx) + "].prenumbra";
	std::string lightSUVar = "spotLights[" + std::to_string(idx) + "].samplerUnit";
	std::string lightProjVar = "spotLights[" + std::to_string(idx) + "].proj";
	std::string lightViewVar = "spotLights[" + std::to_string(idx) + "].view";

	shader_program.set_vec3(lightPosVar.c_str(), pos);
	shader_program.set_vec3(lightColorVar.c_str(), color);
	shader_program.set_vec3(lightDirVar.c_str(), dir);
	shader_program.set_float(lightMultiplierVar.c_str(), multiplier);
	shader_program.set_float(lightUmbraVar.c_str(), umbra);
	shader_program.set_float(lightPrenumbraVar.c_str(), prenumbra);
	shader_program.set_int(lightSUVar.c_str(), 1);
	shader_program.set_mat_4(lightProjVar.c_str(), spot_light_proj);
	shader_program.set_mat_4(lightViewVar.c_str(), spot_light_view);
}

void spot_light_t::debug_render() {
	glm::mat4 modelMat(1.0f);
	modelMat = glm::translate(modelMat, pos);
	modelMat = glm::scale(modelMat, glm::vec3(0.2f, 0.2f, 0.2f));
	// shader_program.set_mat_4("model", modelMat);
	shader_program.set_vec3("color", color);

	shader_program.bind();
	// model.render();

	/*
	const std::vector<transform_t>& transforms = shader_program.models_manager.transforms;
	for (int i = 0; i < transforms.size(); i++) {
		glm::mat4 model_mat(1.0f);
		shader_program.models_manager.transforms[i].get_model_matrix(model_mat);
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