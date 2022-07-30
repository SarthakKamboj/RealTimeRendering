#include "directionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& _color, const glm::vec3& _dir, float _multiplier) {
	multiplier = _multiplier;
	color = _color;
	dir = _dir;

	const std::string vertPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\light.vert";
	const std::string fragPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\light.frag";
	shaderProgram = ShaderProgram(vertPath.c_str(), fragPath.c_str());

	std::string cubePath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\arrow.obj";
	loadModel(cubePath, model);
}

void DirectionalLight::setDirectionalLightInShader(ShaderProgram& shaderProgram, int idx) {
	std::string lightDirVar = "directionalLights[" + std::to_string(idx) + "].dir";
	std::string lightColorVar = "directionalLights[" + std::to_string(idx) + "].pointColor";
	std::string lightMultVar = "directionalLights[" + std::to_string(idx) + "].multiplier";
	shaderProgram.setVec3(lightDirVar.c_str(), dir);
	shaderProgram.setVec3(lightColorVar.c_str(), color);
	shaderProgram.setFloat(lightMultVar.c_str(), multiplier);
}

void DirectionalLight::debugRender() {
	glm::mat4 modelMat(1.0f);
	modelMat = glm::translate(modelMat, debugPos);
	modelMat = glm::scale(modelMat, glm::vec3(0.2f, 0.2f, 0.2f));
	shaderProgram.setMat4("model", modelMat);
	shaderProgram.setVec3("color", color);

	shaderProgram.bind();
	model.vao.bind();
	glDrawElements(GL_TRIANGLES, model.indicies.size(), GL_UNSIGNED_INT, 0);
	model.vao.unbind();
	shaderProgram.unbind();
}