#pragma once

#include <vector>
#include "ticTacGrid.h"
#include "renderer/shaderProgram.h"

#define NO_ACTIVE_GRID -10

struct GridManager {
	GridManager();
	void update(const Input& input, int& turn);
	void render(ShaderProgram& shaderProgram, glm::mat4& projection, glm::mat4& view);

	std::vector<TicTacGrid> grids;
	int activeGrid = NO_ACTIVE_GRID;
	uint32_t timeOfLastClick;
	static const uint32_t TimeBetweenClicks;
	bool gridSelectionMode = true;
	bool firstGridSelectionMove = true;
};