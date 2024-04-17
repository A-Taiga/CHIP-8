#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <SDL2/SDL_render.h>
#include <bitset>

#define BUFFER_WIDTH 64
#define BUFFER_HEIGHT 32
#define SCREEN_WIDTH BUFFER_WIDTH * 13
#define SCREEN_HEIGHT BUFFER_HEIGHT * 13 + 100
#define PIXEL_SIZE 13

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
        bool& get_running ();
        void set_running (bool val);
        void draw_rect (int x, int y);
        SDL_Window* get_window();
        SDL_Renderer* get_renderer();
};

#endif 