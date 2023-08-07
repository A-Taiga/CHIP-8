#include "emulator.h"
#include <stdio.h>

/*
nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
n or nibble - A 4-bit value, the lowest 4 bits of the instruction
x - A 4-bit value, the lower 4 bits of the high byte of the instruction
y - A 4-bit value, the upper 4 bits of the low byte of the instruction
kk or byte - An 8-bit value, the lowest 8 bits of the instruction
*/

int disassembler(unsigned char* buffer, int pc)
{
	uint16_t opcode;
	uint16_t nnn;
	uint8_t n;
	uint8_t x;
	uint8_t y;
	uint8_t kk;

	/* CHIP-8 has 35 opcodes, which are all two bytes long and stored big-endian. */
	opcode = (buffer[pc] << 8) | buffer[pc+1]; 
	nnn = opcode & 0xFFF;
	n	= opcode & 0x00F;
	x	= (opcode >> 8) & 0x00F;
	y	= (opcode >> 4) & 0x00F;
	kk	= opcode & 0x0FF;

	#ifdef DEBUG
	printf("PC: 0x%04x Op: 0x%04x, OP2: 0x%04x nnn: 0x%04x, n: 0x%04x, x: 0x%04x, y: 0x%04x, kk: 0x%04x", pc, opcode, opcode & 0xF000, nnn,n,x,y,kk);
	#endif
	printf("0x%04x ",pc);
	switch(opcode & 0xF000)
	{
		case 0x0000:
		{
			switch(kk)
			{
				case 0x0000: printf("SYS 	0x%04x",nnn);	break;
				case 0x00E0: printf("CLS");					break;
				case 0x00EE: printf("RET");					break;
				default: printf("UNKNOWN 0x0");				break;
			}
		} break;
		case 0x1000: printf("JP  	0x%04x",nnn);	break;
		case 0x2000: printf("CALL	0x%04x",nnn);	break;
		case 0x3000: printf("SE  	Vx,0x%04x",kk);	break;
		case 0x4000: printf("SNE 	Vx,0x%04x",kk);	break;
		case 0x5000: printf("SE  	Vx,Vy");		break;
		case 0x6000: printf("LD  	Vx,0x%04x",kk);	break;
		case 0x7000: printf("ADD 	Vx,0x%04x",kk);	break;
		case 0x8000:
		{
			switch(n)
			{
				case 0x0000: printf("LD  	Vx,Vy");		break;
				case 0x0001: printf("OR  	Vx,Vy");		break;
				case 0x0002: printf("AND 	Vx,Vy");		break;
				case 0x0003: printf("XOR 	Vx,Vy");		break;
				case 0x0004: printf("ADD 	Vx,Vy");		break;
				case 0x0005: printf("SUB 	Vx,Vy");		break;
				case 0x0006: printf("SHR 	Vx,{, Vy}");	break;
				case 0x0007: printf("SUBN	Vx,Vy");		break;
				case 0x000E: printf("SHL 	Vx,{, Vy}");	break;
				default: printf("UNKNOWN 0x8");				break;
			}
		} break;
		case 0x9000: printf("SNE 	Vx,Vy");			break;
		case 0xA000: printf("LD  	I,0x%04x",nnn);		break;
		case 0xB000: printf("JP  	V0,0x%04x",nnn);	break;
		case 0xC000: printf("RND 	Vx,0x%04x",nnn);	break;
		case 0xD000: printf("DRW	Vx,Vy,0x%04x",n);	break;
		case 0xE000:
		{
			switch(kk)
			{
				case 0x009E: printf("SKIP	Vx");	break;
				case 0x00A1: printf("SKNP	Vx");	break;
				default: printf("UNKNOWN 0xE");		break;
			}
		} break;
		case 0xF000:
		{
			switch(kk)
			{
				case 0x0007: printf("LD  	Vx,DT");	break;
				case 0x000A: printf("LD  	Vx,K");		break;
				case 0x0015: printf("LD  	Dt,Vx");	break;
				case 0x0018: printf("LD  	ST,Vx");	break;
				case 0x001E: printf("ADD 	I,Vx");		break;
				case 0x0029: printf("LD  	F,Vx");		break;
				case 0x0033: printf("LD  	F,Vx");		break;
				case 0x0055: printf("LD  	[I],Vx");	break;
				case 0x0065: printf("LD  	Vx,[I]");	break;
				default: printf("UNKNOWN 0xF");			break;
			}
		} break;
		default: printf("UNKNOWN");break;
	}
	puts("");
	return 2;
}


int emulator(unsigned char* buffer, int pc)
{
	uint16_t opcode;
	uint16_t nnn;
	uint8_t n;
	uint8_t x;
	uint8_t y;
	uint8_t kk;

	opcode	= (buffer[pc] << 8) | buffer[pc+1];
	nnn		= opcode & 0x0FFF;
	n		= opcode & 0x000F;
	x		= (opcode >> 8) & 0x000F;
	y		= (opcode >> 4) & 0x000F;
	kk		= opcode & 0x00FF;

	printf("0x%04x ",pc);
	switch(opcode & 0xF000)
	{
		case 0x0000:
		{
			switch(kk)
			{
				case 0x0000: printf("SYS 	0x%04x",nnn);	break;
				case 0x00E0: printf("CLS");					break;
				case 0x00EE: 
					printf("RET");
					state.sp++;
					break;
				default: printf("UNKNOWN 0x0");				break;
			}
		} break;
		case 0x1000: 
			printf("JP  	0x%04x",nnn);
			pc = nnn;
			break;
		case 0x2000: 
			printf("CALL	0x%04x",nnn);
			state.sp++;
			state.V[state.sp] = pc+2;
			pc = nnn;
			break;
		case 0x3000: 
			printf("SE  	Vx,0x%04x",kk);
			if(state.V[x] == kk)
				pc += 2;
			break;
		case 0x4000: 
			printf("SNE 	Vx,0x%04x",kk);
			if(state.V[x] != state.V[y])
				pc += 2;
			break;
		case 0x5000: 
			printf("SE  	0x%04x,0x%04x",state.V[x],state.V[y]);
			if(state.V[x] == state.V[y])
				pc += 2;
			break;
		case 0x6000: 
			printf("LD  	0x%04x,0x%04x",state.V[x],kk);
			state.V[x] = kk;
			break;
		case 0x7000: 
			printf("ADD 	0x%04x,0x%04x",state.V[x],kk);
			state.V[x] += kk;
			break;
		case 0x8000:
		{
			switch(n)
			{
				case 0x0000: 
					printf("LD  	Vx,Vy");
					state.V[x] = state.V[y];
					break;
				case 0x0001: 
					printf("OR  	Vx,Vy");
					state.V[x] = state.V[x] | state.V[y];
					break;
				case 
					0x0002: printf("AND 	Vx,Vy");
					state.V[x] = state.V[x] & state.V[y];
					break;
				case 
					0x0003: printf("XOR 	Vx,Vy");
					state.V[x] = state.V[x] ^ state.V[y];
					break;
				case 0x0004: 
					printf("ADD 	Vx,Vy");
					uint16_t result = state.V[x] + state.V[y];
					if((result & 0x00FF) != 0)
					{
						state.VF = 1;
						state.V[x] = 255;
					}
					else 
					{
						state.VF = 0;
						state.V[x] = result;
					}
					break;
				case 0x0005: 
					printf("SUB 	Vx,Vy"); //// MAYBE LOOK
					state.VF = (state.V[x] > state.V[y]);
					state.V[x] = state.V[x] - state.V[y];
					break;
				case 0x0006: 
					printf("SHR 	Vx,{, Vy}"); //// MAYBE LOOK 
					state.VF = (state.V[x] & 1);
					state.V[x] >>= 1;
					break;
				case 0x0007: 
					printf("SUBN	0x%04x,0x%04x",state.V[x],state.V[y]); ////// MAYBE LOOK
					state.VF = (state.V[y] > state.V[x]);
					state.V[x] = state.V[y] - state.V[x];
					break;
				case 0x000E: 
					printf("SHL 	0x%04x,{, Vy}",state.V[x]); ///// LOOK MAYBE
					if((state.V[x] & 0x80 )!= 0)
						state.VF = 1;
					else
						state.VF = 0;
					state.V[x] *= 2;
					break;
				default: printf("UNKNOWN 0x8");				break;
			}
		} break;
		case 0x9000: printf("SNE 	Vx,Vy");			break;
		case 0xA000: printf("LD  	I,0x%04x",nnn);		break;
		case 0xB000: printf("JP  	V0,0x%04x",nnn);	break;
		case 0xC000: printf("RND 	Vx,0x%04x",nnn);	break;
		case 0xD000: printf("DRW	Vx,Vy,0x%04x",n);	break;
		case 0xE000:
		{
			switch(kk)
			{
				case 0x009E: printf("SKIP	Vx");	break;
				case 0x00A1: printf("SKNP	Vx");	break;
				default: printf("UNKNOWN 0xE");		break;
			}
		} break;
		case 0xF000:
		{
			switch(kk)
			{
				case 0x0007: printf("LD  	Vx,DT");	break;
				case 0x000A: printf("LD  	Vx,K");		break;
				case 0x0015: printf("LD  	Dt,Vx");	break;
				case 0x0018: printf("LD  	ST,Vx");	break;
				case 0x001E: printf("ADD 	I,Vx");		break;
				case 0x0029: printf("LD  	F,Vx");		break;
				case 0x0033: printf("LD  	F,Vx");		break;
				case 0x0055: printf("LD  	[I],Vx");	break;
				case 0x0065: printf("LD  	Vx,[I]");	break;
				default: printf("UNKNOWN 0xF");			break;
			}
		} break;
		default: printf("UNKNOWN");break;
	}
	puts("");

	return pc += 2;

}
