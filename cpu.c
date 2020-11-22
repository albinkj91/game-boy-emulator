#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "cpu.h"
#include "memory.h"

// Variable definitions
typedef struct Registers{
	u_byte a;
	u_byte f;
	u_byte b;
	u_byte c;
	u_byte d;
	u_byte e;
	u_byte h;
	u_byte l;

	u_short sp;
	u_short pc;
}Register;

Register reg;


// Function definitions
void init_registers();
void execute_opcode(u_byte opcode);

int main(){
	init_registers();
	execute_opcode(0x02);
	return 1;
}

void init_registers(){
	reg.a = 0x0;
	reg.f = 0x0;
	reg.b = 0x0;
	reg.c = 0x0;
	reg.d = 0x0;
	reg.e = 0x0;
	reg.h = 0x0;
	reg.l = 0x0;
	reg.sp = 0x0;
	reg.pc = PC_START;
}

void execute_opcode(u_byte opcode){
	switch(opcode){
		case 0x01:
			reg.c = get_memory_value(reg.pc++);
			reg.b = get_memory_value(reg.pc++);
			reg.pc++;
			break;
		case 0x02:;
			set_memory_value((reg.b << 8) | reg.c, reg.a);
			reg.pc++;
			break;
		case 0x4f:
			reg.c = reg.a;
			reg.pc++;
			break;
		default:
			printf("Default case\n");
			reg.pc++;
			break;
	}
}
