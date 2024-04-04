#include "window.hpp"
#include "SDL2/SDL_error.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_scancode.h"
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
							, SDL_WINDOW_SHOWN);
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

void Window::update()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}

void Window::poll(std::bitset<16>& keys)
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev) > 0)
	{
		switch (ev.type)
		{
			case SDL_QUIT: running = false;
			case SDL_KEYDOWN:
				switch(ev.key.keysym.scancode)
				{
					case SDL_SCANCODE_0: keys[0] = true; break;
					case SDL_SCANCODE_1: keys[1] = true; break;
					case SDL_SCANCODE_2: keys[2] = true; break;
					case SDL_SCANCODE_3: keys[3] = true; break;
					case SDL_SCANCODE_4: keys[4] = true; break;
					case SDL_SCANCODE_5: keys[5] = true; break;
					case SDL_SCANCODE_6: keys[6] = true; break;
					case SDL_SCANCODE_7: keys[7] = true; break;
					case SDL_SCANCODE_8: keys[8] = true; break;
					case SDL_SCANCODE_9: keys[9] = true; break;
					case SDL_SCANCODE_A: keys[10] = true; break;
					case SDL_SCANCODE_B: keys[11] = true; break;
					case SDL_SCANCODE_C: keys[12] = true; break;
					case SDL_SCANCODE_D: keys[13] = true; break;
					case SDL_SCANCODE_E: keys[14] = true; break;
					case SDL_SCANCODE_F: keys[15] = true; break;
					default: break;
				}
			break;
			case SDL_KEYUP:
				switch (ev.key.keysym.scancode)
				{
					case SDL_SCANCODE_0: keys[0] = false; break;
					case SDL_SCANCODE_1: keys[1] = false; break;
					case SDL_SCANCODE_2: keys[2] = false; break;
					case SDL_SCANCODE_3: keys[3] = false; break;
					case SDL_SCANCODE_4: keys[4] = false; break;
					case SDL_SCANCODE_5: keys[5] = false; break;
					case SDL_SCANCODE_6: keys[6] = false; break;
					case SDL_SCANCODE_7: keys[7] = false; break;
					case SDL_SCANCODE_8: keys[8] = false; break;
					case SDL_SCANCODE_9: keys[9] = false; break;
					case SDL_SCANCODE_A: keys[10] = false; break;
					case SDL_SCANCODE_B: keys[11] = false; break;
					case SDL_SCANCODE_C: keys[12] = false; break;
					case SDL_SCANCODE_D: keys[13] = false; break;
					case SDL_SCANCODE_E: keys[14] = false; break;
					case SDL_SCANCODE_F: keys[15] = false; break;
					default: break;
				}
			break;
		}
	}
}

void Window::draw_rect(int x, int y)
{
	SDL_Rect r{x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect (renderer, &r);
}

bool Window::is_running()
{
	return running;
}

