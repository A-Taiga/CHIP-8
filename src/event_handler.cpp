#include "event_handler.hpp"
#include "SDL2/SDL_events.h"
#include "imgui_impl_sdl2.h"

namespace
{
	void key_down_event(const SDL_Event& event, std::bitset<16>& keys)
	{
		switch (event.key.keysym.scancode)
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
	}

	void key_up_event(const SDL_Event& event, std::bitset<16>& keys)
	{
		switch (event.key.keysym.scancode)
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
	}
}

void Event::poll(Window& window, std::bitset<16>& keys)
{
	SDL_Event event;
	while (SDL_PollEvent(&event) > 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT: 
				window.set_running(false);
			case SDL_WINDOWEVENT: 
				if (event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window.get_window()))
					window.set_running(false);
			break;
			case SDL_KEYDOWN: 
				key_down_event(event, keys);
			break;
			case SDL_KEYUP:
				key_up_event(event, keys);
			default: break;
		}
	}
}
