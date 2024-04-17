#include "window.hpp"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include <SDL2/SDL.h>
#include <stdexcept>

Window::Window ()
: running(true)
{
	if (SDL_INIT_EVERYTHING < 0)
		throw std::runtime_error(SDL_GetError());

	window = SDL_CreateWindow("CHIP-8"
							, SDL_WINDOWPOS_CENTERED
							, SDL_WINDOWPOS_CENTERED
							, SCREEN_WIDTH
							, SCREEN_HEIGHT
							, SDL_WINDOW_ALLOW_HIGHDPI);
	if (window == nullptr)
		throw std::runtime_error(SDL_GetError());

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
		throw std::runtime_error(SDL_GetError());
}

Window::~Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Window::draw_rect(int x, int y)
{
	SDL_Rect r{x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect (renderer, &r);
}

bool& Window::get_running()
{
	return running;
}

void Window::set_running (bool val)
{
	running = val;
}

SDL_Window* Window::get_window()
{
	return window;
}

SDL_Renderer* Window::get_renderer()
{
	return renderer;
}
