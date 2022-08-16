#include "ticTacToeSquare.h"
#include "globals.h"

extern float deltaTime;
extern globals_t globals;

float lerp(float from, float to, float time) {
	time = std::min(1.0f, std::max(0.0f, time));
	return from + ((to - from) * time);
}

const float TicTacToeSquare::selectAnimTime = 0.45f;

TicTacToeSquare::TicTacToeSquare() {}

TicTacToeSquare::TicTacToeSquare(float posX, float posZ, shader_program_t& shaderProgram) {
	minY = 0.0f;
	maxY = 0.25f;

	std::string selectionModelPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\selection.obj";
	std::string selectionTexPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\selection.png";
	models_manager_t& models_manager = globals.default_shader_program->models_manager;
	model_idx = load_model(models_manager, selectionModelPath);
	attach_texture_to_model(models_manager, selectionTexPath, model_idx, 0);
	// modelIdx = load_model(shaderProgram.models, selectionModelPath);

	transform_t& model_transform = models_manager.transforms[model_idx];
	std::cout << model_idx << std::endl;

	model_transform.pos.x = posX;
	model_transform.pos.z = posZ;
	model_transform.pos.y = minY;
	model_transform.scale = 0.25f;

	transform = &model_transform;
}

void TicTacToeSquare::update(bool enterClicked, int& turn) {

	transform = &globals.default_shader_program->models_manager.transforms[model_idx];

	if (selected) {
		transform->pos.y = lerp(minY, maxY, relativeTime);
		relativeTime += deltaTime / TicTacToeSquare::selectAnimTime;
	}
	else if (transform->pos.y != minY) {
		transform->pos.y = lerp(maxY, minY, relativeTime);
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

/*
void TicTacToeSquare::render(shader_program_t& shaderProgram, glm::mat4& parentMat) {
	shaderProgram.setInt("imgTex", curOptionTex);
	shaderProgram.setInt("chosenMultiplier", curOption != NEITHER);
	glm::mat4 modelMatrix;
	transform->getModelMatrix(modelMatrix);
	modelMatrix = parentMat * modelMatrix;
	shaderProgram->setMat4("model", modelMatrix);
	shaderProgram.bind();
	squareMeshRenderer->render();
	shaderProgram.unbind();
}
*/

void TicTacToeSquare::select() {
	selected = true;
	relativeTime = (transform->pos.y - minY) / (maxY - minY);
}

void TicTacToeSquare::deSelect() {
	selected = false;
	relativeTime = 1.0f - ((transform->pos.y - minY) / (maxY - minY));
}