#include "gridManager.h"

extern uint32_t curTimeMs;

const uint32_t GridManager::TimeBetweenClicks = 100;

GridManager::GridManager(shader_program_t& shader_program) {
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			TicTacGrid tic_tac_grid(glm::vec2((row - 1) * 3, (col - 1) * 3), shader_program);
			grids.push_back(tic_tac_grid);
		}
	}
}

void GridManager::update(const Input& input, int& turn) {

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


	if (gridSelectionMode) {
		for (int i = 0; i < grids.size(); i++) {
			TicTacGrid& grid = grids[i];
			grid.update(input, turn, false);
		}
	}
	else {
		grids[activeGrid].update(input, turn, true);
	}

	int winner = TicTacGrid::GetWinner(board);
	if (winner != NEITHER) {
		if (winner == TTT_X) {
			std::cout << "x wins" << std::endl;
		}
		else {
			std::cout << "o wins" << std::endl;
		}
	}
}

/*
void GridManager::render(shader_program_t& shaderProgram, glm::mat4& projection, glm::mat4& view) {
	for (TicTacGrid& grid : grids) {
		grid.render(shaderProgram, projection, view);
	}
}
*/