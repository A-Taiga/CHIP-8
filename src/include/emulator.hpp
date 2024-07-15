#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <cstdint>
#include <string>
#include "window.hpp"
#include <bitset>
#include <thread>

#define MAIN_MEM_SIZE 4096
#define KEY_SIZE 16
#define SPRITE_TABLE_SIZE 80

class CPU
{
    private:
        std::array      <std::uint8_t, MAIN_MEM_SIZE> RAM; // main memory
        std::array      <std::uint8_t, 16> V; // registers
        std::bitset     <BUFFER_WIDTH * BUFFER_HEIGHT> buffer;
        std::array      <std::uint16_t, 16> stack; // stack 
        std::bitset     <KEY_SIZE> keys;
        std::uint16_t   pc; // program counter
        std::uint16_t   I;  // sotres mem addresses
        std::uint8_t    DT;  // delay timer
        std::uint8_t    ST;  // sound timer
        std::uint8_t    sp;  // stack pointer
        std::uint16_t   opcode;
        std::uint16_t   nnn;  // addr 12-bit value of the lowest 12 bits of the instruction
        std::uint8_t    n;     // nibble lowest 4 bits of instruction
        std::uint8_t    x;     // lower 4-bits of the higher byte
        std::uint8_t    y;     // upper 4-bits of the lower byte
        std::uint8_t    nn;    // byte lowest 8 bits of the instruction

        void _0NNN (); void _00E0 (); void _00EE (); void _1NNN ();
        void _2NNN (); void _3XNN (); void _4XNN (); void _5XY0 ();
        void _6XNN (); void _7XNN (); void _8XY0 (); void _8XY1 ();
        void _8XY2 (); void _8XY3 (); void _8XY4 (); void _8XY5 ();
        void _8XY6 (); void _8XY7 (); void _8XYE (); void _9XY0 ();
        void _ANNN (); void _BNNN (); void _CXNN (); void _DXYN ();
        void _EX9E (); void _EXA1 (); void _FX07 (); void _FX0A ();
        void _FX15 (); void _FX18 (); void _FX1E (); void _FX29 ();
        void _FX33 (); void _FX55 (); void _FX65 ();

        Window          window;
        bool            pause;
        bool            reset;
        std::uint16_t   pause_counter;
        std::thread     timer_thread;
        std::thread     render_thread;
        std::size_t     size;
        
        std::uint8_t random_byte( );
        void load_ROM (const std::string&& fileName); 
        void timers_handler ();
        void render_handler ();
        void emulator ();

    public:
        CPU();
        CPU(std::string&& fileName);
        CPU(const std::string&& fileName);
        void disassembler();
        void run();
};


#endif