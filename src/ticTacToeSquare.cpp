#include "ticTacToeSquare.h"

extern float deltaTime;

float lerp(float from, float to, float time) {
	time = std::min(1.0f, std::max(0.0f, time));
	return from + ((to - from) * time);
}

const float TicTacToeSquare::selectAnimTime = 0.45f;

TicTacToeSquare::TicTacToeSquare() {}

TicTacToeSquare::TicTacToeSquare(float posX, float posZ, MeshRenderer* _squareMeshRenderer) {
	minY = 0.0f;
	maxY = 0.25f;

	transform.pos.x = posX;
	transform.pos.z = posZ;
	transform.pos.y = minY;
	transform.scale = 0.25f;

	squareMeshRenderer = _squareMeshRenderer;
}

void TicTacToeSquare::update(bool enterClicked, int& turn) {
	if (selected) {
		transform.pos.y = lerp(minY, maxY, relativeTime);
		relativeTime += deltaTime / TicTacToeSquare::selectAnimTime;
	}
	else if (transform.pos.y != minY) {
		transform.pos.y = lerp(maxY, minY, relativeTime);
		relativeTime += deltaTime / TicTacToeSquare::selectAnimTime;
	}

	if (officiallySelected) return;

	if (selected) {
		curOptionTex = X_TEX_UNIT + turn;
		curOption = TTT_X + turn;
		if (enterClicked) {
			officiallySelected = true;
			deSelect();
			turn = (turn == TTT_X) ? TTT_Y : TTT_X;
		}
	}
	else {
		if (!officiallySelected) {
			curOptionTex = NEITHER;
			curOption = NEITHER;
		}
	}
}

void TicTacToeSquare::render(ShaderProgram& shaderProgram, glm::mat4& parentMat) {
	shaderProgram.setInt("imgTex", curOptionTex);
	shaderProgram.setInt("chosenMultiplier", curOption != NEITHER);
	glm::mat4 modelMatrix;
	transform.getModelMatrix(modelMatrix);
	modelMatrix = parentMat * modelMatrix;
	shaderProgram.setMat4("model", modelMatrix);
	shaderProgram.bind();
	squareMeshRenderer->render();
	shaderProgram.unbind();
}

void TicTacToeSquare::select() {
	selected = true;
	relativeTime = (transform.pos.y - minY) / (maxY - minY);
}

void TicTacToeSquare::deSelect() {
	selected = false;
	relativeTime = 1.0f - ((transform.pos.y - minY) / (maxY - minY));
}