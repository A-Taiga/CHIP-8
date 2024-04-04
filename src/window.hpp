#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <SDL2/SDL_render.h>
#include <bitset>

#define BUFFER_WIDTH 64
#define BUFFER_HEIGHT 32
#define SCREEN_WIDTH BUFFER_WIDTH * 10
#define SCREEN_HEIGHT BUFFER_HEIGHT * 10
#define PIXEL_SIZE 10

class Window
{
	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		bool running;
	public:
		std::bitset<BUFFER_WIDTH * BUFFER_HEIGHT> buffer;
		Window ();
		~Window ();
		void update ();
		void poll (std::bitset<16>& keys);
		bool is_running ();
		void draw_rect (int x, int y);
};

#endif 