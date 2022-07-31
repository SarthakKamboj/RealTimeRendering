#version 330 core

uniform sampler2D depthTexUnit;

in vec2 tex;
out vec4 FragColor;

void main() {
	// float val = (extraVisible * pow(texture(depthTexUnit, tex).x, 32)) + ((1-extraVisible) * texture(depthTexUnit, tex).x);
	float val = pow(texture(depthTexUnit, tex).x, 32);
	FragColor = vec4(val, val, val, 1.0);
	// FragColor = vec4(0, 1, 0, 1) * ((tex.x * 0.5f) + 0.25f);
	// FragColor = vec4(tex, 0, 1);
}
