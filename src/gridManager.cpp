#include "gridManager.h"

extern uint32_t curTimeMs;

const uint32_t GridManager::TimeBetweenClicks = 100;

GridManager::GridManager() {
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			grids.push_back(TicTacGrid(glm::vec2(row * 3, col * 3)));
		}
	}
}

void GridManager::update(const Input& input, int turn) {

	bool selectionChanged = false;
	bool changedSelectionMode = false;
	int prevActiveGrid;
	if (curTimeMs - timeOfLastClick > TimeBetweenClicks) {
		if (gridSelectionMode) {
			if (input.rightArrow) {
				prevActiveGrid = activeGrid;
				activeGrid = (activeGrid - 1) % NUM_TTT_SQUARES;
				selectionChanged = true;
			}
			else if (input.leftArrow) {
				prevActiveGrid = activeGrid;
				activeGrid = (activeGrid + 1) % NUM_TTT_SQUARES;
				selectionChanged = true;
			}
			else if (input.upArrow) {
				prevActiveGrid = activeGrid;
				activeGrid = (activeGrid - 3) % NUM_TTT_SQUARES;
				selectionChanged = true;
			}
			else if (input.downArrow) {
				prevActiveGrid = activeGrid;
				activeGrid = (activeGrid + 3) % NUM_TTT_SQUARES;
				selectionChanged = true;
			}
		}

		if (input.enter && gridSelectionMode) {
			gridSelectionMode = false;
			changedSelectionMode = true;
		}
		else if (input.slash) {
			gridSelectionMode = true;
			changedSelectionMode = true;
		}

		if (selectionChanged) {
			timeOfLastClick = curTimeMs;
		}

		if (activeGrid < 0 && activeGrid != NO_ACTIVE_GRID) activeGrid += NUM_TTT_SQUARES;
	}

	if (changedSelectionMode && gridSelectionMode && activeGrid != NO_ACTIVE_GRID) {
		grids[activeGrid].deSelect();
		prevActiveGrid = NO_ACTIVE_GRID;
		firstGridSelectionMove = true;
	}

	if (selectionChanged) {
		if (prevActiveGrid != NO_ACTIVE_GRID && !firstGridSelectionMove) {
			grids[prevActiveGrid].deSelect();
		}
		grids[activeGrid].select();
		firstGridSelectionMove = false;
	}

	char board[GRID_ROWS][GRID_COLS]{};
	for (int row = 0; row < GRID_ROWS; row++) {
		for (int col = 0; col < GRID_COLS; col++) {
			const TicTacGrid& grid = grids[(row * 3) + col];
			board[row][col] = grid.winner;
		}
	}

	int winner = TicTacGrid::GetWinner(board);

	/*
	if (!gridSelectionMode) {
		grids[activeGrid].update(input, turn, gridSe);
	}
	*/

	if (gridSelectionMode) {
		for (int i = 0; i < grids.size(); i++) {
			TicTacGrid& grid = grids[i];
			grid.update(input, turn, false);
		}
	}
	else {
		grids[activeGrid].update(input, turn, true);
	}
}

void GridManager::render(ShaderProgram& shaderProgram) {
	for (TicTacGrid& grid : grids) {
		grid.render(shaderProgram);
	}
}