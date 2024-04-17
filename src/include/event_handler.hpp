#ifndef EVENTS_HANDLER_HPP
#define EVENTS_HANDLER_HPP

#include <SDL2/SDL_render.h>
#include <bitset>
#include "window.hpp"

namespace Event
{
	void poll(Window& window, std::bitset<16>& keys);
};

#endif