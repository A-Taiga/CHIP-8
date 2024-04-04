
#include "emulator.hpp"
#include <cstdlib>
int main(int argc, char* argv[])
{
    // if (argc < 2)
    // {
    //     printf("Usage: %s <chip8_ROM>\n\n", argv[0]);
    //     std::exit(EXIT_SUCCESS);
    // }


    // CPU chip (argv[1]);
    // CPU chip("roms/roms2/1-chip8-logo.ch8");
    // CPU chip("roms/roms2/2-ibm-logo.ch8");
    // CPU chip("roms/roms2/pong.ch8");
    // CPU chip("roms/roms2/3-corax+.ch8");
    // CPU chip("roms/roms2/rng.ch8");
    CPU chip("roms/roms2/6-keypad.ch8");
    // CPU chip("roms/roms2/5-quirks.ch8");
    // CPU chip("roms/roms2/Chip8 Picture.ch8");
    // CPU chip("roms/programs/Life [GV Samways, 1980].ch8");
    // CPU chip("roms/roms2/4-flags.ch8");
    // CPU chip("roms/roms2/8-scrolling.ch8");

    // CPU chip(argv[1]);
    chip.run();
    return 0;
}