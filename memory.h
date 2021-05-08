#include "global.h"

#define MEMORY_SIZE 0x10000
#define STACK_START_ADDRESS 0xfffe

u_byte get_memory_value(u_short address);
void set_memory_value(u_short address, u_byte value);

void push_stack(u_short value, u_short *sp);
u_short pop_stack(u_short *sp);
