#include <iostream>
#include <vector>
#include "SDL.h"
#include "input.h"
#include "loadModel.h"
#include "renderer/shaderProgram.h"
#include "renderer/vbo.h"
#include "renderer/vao.h"
#include <renderer/ebo.h>
#include "glm/gtc/type_ptr.hpp"
#include "renderer/meshRenderer.h"
#include "lights/pointLight.h"
#include "lights/directionalLight.h"
#include "lights/spotLight.h"
#include "transform.h"
#include "renderer/lightFrameBuffer.h"
#include "renderer/frameBuffer.h"
#include "ticTacToeSquare.h"
#include "ticTacGrid.h"
#include "gridManager.h"
#include "window.h"
#include "globals.h"
#include "renderer/texture_manager.h"
#include "model.h"
#include "renderer/shaderProgram.h"

float deltaTime = 0.0f;
uint32_t curTimeMs;

static float quadVertices[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

int width = 800;
int height = 600;

void light_pass(light_frame_buffer_t& light_fb, glm::mat4& lightView, glm::mat4& lightProj, shader_program_t& light_pass_program) {
	light_fb.bind();
	frame_buffer_t::clear_frame_buffer(glm::vec3(0, 0, 0));

	light_pass_program.set_mat_4("view", lightView);
	light_pass_program.set_mat_4("projection", lightProj);
	light_pass_program.render_models();

	/*
	const std::vector<transform_t>& transforms = light_pass_program.models.transforms;
	for (int i = 0; i < transforms.size(); i++) {
		glm::mat4 model_mat(1.0f);
		light_pass_program.models.transforms[i].get_model_matrix(model_mat);
		light_pass_program.set_mat_4("model", model_mat);
		light_pass_program.bind();

		const model_vertex_data_t& model = light_pass_program.models.vertices_data[i];
		const std::vector<texture_bind_data_t>& textures_data = light_pass_program.models.model_textures_data[i];

		for (const texture_bind_data_t& texture_data : textures_data) {
			glActiveTexture(GL_TEXTURE0 + texture_data.tex_unit);
			glBindTexture(GL_TEXTURE_2D, texture_data.texture_id);
		}

		glBindVertexArray(model.vao);
		glDrawElements(GL_TRIANGLES, model.num_indices_to_render, GL_UNSIGNED_INT, 0);
		light_pass_program.unbind();
	}
*/

	light_fb.unbind();
}

globals_t globals;

int main(int argc, char* args[]) {

	window_t window(800, 600);

	texture_manager_t texture_manager{};
	globals.texture_manager = &texture_manager;

	const std::string default_vert_path = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\default.vert";
	const std::string default_frag_path = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\default.frag";
	shader_program_t default_shader_program{};
	create_shader_program(default_shader_program, default_vert_path, default_frag_path);
	globals.default_shader_program = &default_shader_program;

	models_data_t models_data{};
	globals.models_data = &models_data;

	float radius = 12.0f;
	float yPos = 10.0f;
	float angle = 0.0f;
	glm::vec3 lookAt(0.0f, 0.0f, 0.0f);
	glm::mat4 perspProj = glm::perspective(glm::radians(45.0f), ((float)width) / height, 0.1f, 100.0f);
	float xExtent = 5.0f;
	float ratio = (float)height / (float)width;
	glm::mat4 orthoProj = glm::ortho(-xExtent, xExtent, -xExtent * ratio, xExtent * ratio, 0.1f, 100.0f);

	bool running = true;
	uint32_t prev = SDL_GetTicks();

	glm::vec3 pointLightColor(1, 0, 0);
	glm::vec3 pointLightPos(0, 3, 0);
	point_light_t point_light(pointLightColor, pointLightPos, 20.0f, 3.0f);

	glm::vec3 dirLightColor(1, 1, 1);
	glm::vec3 dir(0, -1, 0);
	dir_light_t dir_light(dirLightColor, dir, 1.0f);

	glm::vec3 spotLightColor(1, 1, 1);
	glm::vec3 spotLightPos(0, 4, 0.2f);
	glm::vec3 spotLightDir(0, -1, 0);
	spot_light_t spot_light(spotLightColor, spotLightPos, spotLightDir, 20.0f, glm::radians(60.0f), glm::radians(45.0f));

	light_frame_buffer_t spot_light_fb;
	light_frame_buffer_t dir_light_fb;

	const std::string light_pass_vert_path = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightPass.vert";
	const std::string light_pass_frag_path = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightPass.frag";
	shader_program_t light_pass_program{};
	create_shader_program(light_pass_program, light_pass_vert_path, light_pass_frag_path);

	int pcfLayers = 2;
	int turn = TTT_X;

	int TARGET_FPS = 60;
	float secPerFrame = 1.0f / TARGET_FPS;

	GridManager grid_manager(default_shader_program);

	while (running) {
		curTimeMs = SDL_GetTicks();
		deltaTime = ((curTimeMs - prev) / 1000.0f);
		while (deltaTime < secPerFrame) {
			curTimeMs = SDL_GetTicks();
			deltaTime = ((curTimeMs - prev) / 1000.0f);
		}
		prev = curTimeMs;

		Input input{};
		window.poll(input);

		running = !(input.quit || input.escape);

		grid_manager.update(input, turn);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		glm::vec3 camPos(radius * cos(glm::radians(angle)), yPos, radius * sin(glm::radians(angle)));
		glm::mat4 camView = glm::lookAt(camPos, lookAt, glm::vec3(0, 1, 0));

		// light pass
		/*
		std::vector<glm::mat4> modelMats;
		modelMats.resize(NUM_TTT_SQUARES * NUM_TTT_SQUARES);

		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				TicTacGrid& grid = gridManager.grids[row * 3 + col];
				for (int i = 0; i < grid.ticTacToeSquares.size(); i++) {
					TicTacToeSquare& ticTacToeSquare = grid.ticTacToeSquares[i];
					ticTacToeSquare.transform.getModelMatrix(modelMats[((row * 3 + col) * 9) + i]);
				}
			}
		}
		*/

		// std::vector<MeshRenderer*> meshRenderers(NUM_TTT_SQUARES * NUM_TTT_SQUARES, gridManager.grids[0].ticTacToeSquares[0].squareMeshRenderer);
		// light_pass(spot_light_fb, spot_light.spot_light_view, spot_light.spot_light_proj, light_pass_program);
		// light_pass(dir_light_fb, dir_light.dir_light_view, dir_light.dir_light_proj, light_pass_program);

		// full color pass
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);

		default_shader_program.set_int("numPointLights", 0);
		default_shader_program.set_int("numDirectionalLights", 0);
		default_shader_program.set_int("numSpotLights", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, spot_light_fb.depth_tex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, dir_light_fb.depth_tex);

		default_shader_program.set_mat_4("projection", perspProj);
		point_light.shader_program.set_mat_4("projection", perspProj);
		spot_light.shader_program.set_mat_4("projection", perspProj);

		default_shader_program.set_mat_4("view", camView);
		point_light.shader_program.set_mat_4("view", camView);
		spot_light.shader_program.set_mat_4("view", camView);

		point_light.set_point_light_in_shader(default_shader_program, 0);
		dir_light.set_dir_light_in_shader(default_shader_program, 0);
		spot_light.set_spot_light_in_shader_prog(default_shader_program, 0);

		default_shader_program.set_int("pcfLayers", pcfLayers);
		default_shader_program.render_models();

		// gridManager.render(default_shader_program, perspProj, camView);

		// point_light.debug_render();
		// spot_light.debug_render();

		// ImGui::Begin("Selection Transform");
		// ImGui::DragInt("pcf", &pcfLayers, 1, 0, 10);
		/*
		ImGui::DragFloat3("transform", &gridManager.grids[0].ticTacToeSquares[0].transform.pos.x, 0.1f);
		ImGui::DragFloat("scale", &gridManager.grids[0].ticTacToeSquares[0].transform.scale, 0.1f);
		ImGui::DragFloat3("rotation", &gridManager.grids[0].ticTacToeSquares[0].transform.rot.x, 1, -180.0f, 180.0f);
		*/

		/*
		ImGui::End();

		ImGui::Begin("Camera");
		ImGui::DragFloat("angle", &angle, 1, -180.0f, 180.0f);
		ImGui::DragFloat("y pos", &yPos, 0.1);
		ImGui::DragFloat("radius", &radius);
		ImGui::DragFloat3("look at", &lookAt.x);
		ImGui::End();

		ImGui::Begin("point Light");
		ImGui::DragFloat3("pos", &point_light.position.x, 0.1);
		ImGui::DragFloat("max dist", &point_light.max_dist, 0.1, 1);
		ImGui::DragFloat("multiplier", &point_light.multiplier, 0.05, 0);
		ImGui::ColorPicker3("color", &point_light.color.r);
		ImGui::End();

		ImGui::Begin("dir Light");
		ImGui::DragFloat3("dir", &dir_light.dir.x, 0.1);
		ImGui::DragFloat("multiplier", &dir_light.multiplier, 0.05, 0);
		ImGui::DragFloat("xEntext", &dir_light.x_extent, 0.05);
		ImGui::DragFloat("dirYPos", &dir_light.dir_y_pos, 0.05, 0);
		ImGui::ColorPicker3("color", &dir_light.color.r);
		ImGui::End();

		ImGui::Begin("spot Light");
		ImGui::DragFloat3("dir", &spot_light.dir.x, 0.1);
		ImGui::DragFloat3("pos", &spot_light.pos.x, 0.1);
		ImGui::DragFloat("multiplier", &spot_light.multiplier, 0.05, 0);
		ImGui::DragFloat("umbra", &spot_light.umbra, 0.01, 0, glm::radians(90.0f));
		ImGui::DragFloat("prenumbra", &spot_light.prenumbra, 0.01, 0, glm::radians(90.0f));
		ImGui::ColorPicker3("color", &spot_light.color.r);
		ImGui::End();
		*/

		window.swap();
	}

	return -1;
}