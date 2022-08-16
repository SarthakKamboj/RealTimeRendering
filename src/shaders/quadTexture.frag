#version 330 core

uniform sampler2D texUnit;
in vec2 texCoords;
out vec4 FragColor;

void main() {
	FragColor = texture(texUnit, texCoords);
}
