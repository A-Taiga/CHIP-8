#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <SDL2/SDL_render.h>
#include <bitset>

#define CHIP_W 64
#define CHIP_H 32
#define SCREEN_WIDTH CHIP_W * 10
#define SCREEN_HEIGHT CHIP_H * 10
#define PIXEL_SIZE 10

class Window
{
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		bool running;
	public:
		Window();
		~Window();
		void update();
		void poll(std::bitset<16>& keys);
		bool is_running();
		void draw_rect(int x, int y);
};

#endif 