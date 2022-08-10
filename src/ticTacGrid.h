#pragma once

#include <vector>
#include "renderer/meshRenderer.h"
#include "input.h"
#include "ticTacToeSquare.h"

#define GRID_ROWS 3
#define GRID_COLS 3
#define NUM_TTT_SQUARES (GRID_ROWS * GRID_COLS)

class TicTacGrid {
public:
	TicTacGrid(const glm::vec2& centerPos);
	void update(const Input& input, int turn, bool pollInput);
	void render(ShaderProgram& shaderProgram);

	void select();
	void deSelect();

	int winner = NEITHER;
	bool selected = false;
	std::vector<TicTacToeSquare> ticTacToeSquares;
	std::vector<MeshRenderer*> meshRenderers;

	static int GetWinner(const char board[GRID_ROWS][GRID_COLS]);
	static bool GetWinnerRow(const char board[GRID_ROWS][GRID_COLS], int row);
	static bool GetWinnerCol(const char board[GRID_ROWS][GRID_COLS], int col);

private:
	int selectedSquare = 0;
	MeshRenderer* squareMeshRenderer = nullptr;

	uint32_t timeOfLastClick = 0;
	static const uint32_t TimeBetweenClicks;
	static const float ActiveYPosAdd;
};
