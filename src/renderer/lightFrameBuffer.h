#pragma once

#include "light.h"

struct LightFrameBuffer {

	enum LightType {
		SPOTLIGHT, DIRECTIONAL
	};

	LightFrameBuffer();
	void bind();
	void unbind();

	unsigned int fbo;
	unsigned int colorTexture;
	unsigned int depthTexture;

	glm::mat4 getLightViewMat(const glm::vec3& lightPos, const glm::vec3& lightDir);
	glm::mat4 getDirLightProjMat();
	glm::mat4 getSpotLightProjMat(float angle);
};
