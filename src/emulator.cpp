#include <algorithm>
#include <bitset>
#include <cstdint>
#include <cstdio>
#include "emulator.hpp"
#include <cstring>
#include <random>
#include <stdexcept>
#include <thread>
#include "window.hpp"
#include <iostream>
#include <format>

namespace
{
    using namespace std::chrono_literals;
    std::array<std::uint8_t, SPRITE_TABLE_SIZE> sprites = 
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    void print (std::string&& str)
    {
        std::cout << str;
    }
}

CPU::CPU (std::string&& fileName)
: window()
{
    load_ROM(std::forward<std::string>(fileName));
    V.fill(0);
    stack.fill(0);
}

void CPU::load_ROM (const std::string&& fileName)
{
    FILE* file = std::fopen (fileName.c_str(), "rb");
    if (file == nullptr)                                        throw std::runtime_error (std::strerror(errno));
    if (std::fseek (file, 0L, SEEK_END) == -1)                  throw std::runtime_error (std::strerror(errno));
    if ((size = std::ftell (file) + 0x200) == -1UL)             throw std::runtime_error (std::strerror(errno));
    if (size > 4096)                                            throw std::runtime_error ("ROM > 4096 BYTES");
    if (std::fseek (file, 0L, SEEK_SET) == -1L)                 throw std::runtime_error (std::strerror(errno));
    if (std::fread (RAM.data() + 0x200, size, 1, file) == -1UL) throw std::runtime_error (std::strerror(errno));
    fclose (file);
    std::memcpy(RAM.data(), sprites.data(), SPRITE_TABLE_SIZE);
    _00E0(); // clear screen
}

uint8_t CPU::random_byte()
{
    static std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    return dist(mt);
}

void CPU::timers_thread ()
{
    while (window.is_running())
    {
        if (DT > 0) DT--;
        if (ST > 0) ST--;
        std::this_thread::sleep_for(16ms); // 16ms is about 60hz
    }
}

void CPU::_0NNN()
{
    pc = nnn;
}

void CPU::_00E0()
{
    window.buffer.reset();
}

void CPU::_00EE()
{
    pc = stack[sp];
    sp--;
}

void CPU::_1NNN()
{
    pc = nnn;
}

void CPU::_2NNN()
{
    stack[++sp] = pc;
    pc = nnn;
}

void CPU::_3XNN()
{
    if (V[x] == nn)
        pc += 2;
}

void CPU::_4XNN()
{
    if (V[x] != nn)
        pc += 2;
}

void CPU::_5XY0()
{
    if (V[x] == V[y]) pc += 2;
}

void CPU::_6XNN()
{
    V[x] = nn;
}

void CPU::_7XNN()
{
    V[x] += nn;
}

void CPU::_8XY0()
{
    V[x] = V[y];
}

void CPU::_8XY1()
{
    V[x] |= V[y];
    V[0xF] = 0;
}

void CPU::_8XY2()
{
    V[x] = V[x] & V[y];
    V[0xF] = 0;

}

void CPU::_8XY3()
{
    V[x] ^= V[y];
    V[0xF] = 0;

}

void CPU::_8XY4()
{
    bool cary = (V[x] + V[y]) > 0xFF ? 1 : 0;
    V[x] += V[y];
    V[0xF] = cary;
}

void CPU::_8XY5()
{
    bool carry = V[y] > V[x] ? 0 : 1;
    V[x] -= V[y];
    V[0xF] = carry;
}

void CPU::_8XY6()
{
    bool temp =  V[y] & 0x1;
    V[x] = V[y] >> 1;
    V[0xF] = temp;
}

void CPU::_8XY7()
{
    bool carry = V[x] > V[y] ? 0 : 1;
    V[x] = V[y] - V[x];
    V[0xF] = carry;
}

void CPU::_8XYE()
{
    bool temp = V[y] & 0x80;
    V[x] = V[y] << 1;
    V[0xF] = temp;
}

void CPU::_9XY0()
{
    if (V[x] != V[y]) pc += 2;
}

void CPU::_ANNN()
{
    I = nnn;
}

void CPU::_BNNN()
{
    pc = nnn + V[0];
}

void CPU::_CXNN()
{
    V[x] = random_byte() & nn;
}

void CPU::_DXYN() // draw
{
    std::uint8_t px = V[x] % BUFFER_WIDTH;
    std::uint8_t py = V[y] % BUFFER_HEIGHT;
    V[0xF] = 0;
    for (std::uint8_t dy = 0; dy < n; dy++)
    {
        std::uint8_t ry = py + dy;
        if (ry >= BUFFER_HEIGHT) break;
        std::uint8_t loc = RAM[I+dy];
        for (std::uint8_t dx = 0; dx < 8; dx++)
        {
            std::uint8_t rx = px + dx;
            if (rx >= BUFFER_WIDTH) break;
            bool pixel = (loc << dx) & 0x80;
            std::size_t index = rx + BUFFER_WIDTH * ry;
            V[0xF] = (pixel && window.buffer.test(index)) ? 1 : V[0xF];
            window.buffer.set(index,  window.buffer.test(index) ^ pixel);
        }
    }
}

void CPU::_EX9E()
{
    std::size_t index = V[x];
    if (keys[index]) pc += 2;
}

void CPU::_EXA1()
{
    std::size_t index = V[x];
    if (!keys[index]) pc += 2;
}
void CPU::_FX07()
{
    V[x] = DT;
}

void CPU::_FX0A() // i don't like this idk how to call poll from a different thread T_T
{
    bool flag = true;
    std::uint8_t k;
    while (flag && window.is_running())
    {
        window.poll(keys);
        if (keys.any())
        {
            std::uint8_t key = keys.to_ulong();
            while (key & keys.to_ulong() && window.is_running())
            {
                window.poll(keys);
                flag = false;
                k = static_cast<std::uint8_t>(key);
            }
        }
    }
    V[x] = k;
}

void CPU:: _FX15()
{
    DT = V[x];
}

void CPU::_FX18()
{
    ST = V[x];
}

void CPU::_FX1E()
{
    I += V[x];
}

void CPU::_FX29()
{
    I = V[x] * 5;
}

void CPU::_FX33()
{
    RAM[I]   = (V[x] % 1000) / 100;
    RAM[I+1] = (V[x] % 100) / 10;
    RAM[I+2] = (V[x] % 10);
}

void CPU::_FX55()
{

    for (std::size_t i = 0; i <= x; i++)
        RAM[I+i] = V[i];
    I = I + x + 1;
}

void CPU::_FX65()
{
    for (std::size_t i = 0; i <= x; i++) 
        V[i] = RAM[I + i];
    I = I + x + 1;
}


void CPU::emulator()
{
    opcode = (RAM[pc] << 8) | RAM[pc+1];
    nnn    = opcode & 0x0FFF;
    n      = opcode & 0x000F;
    x      = (opcode >> 8) & 0x000F;
    y      = (opcode >> 4) & 0x000F;
    nn     = opcode & 0x00FF;
    pc += 2;
    window.poll(keys);
    switch (opcode & 0xF000)
    {
        case 0x0000:
        {
            switch (nn)
            {
                case 0x0000: /* _0NNN(); */ break; // SYS addr
                case 0x00E0: _00E0(); break; // CLS;
                case 0x00EE: _00EE(); break; // RET
            }
        } break;
        case 0x1000: _1NNN(); break; // JP to address
        case 0x2000: _2NNN(); break; // Call
        case 0x3000: _3XNN(); break; // SE Vx nn
        case 0x4000: _4XNN(); break; // SNE Vx, byte
        case 0x5000: _5XY0(); break; // SE V[x] == V[y]
        case 0x6000: _6XNN(); break; // LD Vx nn
        case 0x7000: _7XNN(); break; // ADD Vx nn
        case 0x8000:
        {
            switch (n)
            {
                case 0x0000: _8XY0(); break; // LD Vx Vy
                case 0x0001: _8XY1(); break; // OR Vx |= Vy
                case 0x0002: _8XY2(); break; // AND Vx &= Vy
                case 0x0003: _8XY3(); break; // XOR Vx ^= Vy
                case 0x0004: _8XY4(); break; // ADD Vx = Vy, V[0xF] = 1
                case 0x0005: _8XY5(); break; // SUB Vx -= Vy V[0xF] = 1
                case 0x0006: _8XY6(); break; // V[0xF] = V[x] & 0x1, V[x] >>= 1 (LSB)
                case 0x0007: _8XY7(); break; // SUBN Vx = Vy - Vx V[0xF] = 1
                case 0x000E: _8XYE(); break; // V[0xF] = V[x] & 0x80, V[x] <<= 1 (MSB)
                default: break;
            }
        } break;
        case 0x9000: _9XY0(); break; // SNE Vx != Vy
        case 0xA000: _ANNN(); break; // LD I = addr
        case 0xB000: _BNNN(); break; // JP nnn + V[0]
        case 0xC000: _CXNN(); break; // RND V[x] = random byte & nn
        case 0xD000: _DXYN(); break; // DRW
        case 0xE000:
        {
            switch (nn)
            {
                case 0x009E: _EX9E(); break; // SKNE V[x] == key pressed
                case 0x00A1: _EXA1(); break; // SKNE V[x] != key pressed
                default: break;
            }
        } break;
        case 0xF000:
        {
            switch (nn)
            {
                case 0x0007: _FX07(); break; // V[x] = DT
                case 0x000A: _FX0A(); break; // V[x] = key pressed
                case 0x0015: _FX15(); break; // DT = V[x]
                case 0x0018: _FX18(); break; // ST = V[x]
                case 0x001E: _FX1E(); break; // I += V[x]
                case 0x0029: _FX29(); break; // I = V[x] (sprite address)
                case 0x0033: _FX33(); break; // BCD
                case 0x0055: _FX55(); break; // LD [I], Vx
                case 0x0065: _FX65(); break; // LD Vx, [I]
                default: break;
            }
        } break;
        default: break;
    }
}

/*
nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
n or nibble - A 4-bit value, the lowest 4 bits of the instruction
x - A 4-bit value, the lower 4 bits of the high byte of the instruction
y - A 4-bit value, the upper 4 bits of the low byte of the instruction
nn or byte - An 8-bit value, the lowest 8 bits of the instruction
*/

void CPU::disassembler()
{
	opcode = (RAM[pc] << 8) | RAM[pc+1];
	nnn = opcode & 0xFFF;
	n	= opcode & 0x00F;
	x	= (opcode >> 8) & 0x00F;
	y	= (opcode >> 4) & 0x00F;
	nn	= opcode & 0x0FF;
    print (std::format("[0x{:4<X}] 0x{:<X} ", pc, opcode));
    switch (opcode & 0xF000)
    {
        case 0x0000:
        {
            switch (nn)
            {
                case 0x0000: print ("CALL"); break;
                case 0x00E0: print ("CLS"); break;
                case 0x00EE: print ("RET"); break;
                default: print ("UNKNOWN 0x0000"); break;
            }
        } break;
        case 0x1000: print ("JP addr"); break;
        case 0x2000: print ("CALL addr"); break;
        case 0x3000: print ("SE Vx, byte"); break;
        case 0x4000: print ("SNE Vx, byte"); break;
        case 0x5000: print ("SE Vx, Vy"); break;
        case 0x6000: print ("LD Vx, byte"); break;
        case 0x7000: print ("ADD Vx, byte"); break;
        case 0x8000:
        {
            switch (n)
            {
                case 0x0000: print ("LD Vx, Vy");break;
                case 0x0001: print ("OR Vx, Vy"); break;
                case 0x0002: print ("AND Vx, Vy"); break;
                case 0x0003: print ("XOR Vx, Vy"); break;
                case 0x0004: print ("ADD Vx, Vy"); break;
                case 0x0005: print ("SUB Vx, Vy"); break;
                case 0x0006: print ("SHR Vx, {, Vy}"); break;
                case 0x0007: print ("SUBN Vx, Vy"); break;
                case 0x000E: print ("SHL Vx {, Vy}"); break;
                default: print ("UNKNOWN 0x8000"); break;
            }
        } break;
        case 0x9000: print ("SNE Vx, Vy"); break;
        case 0xA000: print ("LD I, addr"); break;
        case 0xB000: print ("JP V0, addr"); break;
        case 0xC000: print ("RND Vx, byte"); break;
        case 0xD000: print ("DRW Vx, Vy, nibble"); break;
        case 0xE000:
        {
            switch (nn)
            {
                case 0x009E: print ("SKP Vx"); break;
                case 0x00A1: print ("SKNP Vx"); break;
                default: print ("UNKNOWN 0xE000");
            }
        } break;
        case 0xF000:
        {
            switch (nn)
            {
                case 0x0007: print ("LD Vx, DT"); break;
                case 0x000A: print ("LD Vx, k"); break;
                case 0x0015: print ("LD DT, Vx"); break;
                case 0x0018: print ("LD ST, Vx"); break;
                case 0x001E: print ("ADD I, Vx"); break;
                case 0x0029: print ("LD F, Vx"); break;
                case 0x0033: print ("LD B, Vx"); break;
                case 0x0055: print ("LD [I], Vx"); break;
                case 0x0065: print ("LD Vx, [I]"); break;
                default: print ("UNKNOWN 0xF000"); break;
            }
        } break;
        default: break;
    }
    print ("\n");
    pc += 2;
}
void CPU::run()
{
    timer = std::thread(&CPU::timers_thread, this);
    pc = 0x200;
    while (pc < size && window.is_running())
    {
        emulator();
        std::this_thread::sleep_for(1.5ms);
        for (std::size_t i = 0; i < window.buffer.size(); i++)
        {
            if (window.buffer.test(i))
                window.draw_rect(i % BUFFER_WIDTH, i / BUFFER_WIDTH);
        }
        window.update();
    }
    timer.join();
}


