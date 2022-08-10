#include "lightFrameBuffer.h"
#include "glad/glad.h"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"

extern int width, height;

LightFrameBuffer::LightFrameBuffer() {
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

	// create framebuffer depth texture
	// don't need to make color texture since we will just be sampling from depth texture created
	// by the light passes that use the lightframebuffer
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_FRAMEBUFFER, 0);
}

void LightFrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void LightFrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 LightFrameBuffer::GetLightViewMat(const glm::vec3& lightPos, const glm::vec3& lightDir) {
	const glm::vec3 lookAtPos = lightPos + glm::normalize(lightDir) + glm::vec3(0, 0, 0.000001f);
	return glm::lookAt(lightPos, lookAtPos, glm::vec3(0, 1, 0));
}

glm::mat4 LightFrameBuffer::GetDirLightProjMat(float xExtent) {
	// float xExtent = 2.0f;
	float ratio = (float)height / (float)width;
	return glm::ortho(-xExtent, xExtent, -xExtent * ratio, xExtent * ratio, 0.1f, 100.0f);
}

glm::mat4 LightFrameBuffer::GetSpotLightProjMat(float angle) {
	return glm::perspective(angle, (float)width / (float)height, 0.1f, 100.0f);
}

/*
glm::mat4 LightFrameBuffer::getLightProjMat(LightType lightType) {
	if (lightType == LightType::SPOTLIGHT) {
		// return glm::lookAt(light->pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	}

	if (lightType == LightType::DIRECTIONAL) {
		// return glm::lookAt(light->pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	}
}
*/
