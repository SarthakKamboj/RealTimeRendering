#pragma once

#include "renderer/meshRenderer.h"
#include "renderer/shaderProgram.h"
#include "transform.h"

#define X_TEX_UNIT 3
#define O_TEX_UNIT 4

#define NEITHER -1
#define TTT_X 0
#define TTT_Y 1

float lerp(float from, float to, float time);

class TicTacToeSquare {
public:
	TicTacToeSquare();
	// TicTacToeSquare(float posX, float posZ, MeshRenderer* _squareMeshRenderer);
	TicTacToeSquare(float posX, float posZ, shader_program_t& shaderProgram);
	void update(bool enterClicked, int& turn);
	// void render(shader_program_t& shaderProgram, glm::mat4& parentMat);
	void select();
	void deSelect();

	// Transform transform;
	bool selected = false;
	transform_t* transform;

	bool officiallySelected = false;
	int curOption = NEITHER;

	float minY;
	float maxY;

private:
	int model_idx;
	int curOptionTex = NEITHER;
	float relativeTime = 0.0f;
	static const float selectAnimTime;
};
