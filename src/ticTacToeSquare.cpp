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
	std::string xPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\x.png";
	std::string oPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\o.png";

	models_manager_t& models_manager = globals.default_shader_program->models_manager;
	model_idx = load_model(*globals.default_shader_program, selectionModelPath);
	attach_texture_to_model(models_manager, selectionTexPath, model_idx, 0);
	attach_texture_to_model(models_manager, xPath, model_idx, 1);
	attach_texture_to_model(models_manager, oPath, model_idx, 2);

	transform_t& model_transform = models_manager.transforms[model_idx];

	model_transform.pos.x = posX;
	model_transform.pos.z = posZ;
	model_transform.pos.y = minY;
	model_transform.scale = 0.25f;
}

void TicTacToeSquare::update(bool enterClicked, int& turn) {

	transform_t& transform = globals.default_shader_program->models_manager.transforms[model_idx];

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
		globals.default_shader_program->shader_parameters[model_idx]["selected_tex"] = turn + 1;
		globals.default_shader_program->shader_parameters[model_idx]["selected"] = 1;
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

			globals.default_shader_program->shader_parameters[model_idx]["selected"] = 0;
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
	transform_t& transform = globals.default_shader_program->models_manager.transforms[model_idx];
	selected = true;
	relativeTime = (transform.pos.y - minY) / (maxY - minY);
}

void TicTacToeSquare::deSelect() {
	transform_t& transform = globals.default_shader_program->models_manager.transforms[model_idx];
	selected = false;
	relativeTime = 1.0f - ((transform.pos.y - minY) / (maxY - minY));
}