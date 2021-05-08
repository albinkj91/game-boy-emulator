#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "memory.h"

u_byte memory[MEMORY_SIZE];

u_byte get_memory_value(u_short address){
	return memory[address];
}

void set_memory_value(u_short address, u_byte value){
	memory[address] = value;
}

void push_stack(u_short value, u_short *sp){
	if(*sp < 0xff80){
		printf("ERROR::SP_OUT_OF_BOUNDS\n");
		exit(0);
	}
	memory[*sp--] = value;
	memory[*sp--] = value >> 8;
}

u_short pop_stack(u_short *sp){
	u_short value = memory[++*sp] << 8;
	value |= memory[++*sp];
	return value;
}
