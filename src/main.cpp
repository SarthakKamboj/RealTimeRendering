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
#include "renderer/vao.h"
#include "glm/gtc/type_ptr.hpp"
#include <renderer/ebo.h>

int width = 800;
int height = 600;

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

	std::string modelPath = "C:\\Sarthak\\programming\\VulkanGraphicsEngine\\assets\\viking_room.obj";
	// std::string modelPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\assets\\cube.obj";
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indicies;
	loadModel(modelPath, vertices, indicies);

	std::string texPath = "C:\\Sarthak\\programming\\VulkanGraphicsEngine\\assets\\viking_room.png";
	Texture texture(texPath.c_str(), 0);

	const std::string vertPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\default.vert";
	const std::string fragPath = "C:\\Sarthak\\programming\\RealTimeRendering\\src\\default.frag";
	ShaderProgram shaderProgram(vertPath.c_str(), fragPath.c_str());

	texture.bind();
	shaderProgram.setInt("tex", 0);

	float radius = 10.0f;
	float yPos = 5.0f;
	float angle = 0.0f;
	glm::vec3 pos(radius * cos(glm::radians(angle)), yPos, radius * sin(glm::radians(angle)));
	glm::mat4 view = glm::lookAt(pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	shaderProgram.setMat4("view", view);
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), ((float)width) / height, 0.1f, 100.0f);
	shaderProgram.setMat4("projection", proj);

	VAO vao;
	VBO vbo;

#if USE_EBO == 1

	EBO ebo;
	ebo.setData((unsigned int*)&indicies[0], indicies.size() * sizeof(indicies[0]), GL_STATIC_DRAW);
#endif
	vbo.setData((float*)&vertices[0], vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

	vao.bind();
#if USE_EBO == 1
	ebo.bind();
#endif
	vao.attachVBO(vbo, 0, 3, sizeof(Vertex), offsetof(Vertex, pos));
	vao.attachVBO(vbo, 1, 3, sizeof(Vertex), offsetof(Vertex, color));
	vao.attachVBO(vbo, 2, 2, sizeof(Vertex), offsetof(Vertex, texCoord));

	vao.unbind();
	vbo.unbind();
#if USE_EBO == 1
	ebo.unbind();
#endif

	Input input;
	bool running = true;

	float anglePerSec = 90.0f;
	float runningAngle = 0.0f;

	glm::vec3 rotArr[3] = { glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1) };

	int rotIdx = 0;

	uint32_t prev = SDL_GetTicks();

	glm::vec3 rot{};
	rot.x = -90.0f;

	while (running) {
		uint32_t cur = SDL_GetTicks();
		float timeElapsed = ((cur - prev) / 1000.0f);
		prev = cur;

		runningAngle += (timeElapsed * anglePerSec);

		if (runningAngle >= 360.0f) {
			runningAngle = 0.0f;
			rotIdx = (rotIdx + 1) % 3;
		}

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			input.enterPressed = false;
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT) {
				running = false;
			}
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
				running = false;
			}
			if (event.type == SDL_KEYDOWN) {
				SDL_Keycode keyDown = event.key.keysym.sym;
				input.enterPressed = (keyDown == SDLK_RETURN);
			}
		}

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, width, height);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		rot.z = runningAngle;

		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(3.0f));
		model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
		shaderProgram.setMat4("model", model);

		shaderProgram.bind();
		vao.bind();
#if USE_EBO == 1
		glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
#else
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
#endif
		vao.unbind();
		shaderProgram.unbind();

		ImGui::DragFloat3("rotation", &rot.x, 1, -180.0f, 180.0f);

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