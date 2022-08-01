#include "spotLight.h"

SpotLight::SpotLight(const glm::vec3& _color, const glm::vec3& _position, glm::vec3& _dir, float _multiplier, float _umbra, float _prenumbra) {
	multiplier = _multiplier;
	color = _color;
	pos = _position;
	dir = _dir;
	umbra = _umbra;
	prenumbra = _prenumbra;

	const std::string vertPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightDebug.vert";
	const std::string fragPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightDebug.frag";
	shaderProgram = ShaderProgram(vertPath.c_str(), fragPath.c_str());

	std::string cubePath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\cube.obj";
	loadModel(cubePath, model);
}

void SpotLight::setSpotLightInShader(ShaderProgram& shaderProgram, int idx) {
	std::string lightPosVar = "spotLights[" + std::to_string(idx) + "].pos";
	std::string lightColorVar = "spotLights[" + std::to_string(idx) + "].pointColor";
	std::string lightDirVar = "spotLights[" + std::to_string(idx) + "].dir";
	std::string lightMultiplierVar = "spotLights[" + std::to_string(idx) + "].multiplier";
	std::string lightUmbraVar = "spotLights[" + std::to_string(idx) + "].umbra";
	std::string lightPrenumbraVar = "spotLights[" + std::to_string(idx) + "].prenumbra";
	shaderProgram.setVec3(lightPosVar.c_str(), pos);
	shaderProgram.setVec3(lightColorVar.c_str(), color);
	shaderProgram.setVec3(lightDirVar.c_str(), dir);
	shaderProgram.setFloat(lightMultiplierVar.c_str(), multiplier);
	shaderProgram.setFloat(lightUmbraVar.c_str(), umbra);
	shaderProgram.setFloat(lightPrenumbraVar.c_str(), prenumbra);
}

void SpotLight::debugRender() {
	glm::mat4 modelMat(1.0f);
	modelMat = glm::translate(modelMat, pos);
	modelMat = glm::scale(modelMat, glm::vec3(0.2f, 0.2f, 0.2f));
	shaderProgram.setMat4("model", modelMat);
	shaderProgram.setVec3("color", color);

	shaderProgram.bind();
	/*
	model.vao.bind();
	glDrawElements(GL_TRIANGLES, model.indicies.size(), GL_UNSIGNED_INT, 0);
	model.vao.unbind();
	*/
	model.render();
	shaderProgram.unbind();
}