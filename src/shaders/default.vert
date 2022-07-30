#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoords;
out vec3 normal;
out vec3 worldPos;

void main() {
	vec4 worldPosVec4 = model * vec4(aPos, 1.0);
	worldPos = worldPosVec4.xyz;
	gl_Position = projection * view * worldPosVec4;
	texCoords = aTexCoords;
	normal = normalize(model * vec4(aNormal, 1.0)).xyz;
}