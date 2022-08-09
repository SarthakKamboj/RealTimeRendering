#include "ticTacToeSquare.h"

extern float deltaTime;

float lerp(float from, float to, float time) {
	time = std::min(1.0f, std::max(0.0f, time));
	return from + ((to - from) * time);
}

const float TicTacToeSquare::selectAnimTime = 0.2f;
const float TicTacToeSquare::minY = 0.0f;
const float TicTacToeSquare::maxY = 0.5f;

TicTacToeSquare::TicTacToeSquare() {}

TicTacToeSquare::TicTacToeSquare(float posX, float posZ, MeshRenderer* _squareMeshRenderer) {
	transform.pos.x = posX;
	transform.pos.z = posZ;
	transform.pos.y = minY;
	transform.scale = 0.25f;

	squareMeshRenderer = _squareMeshRenderer;
}

void TicTacToeSquare::update(bool spaceClicked) {
	if (selected) {
		transform.pos.y = lerp(minY, maxY, relativeTime);
		relativeTime += deltaTime / TicTacToeSquare::selectAnimTime;
	}
	else if (transform.pos.y != minY) {
		transform.pos.y = lerp(maxY, minY, relativeTime);
		relativeTime += deltaTime / TicTacToeSquare::selectAnimTime;
	}

	if (spaceClicked && selected) {
		curOption = (((curOption - X_TEX_UNIT) + 1) % 2) + X_TEX_UNIT;
	}
}

void TicTacToeSquare::render(ShaderProgram& shaderProgram) {
	shaderProgram.setInt("imgTex", curOption);
	glm::mat4 modelMatrix;
	transform.getModelMatrix(modelMatrix);
	shaderProgram.setMat4("model", modelMatrix);
	shaderProgram.bind();
	squareMeshRenderer->render();
	shaderProgram.unbind();
}

void TicTacToeSquare::select() {
	selected = true;
	relativeTime = transform.pos.y / maxY;
}

void TicTacToeSquare::deSelect() {
	selected = false;
	relativeTime = 1.0f - (transform.pos.y / maxY);
}