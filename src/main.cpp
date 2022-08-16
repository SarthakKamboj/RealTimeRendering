#include <iostream>
#include <vector>
#include "SDL.h"
#include "glad/glad.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "imgui.h"
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

void lightPassFb(LightFrameBuffer& fb, glm::mat4& lightView, glm::mat4& lightProj, ShaderProgram& lightPassShaderProgram,
	std::vector<MeshRenderer*>& meshRenderers, const std::vector<glm::mat4>& modelMats
) {
	fb.bind();
	FrameBuffer::ClearBuffers(glm::vec3(0, 0, 0));

	lightPassShaderProgram.setMat4("view", lightView);
	lightPassShaderProgram.setMat4("projection", lightProj);

	for (int i = 0; i < modelMats.size(); i++) {
		lightPassShaderProgram.setMat4("model", modelMats[i]);
		lightPassShaderProgram.bind();
		meshRenderers[i]->model.render();
		lightPassShaderProgram.unbind();
	}

	fb.unbind();
}

int main(int argc, char* args[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "sdl gave error" << std::endl;
		return 1;
	}

	// set opengl attributes through sdl
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	uint32_t windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	SDL_Window* window = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	SDL_GL_MakeCurrent(window, context);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// set up ImGUi
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplSDL2_InitForOpenGL(window, context);
	const char* glslVersion = "#version 330";
	ImGui_ImplOpenGL3_Init(glslVersion);

	const std::string vertPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\default.vert";
	const std::string fragPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\default.frag";
	ShaderProgram shaderProgram(vertPath.c_str(), fragPath.c_str());

	shaderProgram.setInt("tex", 0);

	float radius = 12.0f;
	float yPos = 10.0f;
	float angle = 0.0f;
	glm::vec3 lookAt(0.0f, 0.0f, 0.0f);
	glm::mat4 perspProj = glm::perspective(glm::radians(45.0f), ((float)width) / height, 0.1f, 100.0f);
	float xExtent = 5.0f;
	float ratio = (float)height / (float)width;
	glm::mat4 orthoProj = glm::ortho(-xExtent, xExtent, -xExtent * ratio, xExtent * ratio, 0.1f, 100.0f);

	std::string xPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\x.png";
	std::string oPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\o.png";
	Texture xTex(xPath.c_str(), X_TEX_UNIT);
	Texture oTex(oPath.c_str(), O_TEX_UNIT);

	Input input;
	bool running = true;

	uint32_t prev = SDL_GetTicks();

	glm::vec3 pointLightColor(1, 0, 0);
	glm::vec3 pointLightPos(0, 3, 0);
	PointLight pointLight(pointLightColor, pointLightPos, 20.0f, 3.0f);

	glm::vec3 dirLightColor(1, 1, 1);
	glm::vec3 dir(0, -1, 0);
	DirectionalLight directionalLight(dirLightColor, dir, 1.0f);

	glm::vec3 spotLightColor(1, 1, 1);
	glm::vec3 spotLightPos(0, 4, 0.2f);
	glm::vec3 spotLightDir(0, -1, 0);
	SpotLight spotLight(spotLightColor, spotLightPos, spotLightDir, 20.0f, glm::radians(60.0f), glm::radians(45.0f));

	LightFrameBuffer spotLightPassFb;
	LightFrameBuffer dirLightPassFb;

	const std::string lightPassVertPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightPass.vert";
	const std::string lightPassFragPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\shaders\\lightPass.frag";
	ShaderProgram lightPassShaderProgram(lightPassVertPath.c_str(), lightPassFragPath.c_str());

	Model lightPassRenderModel;
	for (int i = 0; i < sizeof(quadVertices) / sizeof(float) / 4; i++) {
		Vertex vertex{};
		vertex.pos = glm::vec3(quadVertices[i * 4 + 0], quadVertices[i * 4 + 1], 0.0f);
		vertex.texCoord = glm::vec2(quadVertices[i * 4 + 2], quadVertices[i * 4 + 3]);
		lightPassRenderModel.vertices.push_back(vertex);
	}
	lightPassRenderModel.indexEnabled = false;
	VBO& lVbo = lightPassRenderModel.vbo;
	VAO& lVao = lightPassRenderModel.vao;

	lVbo.setData((float*)&lightPassRenderModel.vertices[0], lightPassRenderModel.vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);
	lVao.bind();
	lVao.attachVBO(lVbo, 0, 3, sizeof(Vertex), offsetof(Vertex, pos));
	lVao.attachVBO(lVbo, 1, 2, sizeof(Vertex), offsetof(Vertex, texCoord));
	lVao.unbind();

	int pcfLayers = 2;

	int turn = X;

	int TARGET_FPS = 60;
	float secPerFrame = 1.0f / TARGET_FPS;

	GridManager gridManager;

	while (running) {
		curTimeMs = SDL_GetTicks();
		deltaTime = ((curTimeMs - prev) / 1000.0f);
		while (deltaTime < secPerFrame) {
			curTimeMs = SDL_GetTicks();
			deltaTime = ((curTimeMs - prev) / 1000.0f);
		}
		prev = curTimeMs;

		SDL_Event event;
		memset(&input, 0, sizeof(Input));
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) {
				running = false;
			}
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
				running = false;
			}
			if (event.type == SDL_KEYDOWN) {
				SDL_Keycode keyDown = event.key.keysym.sym;
				if (keyDown == SDLK_RETURN) {
					input.enter = true;
				}
				else if (keyDown == SDLK_RIGHT) {
					input.rightArrow = true;
				}
				else if (keyDown == SDLK_LEFT) {
					input.leftArrow = true;
				}
				else if (keyDown == SDLK_UP) {
					input.upArrow = true;
				}
				else if (keyDown == SDLK_DOWN) {
					input.downArrow = true;
				}
				else if (keyDown == SDLK_SLASH) {
					input.slash = true;
				}
				else if (keyDown == SDLK_ESCAPE) {
					running = false;
				}
			}
		}

		gridManager.update(input, turn);

		// if (input.enter) {
			// turn = (turn == X) ? Y : X;
		// }

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		glm::vec3 camPos(radius * cos(glm::radians(angle)), yPos, radius * sin(glm::radians(angle)));
		glm::mat4 camView = glm::lookAt(camPos, lookAt, glm::vec3(0, 1, 0));

		// light pass
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

		std::vector<MeshRenderer*> meshRenderers(NUM_TTT_SQUARES * NUM_TTT_SQUARES, gridManager.grids[0].ticTacToeSquares[0].squareMeshRenderer);
		lightPassFb(spotLightPassFb, spotLight.spotLightView, spotLight.spotLightProj, lightPassShaderProgram,
			meshRenderers, modelMats);
		lightPassFb(dirLightPassFb, directionalLight.dirLightView, directionalLight.dirLightProj, lightPassShaderProgram,
			meshRenderers, modelMats);

		// full color pass
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);

		shaderProgram.setInt("numPointLights", 0);
		shaderProgram.setInt("numDirectionalLights", 0);
		shaderProgram.setInt("numSpotLights", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, spotLightPassFb.depthTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, dirLightPassFb.depthTexture);
		xTex.bind();
		oTex.bind();

		shaderProgram.setMat4("projection", perspProj);
		pointLight.shaderProgram.setMat4("projection", perspProj);
		spotLight.shaderProgram.setMat4("projection", perspProj);

		shaderProgram.setMat4("view", camView);
		pointLight.shaderProgram.setMat4("view", camView);
		spotLight.shaderProgram.setMat4("view", camView);

		pointLight.setPointLightInShader(shaderProgram, 0);
		directionalLight.setDirectionalLightInShader(shaderProgram, 0);
		spotLight.setSpotLightInShader(shaderProgram, 0);

		shaderProgram.setInt("pcfLayers", pcfLayers);

		gridManager.render(shaderProgram, perspProj, camView);

		pointLight.debugRender();
		spotLight.debugRender();

		ImGui::Begin("Selection Transform");
		ImGui::DragInt("pcf", &pcfLayers, 1, 0, 10);
		ImGui::DragFloat3("transform", &gridManager.grids[0].ticTacToeSquares[0].transform.pos.x, 0.1f);
		ImGui::DragFloat("scale", &gridManager.grids[0].ticTacToeSquares[0].transform.scale, 0.1f);
		ImGui::DragFloat3("rotation", &gridManager.grids[0].ticTacToeSquares[0].transform.rot.x, 1, -180.0f, 180.0f);
		ImGui::End();

		ImGui::Begin("Camera");
		ImGui::DragFloat("angle", &angle, 1, -180.0f, 180.0f);
		ImGui::DragFloat("y pos", &yPos, 0.1);
		ImGui::DragFloat("radius", &radius);
		ImGui::DragFloat3("look at", &lookAt.x);
		ImGui::End();

		ImGui::Begin("point Light");
		ImGui::DragFloat3("pos", &pointLight.position.x, 0.1);
		ImGui::DragFloat("max dist", &pointLight.maxDist, 0.1, 1);
		ImGui::DragFloat("multiplier", &pointLight.multiplier, 0.05, 0);
		ImGui::ColorPicker3("color", &pointLight.color.r);
		ImGui::End();

		ImGui::Begin("dir Light");
		ImGui::DragFloat3("dir", &directionalLight.dir.x, 0.1);
		ImGui::DragFloat("multiplier", &directionalLight.multiplier, 0.05, 0);
		ImGui::DragFloat("xEntext", &directionalLight.xEntext, 0.05);
		ImGui::DragFloat("dirYPos", &directionalLight.dirYPos, 0.05, 0);
		ImGui::ColorPicker3("color", &directionalLight.color.r);
		ImGui::End();

		ImGui::Begin("spot Light");
		ImGui::DragFloat3("dir", &spotLight.dir.x, 0.1);
		ImGui::DragFloat3("pos", &spotLight.pos.x, 0.1);
		ImGui::DragFloat("multiplier", &spotLight.multiplier, 0.05, 0);
		ImGui::DragFloat("umbra", &spotLight.umbra, 0.01, 0, glm::radians(90.0f));
		ImGui::DragFloat("prenumbra", &spotLight.prenumbra, 0.01, 0, glm::radians(90.0f));
		ImGui::ColorPicker3("color", &spotLight.color.r);
		ImGui::End();

		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			// handles the multiple windows created by ImGui
			SDL_Window* backupCurrentWindow = SDL_GL_GetCurrentWindow();
			SDL_GLContext backupCurrentContext = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backupCurrentWindow, backupCurrentContext);
		}
		SDL_GL_SwapWindow(window);
	}

	return -1;
}