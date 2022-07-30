#include "pointLight.h"

PointLight::PointLight(const glm::vec3& _color, const glm::vec3& _position, float _maxDist, float _multiplier) {
	maxDist = _maxDist;
	multiplier = _multiplier;
	color = _color;
	position = _position;

	const std::string vertPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\light.vert";
	const std::string fragPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\light.frag";
	shaderProgram = ShaderProgram(vertPath.c_str(), fragPath.c_str());

	std::string cubePath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\cube.obj";
	loadModel(cubePath, model);
}

void PointLight::setPointLightInShader(ShaderProgram& shaderProgram, int idx) {
	std::string pointLightPosVar = "pointLights[" + std::to_string(idx) + "].pos";
	std::string pointLightColorVar = "pointLights[" + std::to_string(idx) + "].pointColor";
	std::string pointLightMaxDistVar = "pointLights[" + std::to_string(idx) + "].maxDist";
	std::string pointLightMultiplierVar = "pointLights[" + std::to_string(idx) + "].multiplier";
	shaderProgram.setVec3(pointLightPosVar.c_str(), position);
	shaderProgram.setFloat(pointLightMaxDistVar.c_str(), maxDist);
	shaderProgram.setFloat(pointLightMultiplierVar.c_str(), multiplier);
	shaderProgram.setVec3(pointLightColorVar.c_str(), color);
}

void PointLight::debugRender() {
	glm::mat4 modelMat(1.0f);
	modelMat = glm::translate(modelMat, position);
	modelMat = glm::scale(modelMat, glm::vec3(0.2f, 0.2f, 0.2f));
	shaderProgram.setMat4("model", modelMat);
	shaderProgram.setVec3("color", color);

	shaderProgram.bind();
	model.vao.bind();
	glDrawElements(GL_TRIANGLES, model.indicies.size(), GL_UNSIGNED_INT, 0);
	model.vao.unbind();
	shaderProgram.unbind();
}
