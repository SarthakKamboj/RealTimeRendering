#version 330 core

out vec4 FragColor;

uniform sampler2D tex;

in vec2 texCoords;
in vec3 normal;
in vec3 worldPos;

struct PointLight {
	vec3 color;
	vec3 pos;
	float maxDist;
	float multiplier;
	vec3 pointColor;
};

struct DirectionalLight {
	vec3 dir;
	vec3 pointColor;
	float multiplier;
};

struct SpotLight {
	vec3 dir;
	vec3 pos;
	vec3 color;
};

uniform PointLight pointLights[5];
uniform int numPointLights;

uniform DirectionalLight directionalLights[5];
uniform int numDirectionalLights;

float windowFunc(float r, float maxDist) {
	float val = max(0, 1 - pow(r / maxDist, 4));
	return pow(val, 2);
}

float fallOffFunc(float lightDistSquared, float multiplier) {
	return multiplier / (lightDistSquared + 0.1);
}

vec4 calcPointLight(int i, vec4 surfaceColor) {
	vec3 lightVec = pointLights[i].pos - worldPos;
	float lightDistSquared = dot(lightVec, lightVec);
	float lightDist = sqrt(lightDistSquared);
	lightVec = lightVec / lightDist;
	float angleMultiplier = clamp(dot(lightVec, normal), 0, 1);
	vec3 pointColor = pointLights[i].pointColor;
	return (angleMultiplier * vec4(pointColor, 1.0) * surfaceColor * fallOffFunc(lightDistSquared, pointLights[i].multiplier) * windowFunc(lightDist, pointLights[i].maxDist));
}

vec4 calcDirLight(int i, vec4 surfaceColor) {
	vec3 lightVec = -normalize(directionalLights[i].dir);
	float angleMultiplier = clamp(dot(lightVec, normal), 0, 1);
	vec3 pointColor = directionalLights[i].pointColor;
	return (directionalLights[i].multiplier * angleMultiplier * vec4(pointColor, 1.0) * surfaceColor);
}

void main() {
	vec4 surfaceColor = texture(tex, texCoords);

	FragColor = surfaceColor * 0.1;

	for (int i = 0; i < numPointLights; i++) {
		FragColor += calcPointLight(i, surfaceColor);
	}

	for (int i = 0; i < numDirectionalLights; i++) {
		FragColor += calcDirLight(i, surfaceColor);
	}
}