#pragma once

#include "renderer/meshRenderer.h"
#include "renderer/shaderProgram.h"
#include "transform.h"

#define X_TEX_UNIT 3
#define O_TEX_UNIT 4

float lerp(float from, float to, float time);

class TicTacToeSquare {
public:
	TicTacToeSquare();
	TicTacToeSquare(float posX, float posZ, MeshRenderer* _squareMeshRenderer);
	void update(bool spaceClicked);
	void render(ShaderProgram& shaderProgram);
	void select();
	void deSelect();

	Transform transform;
	bool selected = false;

	MeshRenderer* squareMeshRenderer = NULL;

private:
	int curOption = X_TEX_UNIT;
	float relativeTime = 0.0f;
	static const float selectAnimTime;
	static const float minY;
	static const float maxY;
};
