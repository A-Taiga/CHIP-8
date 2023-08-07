#pragma once
#include <stdint.h>

typedef struct CPU
{
	uint8_t	V[16];
	uint16_t I;
	uint8_t delayTimer;
	uint8_t soundTimer;
	uint8_t VF;

	uint16_t pc; // program counter
	uint8_t sp; // stack pointer

} CPU;
static CPU state = {};
int disassembler(unsigned char* buffer, int pc);
int emulator(unsigned char* buffer, int pc);