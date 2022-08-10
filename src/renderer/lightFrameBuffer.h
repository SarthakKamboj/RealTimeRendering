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

	static glm::mat4 GetLightViewMat(const glm::vec3& lightPos, const glm::vec3& lightDir);
	static glm::mat4 GetDirLightProjMat(float xExtent);
	static glm::mat4 GetSpotLightProjMat(float angle);
};
