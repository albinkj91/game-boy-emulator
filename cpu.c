#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "cpu.h"
#include "memory.h"

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


void init_registers();
void execute_opcode(u_byte opcode);
void set_flag(_Bool value, u_byte flag_mask);
u_byte get_flag(u_byte flag_mask);
void add_x9(u_byte higher, u_byte lower);
void ld_8_immediate(u_byte *reg_pointer);
void ld_xa_16(u_byte higher, u_byte lower);
void dec_16(u_byte *higher, u_byte *lower);
void inc_8(u_byte *reg_pointer);
void inc_16(u_byte *higher, u_byte *lower);
void dec_8(u_byte *reg_pointer);
void jr_conditional(_Bool flag, s_byte steps);
void daa();


int main(){
	//TODO: SUB and then test DAA instruction
	init_registers();
	execute_opcode(0x34);
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
	u_byte lower;
	u_byte higher;
	u_short address;
	u_short hl;
	u_byte mem_value;
	u_byte flag;
	s_byte steps;

	switch(opcode){
		case 0x01:
			reg.c = get_memory_value(reg.pc++);
			reg.b = get_memory_value(reg.pc);
			break;
		case 0x02:
			address = (reg.b << 8) | reg.c;
			set_memory_value(address, reg.a);
			break;
		case 0x03:
			inc_16(&reg.b, &reg.c);
			break;
		case 0x04:
			inc_8(&reg.b);
			break;
		case 0x05:
			dec_8(&reg.b);
			break;
		case 0x06:
			ld_8_immediate(&reg.b);
			break;
		case 0x07:
			reg.f &= 0x00;
			set_flag(reg.a & 0x80, C_FLAG);
			reg.a = (reg.a << 1) | get_flag(C_FLAG);
			break;
		case 0x08:;
			lower = get_memory_value(reg.pc++);
			higher = get_memory_value(reg.pc);
			address = (higher << 8) | lower;
			set_memory_value(address++, reg.sp & 0x00ff);
			set_memory_value(address, reg.sp >> 8);
			break;
		case 0x09:
			add_x9(reg.b, reg.c);
			break;
		case 0x0a:
			ld_xa_16(reg.b, reg.c);
			break;
		case 0x0b:
			dec_16(&reg.b, &reg.c);
			break;
		case 0x0c:
			inc_8(&reg.c);
			break;
		case 0x0d:
			dec_8(&reg.c);
			break;
		case 0x0e:
			ld_8_immediate(&reg.c);
			break;
		case 0x10:
			//TODO: STOP
			break;
		case 0x11:
			reg.e = get_memory_value(reg.pc++);
			reg.d = get_memory_value(reg.pc);
			break;
		case 0x12:
			address = (reg.d << 8) | reg.e;
			set_memory_value(address, reg.a);
			break;
		case 0x13:
			inc_16(&reg.d, &reg.e);
			break;
		case 0x14:
			inc_8(&reg.d);
			break;
		case 0x15:
			dec_8(&reg.d);
			break;
		case 0x16:
			ld_8_immediate(&reg.d);
			break;
		case 0x17:
			reg.f &= 0x00;
			set_flag(reg.a & 0x80, C_FLAG);
			reg.a = (reg.a << 1) | get_flag(C_FLAG);
			break;
		case 0x18:
			steps = get_memory_value(reg.pc + 1);
			reg.pc += steps - 1;
			break;
		case 0x19:
			add_x9(reg.d, reg.e);
			break;
		case 0x1a:
			ld_xa_16(reg.d, reg.e);
			break;
		case 0x1b:
			dec_16(&reg.d, &reg.e);
			break;
		case 0x1c:
			inc_8(&reg.e);
			break;
		case 0x1d:
			dec_8(&reg.e);
			break;
		case 0x1e:
			ld_8_immediate(&reg.e);
			break;
		case 0x20:
			flag = !get_flag(Z_FLAG);
			steps = get_memory_value(reg.pc + 1);
			jr_conditional(flag, steps);
			break;
		case 0x21:
			reg.l = get_memory_value(reg.pc++);
			reg.h = get_memory_value(reg.pc);
			break;
		case 0x22:
			address = (reg.h << 8) | reg.l;
			set_memory_value(address, reg.a);
			inc_16(&reg.h, &reg.l);
			break;
		case 0x23:
			inc_16(&reg.h, &reg.l);
			break;
		case 0x24:
			inc_8(&reg.h);
			break;
		case 0x25:
			dec_8(&reg.h);
			break;
		case 0x26:
			ld_8_immediate(&reg.h);
			break;
		case 0x28:
			flag = get_flag(Z_FLAG);
			steps = get_memory_value(reg.pc + 1);
			jr_conditional(flag, steps);
			break;
		case 0x29:
			add_x9(reg.h, reg.l);
			break;
		case 0x2a:
			ld_xa_16(reg.h, reg.l);
			inc_16(&reg.h, &reg.l);
			break;
		case 0x2b:
			dec_16(&reg.h, &reg.l);
			break;
		case 0x2c:
			inc_8(&reg.l);
			break;
		case 0x2d:
			dec_8(&reg.l);
			break;
		case 0x2e:
			ld_8_immediate(&reg.l);
			break;
		case 0x30:
			flag = !get_flag(C_FLAG);
			steps = get_memory_value(reg.pc + 1);
			jr_conditional(flag, steps);
			break;
		case 0x31:
			lower = get_memory_value(reg.pc++);
			higher = get_memory_value(reg.pc);
			reg.sp = (higher << 8) | lower;
			break;
		case 0x32:
			address = (reg.h << 8) | reg.l;
			set_memory_value(address, reg.a);
			dec_16(&reg.h, &reg.l);
			break;
		case 0x33:
			reg.sp++;
			break;
		case 0x34:
			address = (reg.h << 8) | reg.l;
			mem_value = get_memory_value(address);
			inc_8(&mem_value);
			set_memory_value(address, mem_value);
			break;
		case 0x35:
			address = (reg.h << 8) | reg.l;
			mem_value = get_memory_value(address);
			dec_8(&mem_value);
			set_memory_value(address, mem_value);
			break;
		case 0x36:
			address = (reg.h << 8) | reg.l;
			mem_value = get_memory_value(++reg.pc);
			set_memory_value(address, mem_value);
			break;
		case 0x38:
			flag = get_flag(C_FLAG);
			steps = get_memory_value(reg.pc + 1);
			jr_conditional(flag, steps);
			break;
		case 0x39:;
			lower = reg.sp & 0x00ff;
			higher = reg.sp >> 8; 
			add_x9(higher, lower);
			break;
		case 0x3a:
			ld_xa_16(reg.h, reg.l);
			hl = ((reg.h << 8) | reg.l) - 1;
			reg.h = hl >> 8;
			reg.l = hl & 0x00ff;
			break;
		case 0x3b:
			higher = (reg.sp & 0xff00) >> 8;
			lower = reg.sp & 0x00ff;
			dec_16(&higher, &lower);
			reg.sp = (higher << 8) | lower;
			break;
		case 0x3c:
			inc_8(&reg.a);
			break;
		case 0x3d:
			dec_8(&reg.a);
			break;
		case 0x3e:
			ld_8_immediate(&reg.a);
			break;
		case 0x4f:
			reg.c = reg.a;
			break;
		default:
			printf("Default case\n");
			break;
	}
	reg.pc++;
}


void set_flag(_Bool value, u_byte flag_mask){
	if(value){
		reg.f |= flag_mask;
	}
}

u_byte get_flag(u_byte flag_mask){
	return reg.f & flag_mask;
}

void add_x9(u_byte higher, u_byte lower){
	reg.f &= 0x80;
	u_short hl = (reg.h << 8) | reg.l;
	u_short nn = (higher << 8) | lower;
	set_flag((hl + nn) > 0xffff, C_FLAG);
	set_flag(((hl & 0x0fff) + (nn & 0x0fff)) > 0xfff, H_FLAG);
	reg.h += higher;
	reg.l += lower;
}

void ld_xa_16(u_byte higher, u_byte lower){
	u_short address = (higher << 8) | lower;
	reg.a = get_memory_value(address);
}

void ld_8_immediate(u_byte *reg_pointer){
	*reg_pointer = get_memory_value(++reg.pc);
}

void dec_16(u_byte *higher, u_byte *lower){
	u_short hl = ((*higher << 8) | *lower) - 1;
	*higher = (hl & 0xff00) >> 8;
	*lower = hl & 0x00ff;
}

void inc_8(u_byte *reg_pointer){
	reg.f &= 0x10;
	set_flag((*reg_pointer & 0x0f) == 0x0f, H_FLAG);
	set_flag(!++*reg_pointer, Z_FLAG);
}

void inc_16(u_byte *higher, u_byte *lower){
	u_short hl = (*higher << 8) | *lower;
	hl++;
	*higher = (hl & 0xff00) >> 8;
	*lower = hl & 0x00ff;
}

void dec_8(u_byte *reg_pointer){
	reg.f &= 0x10;
	set_flag(((*reg_pointer & 0x0f) == 0x00) && *reg_pointer, H_FLAG);
	set_flag(!--*reg_pointer, Z_FLAG);
}

void jr_conditional(_Bool flag, s_byte steps){
	if(flag){
		reg.pc += steps - 1;
	}
}

void daa(){
	u_byte lower = reg.a & 0x0f;
	u_byte higher = reg.a >> 4;

	if(!get_flag(N_FLAG)){ // ADD
		if(get_flag(C_FLAG)){
			if(!get_flag(H_FLAG) && lower < 0xa){
				reg.a += 0x60;
			}else{
				reg.a += 0x66;
			}
		}else{
			if(!get_flag(H_FLAG)){
				if(higher < 0x9 && lower > 0x9){
					reg.a += 0x06;
				}else if(higher > 0x9 && lower < 0xa){
					reg.a += 0x60;
				}else if(higher > 0x8 && lower > 0x9){
					reg.a += 0x66;
				}
			}
			else if(get_flag(H_FLAG) && higher < 0xa){
				reg.a += 0x06;
			}else{
				reg.a += 0x66;
				set_flag(1, C_FLAG);
			}
		}
	}else{ // SUB
		//TODO	
	}
	reg.f &= 0xd; 
}
