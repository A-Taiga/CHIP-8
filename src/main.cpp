
#include "emulator.hpp"
int main(int argc, char* argv[])
{
    // CPU chip (argv[1]);
    // CPU chip("roms/roms2/1-chip8-logo.ch8");
    // CPU chip("roms/roms2/2-ibm-logo.ch8");
    // CPU chip("roms/roms2/pong.ch8");
    // CPU chip("roms/roms2/3-corax+.ch8");
    // CPU chip("roms/roms2/rng.ch8");
    // CPU chip("roms/roms2/6-keypad.ch8");
    // CPU chip("roms/roms2/5-quirks.ch8");
    // CPU chip("roms/roms2/Chip8 Picture.ch8");
    // CPU chip("roms/programs/Life [GV Samways, 1980].ch8");
    CPU chip("roms/roms2/4-flags.ch8");
    // CPU chip("roms/roms2/8-scrolling.ch8");


    chip.run();
    return 0;
}