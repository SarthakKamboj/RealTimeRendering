#include "SDL.h"
#include <iostream>
#include "glad/glad.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "imgui.h"
#include "input.h"

class window_t {
public:
	window_t(int width, int height);
	void swap();
	void poll(Input& input);

private:
	ImGuiIO io;
	int width;
	int height;
	SDL_Window* sdl_win;

	int init_window();
};
