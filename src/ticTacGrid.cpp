#include "ticTacGrid.h"
#include "ticTacToeSquare.h"
#include "input.h"

extern float deltaTime;
extern uint32_t curTimeMs;

const uint32_t TicTacGrid::TimeBetweenClicks = 300;
const float TicTacGrid::ActiveYPosAdd = 0.5f;

TicTacGrid::TicTacGrid(const glm::vec2& centerPos) {

	ticTacToeSquares.resize(NUM_TTT_SQUARES);
	meshRenderers.resize(NUM_TTT_SQUARES);

	std::string selectionModelPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\selection.obj";
	std::string selectionTexPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\selection.png";
	squareMeshRenderer = new MeshRenderer(selectionModelPath, selectionTexPath);

	int startRow = centerPos.x - 1;
	int startCol = centerPos.y - 1;
	for (int row = 0; row < GRID_ROWS; row++) {
		for (int col = 0; col < GRID_COLS; col++) {
			ticTacToeSquares[row * 3 + col] = TicTacToeSquare(row + startRow, col + startCol, squareMeshRenderer);
			meshRenderers[row * 3 + col] = squareMeshRenderer;
		}
	}
}

void TicTacGrid::update(const Input& input, int turn, bool pollInput) {

	bool selectionChanged = false;
	if (curTimeMs - timeOfLastClick > TimeBetweenClicks && pollInput) {
		if (input.rightArrow) {
			selectedSquare = (selectedSquare - 1) % NUM_TTT_SQUARES;
			selectionChanged = true;
		}
		else if (input.leftArrow) {
			selectedSquare = (selectedSquare + 1) % NUM_TTT_SQUARES;
			selectionChanged = true;
		}
		else if (input.upArrow) {
			selectedSquare = (selectedSquare - 3) % NUM_TTT_SQUARES;
			selectionChanged = true;
		}
		else if (input.downArrow) {
			selectedSquare = (selectedSquare + 3) % NUM_TTT_SQUARES;
			selectionChanged = true;
		}
		else if (input.enter) {
			selectedSquare = -1;
		}

		if (selectionChanged) {
			timeOfLastClick = curTimeMs;
		}

		if (selectedSquare < 0) selectedSquare += NUM_TTT_SQUARES;
	}

	for (int i = 0; i < ticTacToeSquares.size(); i++) {
		if (pollInput) {
			if (selectionChanged && ticTacToeSquares[i].selected) {
				ticTacToeSquares[i].deSelect();
			}
			if (selectionChanged && i == selectedSquare) {
				ticTacToeSquares[i].select();
			}
			ticTacToeSquares[i].update(input.enter, turn);
		}
		else {
			ticTacToeSquares[i].update(false, turn);
		}
	}

	char board[GRID_ROWS][GRID_COLS]{};
	for (int row = 0; row < GRID_ROWS; row++) {
		for (int col = 0; col < GRID_COLS; col++) {
			const TicTacToeSquare& ttts = ticTacToeSquares[(row * 3) + col];
			board[row][col] = ttts.officiallySelected ? ttts.curOption : NEITHER;
		}
	}

	winner = TicTacGrid::GetWinner(board);
}

void TicTacGrid::select() {
	selected = true;
	ticTacToeSquares[selectedSquare].deSelect();
	/*
	for (int i = 0; i < ticTacToeSquares.size(); i++) {
		ticTacToeSquares[i].transform.pos.y += ActiveYPosAdd;
		ticTacToeSquares[i].minY += ActiveYPosAdd;
		ticTacToeSquares[i].maxY += ActiveYPosAdd;
	}
	*/
}

void TicTacGrid::deSelect() {
	selected = false;
	ticTacToeSquares[selectedSquare].deSelect();
	/*
	for (int i = 0; i < ticTacToeSquares.size(); i++) {
		ticTacToeSquares[i].transform.pos.y -= ActiveYPosAdd;
		ticTacToeSquares[i].minY -= ActiveYPosAdd;
		ticTacToeSquares[i].maxY -= ActiveYPosAdd;
	}
	*/
}

void TicTacGrid::render(ShaderProgram& shaderProgram) {
	glm::mat4 parentMat(1.0f);
	if (selected) {
		parentMat = glm::translate(parentMat, glm::vec3(0, ActiveYPosAdd, 0));
	}
	shaderProgram.bind();
	for (int i = 0; i < ticTacToeSquares.size(); i++) {
		ticTacToeSquares[i].render(shaderProgram, parentMat);
	}
	shaderProgram.unbind();
}


bool TicTacGrid::GetWinnerRow(const char board[GRID_ROWS][GRID_COLS], int row) {
	return board[row][0] == board[row][1] && board[row][0] == board[row][2];
}

bool TicTacGrid::GetWinnerCol(const char board[GRID_ROWS][GRID_COLS], int col) {
	return board[0][col] == board[1][col] && board[1][col] == board[2][col];
}

int TicTacGrid::GetWinner(const char board[GRID_ROWS][GRID_COLS]) {
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
		return board[0][0];
	}
	if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
		return board[0][2];
	}
	for (int i = 0; i < 3; i++) {
		if (TicTacGrid::GetWinnerRow(board, i)) {
			return board[i][0];
		}
		if (TicTacGrid::GetWinnerCol(board, i)) {
			return board[0][i];
		}
	}
	return NEITHER;
}

