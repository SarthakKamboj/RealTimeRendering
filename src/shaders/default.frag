#version 330 core

out vec4 FragColor;

uniform sampler2D tex;
uniform sampler2D lightDepthTexUnit;

in vec2 texCoords;
in vec3 normal;
in vec4 worldPos;

uniform mat4 lightProj;
uniform mat4 lightView;
uniform vec3 color;

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
	vec3 pointColor;
	float umbra;
	float prenumbra;
	float multiplier;
};

uniform PointLight pointLights[5];
uniform int numPointLights;

uniform DirectionalLight directionalLights[5];
uniform int numDirectionalLights;

uniform SpotLight spotLights[5];
uniform int numSpotLights;

float windowFunc(float r, float maxDist) {
	float val = max(0, 1 - pow(r / maxDist, 4));
	return pow(val, 2);
}

float distanceFallOffFunc(float lightDistSquared) {
	return 1.0 / (lightDistSquared + 0.1);
}

float dirFalloffFunc(vec3 lightToSurface, int i) {
	float lDotSpot = dot(lightToSurface, normalize(spotLights[i].dir));
	float prenumbra = spotLights[i].prenumbra;
	float umbra = max(spotLights[i].umbra, prenumbra);
	float t = clamp((lDotSpot - cos(umbra)) / (cos(prenumbra) - cos(umbra) + 0.1), 0, 1);
	return pow(t, 2);
}

vec4 calcPointLight(int i, vec4 surfaceColor) {
	vec3 lightVec = pointLights[i].pos - worldPos.xyz;
	float lightDistSquared = dot(lightVec, lightVec);
	float lightDist = sqrt(lightDistSquared);
	lightVec = lightVec / lightDist;
	float angleMultiplier = clamp(dot(lightVec, normal), 0, 1);
	vec3 pointColor = pointLights[i].pointColor;
	return pointLights[i].multiplier * angleMultiplier * vec4(pointColor, 1.0) * surfaceColor * distanceFallOffFunc(lightDistSquared) * windowFunc(lightDist, pointLights[i].maxDist);
}

vec4 calcDirLight(int i, vec4 surfaceColor) {
	vec3 lightVec = -normalize(directionalLights[i].dir);
	float angleMultiplier = clamp(dot(lightVec, normal), 0, 1);
	vec3 pointColor = directionalLights[i].pointColor;
	return directionalLights[i].multiplier * angleMultiplier * vec4(pointColor, 1.0) * surfaceColor;
}

vec4 calcSpotLight(int i, vec4 surfaceColor) {
	vec3 lightToSurface = worldPos.xyz - spotLights[i].pos;
	float lightDistSquared = dot(lightToSurface, lightToSurface);
	lightToSurface = lightToSurface / sqrt(lightDistSquared);

	vec3 pointColor = spotLights[i].pointColor;
	vec3 lightVec = -lightToSurface;
	float angleMultiplier = clamp(dot(lightVec, normal), 0, 1);

	return spotLights[i].multiplier * angleMultiplier * vec4(pointColor, 1.0) * surfaceColor * distanceFallOffFunc(lightDistSquared) * dirFalloffFunc(lightToSurface, i);
}

float ndcToTex(float ndc) {
	return (ndc * 0.5) + 0.5;
}

void main() {

	vec4 surfaceColor = texture(tex, texCoords);
	FragColor = surfaceColor * 0.1;

	vec4 lightRelPos = lightProj * lightView * worldPos;
	lightRelPos /= lightRelPos.w;
	vec2 lightDepthUv = vec2(ndcToTex(lightRelPos.x), ndcToTex(lightRelPos.y));

	if (lightDepthUv.x < 0 || lightDepthUv.x > 1 || lightDepthUv.y < 0 || lightDepthUv.y > 1) {
		// FragColor = vec4(0,0,0,1);
		return;
	}

	float depthVal = ndcToTex(lightRelPos.z);

	float closestZVal = texture(lightDepthTexUnit, lightDepthUv).x;

	if (depthVal - 0.0025 > closestZVal) {
		// FragColor = vec4(0,0,0,1);
		return;	
	}

	for (int i = 0; i < numPointLights; i++) {
		FragColor += calcPointLight(i, surfaceColor);
	}

	for (int i = 0; i < numDirectionalLights; i++) {
		 FragColor += calcDirLight(i, surfaceColor);
	}

	for (int i = 0; i < numSpotLights; i++) {
		FragColor += calcSpotLight(i, surfaceColor);
	}
}