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
void set_flag(_Bool value, u_byte flag_bit_position);
u_byte get_flag(u_byte flag_bit_position);

int main(){
	init_registers();
	reg.b = 0x0f;
	execute_opcode(0x04);
	printf("Register F = %x\n", reg.f);
	
	// TODO: When implementing loop make sure PC is incremented after reading OP code
	// otherwise program will break.

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
			break;
		case 0x02:;
			set_memory_value((reg.b << 8) | reg.c, reg.a);
			reg.pc++;
			break;
		case 0x03:;
			u_short bc = reg.b << 8;
			bc |= reg.c;
			bc++;
			reg.b = (bc & 0xff00) >> 8;
			reg.c = bc & 0x00ff;
			reg.pc++;
			break;
		case 0x04:
			reg.f &= 0x10;
			set_flag((reg.b & 0x0f) == 0x0f, H_FLAG);
			set_flag(!++reg.b, Z_FLAG);
			reg.pc++;
			break;
		case 0x05:
			reg.f &= 0x10;
			set_flag(!reg.b, Z_FLAG);
			set_flag(((reg.b & 0x0f) == 0x00) && reg.b, H_FLAG);
			reg.b--;
			reg.pc++;
			break;
		case 0x06:
			reg.b = get_memory_value(reg.pc++);
			break;
		case 0x07:
			reg.f &= 0x00;
			set_flag(reg.a & 0x80, C_FLAG);
			reg.a = (reg.a << 1) | get_flag(C_FLAG);
			reg.pc++;
			break;
		case 0x08:;
			u_byte lower = get_memory_value(reg.pc++);
			u_byte higher = get_memory_value(reg.pc++);
			u_short address = (higher << 8) | lower;
			set_memory_value(address++, reg.sp & 0x00ff);
			set_memory_value(address, reg.sp >> 8);
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


void set_flag(_Bool value, u_byte flag_bit_position){
	reg.f |= value << flag_bit_position;
}

u_byte get_flag(u_byte flag_bit_position){
	return (reg.f << (7 - flag_bit_position)) >> 7;
}







