#include "pointLight.h"

point_light_t::point_light_t(const glm::vec3& _color, const glm::vec3& _position, float _maxDist, float _multiplier) {
	max_dist = _maxDist;
	multiplier = _multiplier;
	color = _color;
	position = _position;

	const std::string vertPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightDebug.vert";
	const std::string fragPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightDebug.frag";
	// create_shader_program(shader_program, vertPath, fragPath);
	const std::vector<std::string> parameters;
	shader_program = shader_program_t(vertPath, fragPath, parameters);
	// shaderProgram = ShaderProgram(vertPath.c_str(), fragPath.c_str());

	std::string cubePath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\cube.obj";
}

void point_light_t::set_point_light_in_shader(shader_program_t& shader_program, int idx) {
	std::string pointLightPosVar = "pointLights[" + std::to_string(idx) + "].pos";
	std::string pointLightColorVar = "pointLights[" + std::to_string(idx) + "].pointColor";
	std::string pointLightMaxDistVar = "pointLights[" + std::to_string(idx) + "].maxDist";
	std::string pointLightMultiplierVar = "pointLights[" + std::to_string(idx) + "].multiplier";
	shader_program.set_vec3(pointLightPosVar.c_str(), position);
	shader_program.set_vec3(pointLightColorVar.c_str(), color);
	shader_program.set_float(pointLightMaxDistVar.c_str(), max_dist);
	shader_program.set_float(pointLightMultiplierVar.c_str(), multiplier);
}

void point_light_t::debug_render() {
	glm::mat4 modelMat(1.0f);
	modelMat = glm::translate(modelMat, position);
	modelMat = glm::scale(modelMat, glm::vec3(0.2f, 0.2f, 0.2f));
	shader_program.set_mat_4("model", modelMat);
	shader_program.set_vec3("color", color);

	shader_program.bind();
	/*
	model.vao.bind();
	glDrawElements(GL_TRIANGLES, model.indicies.size(), GL_UNSIGNED_INT, 0);
	model.vao.unbind();
	*/
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
