#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "emulator.h"




int main(int argc, char* argv[])
{

	FILE* file				= fopen(argv[1],"rb");
	int size				= 0;
	int pc					= 0;
	unsigned char* buffer	= NULL;

	fseek(file,0L,SEEK_END);
	size = ftell(file);
	fseek(file,0L,SEEK_SET);

	// programs start at location 0x200 in memory
	buffer = malloc(size + 0x200); 
	fread(buffer+0x200,size,1,file);
	fclose(file);

	pc = 0x200;
	printf("PC     OP\n");
	printf("---------\n");
	while(pc < (size + 0x200))
		// pc += disassembler(buffer, pc);
		pc = emulator(buffer, pc);
	return 0;
}