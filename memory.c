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
