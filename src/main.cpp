
#include "emulator.hpp"
#include <cstdlib>
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <chip8_ROM>\n\n", argv[0]);
        std::exit(EXIT_SUCCESS);
    }
    CPU chip(argv[1]);
    chip.run();
    return 0;
}