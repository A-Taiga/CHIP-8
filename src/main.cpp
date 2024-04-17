
#include "emulator.hpp"
int main()
{
    CPU chip("roms/programs/Chip8 Picture.ch8");
    chip.run();
    return 0;
}