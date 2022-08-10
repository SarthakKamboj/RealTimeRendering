#include "directionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& _color, const glm::vec3& _dir, float _multiplier) {
	multiplier = _multiplier;
	color = _color;
	dir = _dir;

	const std::string vertPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightDebug.vert";
	const std::string fragPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightDebug.frag";
	shaderProgram = ShaderProgram(vertPath.c_str(), fragPath.c_str());

	std::string cubePath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\arrow.obj";
	loadModel(cubePath, model);
}

void DirectionalLight::setDirectionalLightInShader(ShaderProgram& shaderProgram, int idx) {

	dirLightView = LightFrameBuffer::GetLightViewMat(glm::vec3(0, 0, 0) - (dirYPos * dir), dir);
	dirLightProj = LightFrameBuffer::GetDirLightProjMat(xEntext);

	std::string lightDirVar = "directionalLights[" + std::to_string(idx) + "].dir";
	std::string lightColorVar = "directionalLights[" + std::to_string(idx) + "].pointColor";
	std::string lightMultVar = "directionalLights[" + std::to_string(idx) + "].multiplier";
	std::string lightSUVar = "directionalLights[" + std::to_string(idx) + "].samplerUnit";
	std::string lightProjVar = "directionalLights[" + std::to_string(idx) + "].proj";
	std::string lightViewVar = "directionalLights[" + std::to_string(idx) + "].view";

	shaderProgram.setVec3(lightDirVar.c_str(), dir);
	shaderProgram.setVec3(lightColorVar.c_str(), color);
	shaderProgram.setFloat(lightMultVar.c_str(), multiplier);
	shaderProgram.setInt(lightSUVar.c_str(), 2);
	shaderProgram.setMat4(lightProjVar.c_str(), dirLightProj);
	shaderProgram.setMat4(lightViewVar.c_str(), dirLightView);
}

void DirectionalLight::debugRender() {
	glm::mat4 modelMat(1.0f);
	modelMat = glm::translate(modelMat, debugPos);
	modelMat = glm::scale(modelMat, glm::vec3(0.2f, 0.2f, 0.2f));
	shaderProgram.setMat4("model", modelMat);
	shaderProgram.setVec3("color", color);

	shaderProgram.bind();
	model.render();
	shaderProgram.unbind();
}