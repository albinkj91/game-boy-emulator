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
void add_8(u_byte value);
void add_x9(u_byte higher, u_byte lower);
void adc(u_byte value);
void sub_8(u_byte value);
void sbc(u_byte value);
void ld_8_immediate(u_byte *reg_pointer);
void ld_xa_16(u_byte higher, u_byte lower);
void dec_16(u_byte *higher, u_byte *lower);
void inc_8(u_byte *reg_pointer);
void inc_16(u_byte *higher, u_byte *lower);
void dec_8(u_byte *reg_pointer);
void jr_conditional(_Bool flag, s_byte steps);
void daa();


int main(){
	init_registers();
	execute_opcode(0x98);
	printf("REG A = %x\n", reg.a);
	printf("REG F = %x\n", reg.f);
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
	u_byte temp_flag;

	switch(opcode){
		case 0x0:
			reg.pc++;
			break;
		case 0x01:
			reg.c = get_memory_value(reg.pc + 1);
			reg.b = get_memory_value(reg.pc + 2);
			reg.pc += 3;
			break;
		case 0x02:
			address = (reg.b << 8) | reg.c;
			set_memory_value(address, reg.a);
			reg.pc++;
			break;
		case 0x03:
			inc_16(&reg.b, &reg.c);
			reg.pc++;
			break;
		case 0x04:
			inc_8(&reg.b);
			reg.pc++;
			break;
		case 0x05:
			dec_8(&reg.b);
			reg.pc++;
			break;
		case 0x06:
			ld_8_immediate(&reg.b);
			reg.pc += 2;
			break;
		case 0x07:
			reg.f &= 0x0;
			set_flag(reg.a & 0x80, C_FLAG);
			reg.a = (reg.a << 1) | get_flag(C_FLAG);
			reg.pc++;
			break;
		case 0x08:;
			lower = get_memory_value(reg.pc + 1);
			higher = get_memory_value(reg.pc + 2);
			address = (higher << 8) | lower;
			set_memory_value(address++, reg.sp & 0x00ff);
			set_memory_value(address, reg.sp >> 8);
			reg.pc += 3;
			break;
		case 0x09:
			add_x9(reg.b, reg.c);
			reg.pc++;
			break;
		case 0x0a:
			ld_xa_16(reg.b, reg.c);
			reg.pc++;
			break;
		case 0x0b:
			dec_16(&reg.b, &reg.c);
			reg.pc++;
			break;
		case 0x0c:
			inc_8(&reg.c);
			reg.pc++;
			break;
		case 0x0d:
			dec_8(&reg.c);
			reg.pc++;
			break;
		case 0x0e:
			ld_8_immediate(&reg.c);
			reg.pc += 2;
			break;
		case 0x0f:
			reg.f &= 0x0;
			set_flag(reg.a & 0x1, C_FLAG);
			reg.a = (reg.a >> 1) | (get_flag(C_FLAG) << 3);
			reg.pc++;
			break;
		case 0x10:
			//TODO: STOP
			printf("0x10 STOP, not implemented yet\n");
			break;
		case 0x11:
			reg.e = get_memory_value(reg.pc + 1);
			reg.d = get_memory_value(reg.pc + 2);
			reg.pc += 3;
			break;
		case 0x12:
			address = (reg.d << 8) | reg.e;
			set_memory_value(address, reg.a);
			reg.pc++;
			break;
		case 0x13:
			inc_16(&reg.d, &reg.e);
			reg.pc++;
			break;
		case 0x14:
			inc_8(&reg.d);
			reg.pc++;
			break;
		case 0x15:
			dec_8(&reg.d);
			reg.pc++;
			break;
		case 0x16:
			ld_8_immediate(&reg.d);
			reg.pc += 2;
			break;
		case 0x17:
			reg.f &= 0x10;
			temp_flag = reg.f;
			set_flag(reg.a & 0x80, C_FLAG);
			reg.a = (reg.a << 1) | (temp_flag >> 4);
			reg.pc++;
			break;
		case 0x18:
			steps = get_memory_value(reg.pc + 1);
			reg.pc += steps;
			break;
		case 0x19:
			add_x9(reg.d, reg.e);
			reg.pc++;
			break;
		case 0x1a:
			ld_xa_16(reg.d, reg.e);
			reg.pc++;
			break;
		case 0x1b:
			dec_16(&reg.d, &reg.e);
			reg.pc++;
			break;
		case 0x1c:
			inc_8(&reg.e);
			reg.pc++;
			break;
		case 0x1d:
			dec_8(&reg.e);
			reg.pc++;
			break;
		case 0x1e:
			ld_8_immediate(&reg.e);
			reg.pc += 2;
			break;
		case 0x1f:
			reg.f &= 0x10;
			temp_flag = reg.f;
			set_flag(reg.a & 0x1, C_FLAG);
			reg.a = (reg.a >> 1) | (temp_flag >> 4);
			reg.pc++;
		case 0x20:
			flag = !get_flag(Z_FLAG);
			steps = get_memory_value(reg.pc + 1);
			jr_conditional(flag, steps);
			break;
		case 0x21:
			reg.l = get_memory_value(reg.pc + 1);
			reg.h = get_memory_value(reg.pc + 2);
			reg.pc += 3;
			break;
		case 0x22:
			address = (reg.h << 8) | reg.l;
			set_memory_value(address, reg.a);
			inc_16(&reg.h, &reg.l);
			reg.pc++;
			break;
		case 0x23:
			inc_16(&reg.h, &reg.l);
			reg.pc++;
			break;
		case 0x24:
			inc_8(&reg.h);
			reg.pc++;
			break;
		case 0x25:
			dec_8(&reg.h);
			reg.pc++;
			break;
		case 0x26:
			ld_8_immediate(&reg.h);
			reg.pc += 2;
			break;
		case 0x27:
			daa();
			reg.pc++;
			break;
		case 0x28:
			flag = get_flag(Z_FLAG);
			steps = get_memory_value(reg.pc + 1);
			jr_conditional(flag, steps);
			break;
		case 0x29:
			add_x9(reg.h, reg.l);
			reg.pc++;
			break;
		case 0x2a:
			ld_xa_16(reg.h, reg.l);
			inc_16(&reg.h, &reg.l);
			reg.pc++;
			break;
		case 0x2b:
			dec_16(&reg.h, &reg.l);
			reg.pc++;
			break;
		case 0x2c:
			inc_8(&reg.l);
			reg.pc++;
			break;
		case 0x2d:
			dec_8(&reg.l);
			reg.pc++;
			break;
		case 0x2e:
			ld_8_immediate(&reg.l);
			reg.pc += 2;
			break;
		case 0x2f:
			reg.a = ~reg.a;
			set_flag(1, H_FLAG);
			set_flag(1, N_FLAG);
			break;
		case 0x30:
			flag = !get_flag(C_FLAG);
			steps = get_memory_value(reg.pc + 1);
			jr_conditional(flag, steps);
			break;
		case 0x31:
			lower = get_memory_value(reg.pc + 1);
			higher = get_memory_value(reg.pc + 2);
			reg.sp = (higher << 8) | lower;
			reg.pc += 3;
			break;
		case 0x32:
			address = (reg.h << 8) | reg.l;
			set_memory_value(address, reg.a);
			dec_16(&reg.h, &reg.l);
			reg.pc++;
			break;
		case 0x33:
			reg.sp++;
			reg.pc++;
			break;
		case 0x34:
			address = (reg.h << 8) | reg.l;
			mem_value = get_memory_value(address);
			inc_8(&mem_value);
			set_memory_value(address, mem_value);
			reg.pc++;
			break;
		case 0x35:
			address = (reg.h << 8) | reg.l;
			mem_value = get_memory_value(address);
			dec_8(&mem_value);
			set_memory_value(address, mem_value);
			reg.pc++;
			break;
		case 0x36:
			address = (reg.h << 8) | reg.l;
			mem_value = get_memory_value(reg.pc + 1);
			set_memory_value(address, mem_value);
			reg.pc += 2;
			break;
		case 0x37:
			reg.f |= 0x90;
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
			reg.pc++;
			break;
		case 0x3a:
			ld_xa_16(reg.h, reg.l);
			hl = ((reg.h << 8) | reg.l) - 1;
			reg.h = hl >> 8;
			reg.l = hl & 0x00ff;
			reg.pc++;
			break;
		case 0x3b:
			higher = (reg.sp & 0xff00) >> 8;
			lower = reg.sp & 0x00ff;
			dec_16(&higher, &lower);
			reg.sp = (higher << 8) | lower;
			reg.pc++;
			break;
		case 0x3c:
			inc_8(&reg.a);
			reg.pc++;
			break;
		case 0x3d:
			dec_8(&reg.a);
			reg.pc++;
			break;
		case 0x3e:
			ld_8_immediate(&reg.a);
			reg.pc += 2;
			break;
		case 0x3f:
			reg.f &= 0x90;
			reg.f ^= 0x10;
			break;
		case 0x40:
			reg.b = reg.b;
			reg.pc++;
			break;
		case 0x41:
			reg.b = reg.c;
			reg.pc++;
			break;
		case 0x42:
			reg.b = reg.d;
			reg.pc++;
			break;
		case 0x43:
			reg.b = reg.e;
			reg.pc++;
			break;
		case 0x44:
			reg.b = reg.h;
			reg.pc++;
			break;
		case 0x45:
			reg.b = reg.l;
			reg.pc++;
			break;
		case 0x46:
			address = (reg.h << 8) | reg.l;
			reg.b = get_memory_value(address);
			reg.pc++;
			break;
		case 0x47:
			reg.b = reg.a;
			reg.pc++;
			break;
		case 0x48:
			reg.c = reg.b;
			reg.pc++;
			break;
		case 0x49:
			reg.c = reg.c;
			reg.pc++;
			break;
		case 0x4a:
			reg.c = reg.d;
			reg.pc++;
			break;
		case 0x4b:
			reg.c = reg.e;
			reg.pc++;
			break;
		case 0x4c:
			reg.c = reg.h;
			reg.pc++;
			break;
		case 0x4d:
			reg.c = reg.l;
			reg.pc++;
			break;
		case 0x4e:
			address = (reg.h << 8) | reg.l;
			reg.c = get_memory_value(address);
			reg.pc++;
			break;
		case 0x4f:
			reg.c = reg.a;
			reg.pc++;
			break;
		case 0x50:
			reg.d = reg.b;
			reg.pc++;
			break;
		case 0x51:
			reg.d = reg.c;
			reg.pc++;
			break;
		case 0x52:
			reg.d = reg.d;
			reg.pc++;
			break;
		case 0x53:
			reg.d = reg.e;
			reg.pc++;
			break;
		case 0x54:
			reg.d = reg.h;
			reg.pc++;
			break;
		case 0x55:
			reg.d = reg.l;
			reg.pc++;
			break;
		case 0x56:
			address = (reg.h << 8) | reg.l;
			reg.d = get_memory_value(address);
			reg.pc++;
			break;
		case 0x57:
			reg.d = reg.a;
			reg.pc++;
			break;
		case 0x58:
			reg.e = reg.b;
			reg.pc++;
			break;
		case 0x59:
			reg.e = reg.c;
			reg.pc++;
			break;
		case 0x5a:
			reg.e = reg.d;
			reg.pc++;
			break;
		case 0x5b:
			reg.e = reg.e;
			reg.pc++;
			break;
		case 0x5c:
			reg.e = reg.h;
			reg.pc++;
			break;
		case 0x5d:
			reg.e = reg.l;
			reg.pc++;
			break;
		case 0x5e:
			address = (reg.h << 8) | reg.l;
			reg.e = get_memory_value(address);
			reg.pc++;
			break;
		case 0x5f:
			reg.e = reg.a;
			reg.pc++;
			break;
		case 0x60:
			reg.h = reg.b;
			reg.pc++;
			break;
		case 0x61:
			reg.h = reg.c;
			reg.pc++;
			break;
		case 0x62:
			reg.h = reg.d;
			reg.pc++;
			break;
		case 0x63:
			reg.h = reg.e;
			reg.pc++;
			break;
		case 0x64:
			reg.h = reg.h;
			reg.pc++;
			break;
		case 0x65:
			reg.h = reg.l;
			reg.pc++;
			break;
		case 0x66:
			address = (reg.h << 8) | reg.l;
			reg.h = get_memory_value(address);
			reg.pc++;
			break;
		case 0x67:
			reg.h = reg.a;
			reg.pc++;
			break;
		case 0x68:
			reg.l = reg.b;
			reg.pc++;
			break;
		case 0x69:
			reg.l = reg.c;
			reg.pc++;
			break;
		case 0x6a:
			reg.l = reg.d;
			reg.pc++;
			break;
		case 0x6b:
			reg.l = reg.e;
			reg.pc++;
			break;
		case 0x6c:
			reg.l = reg.h;
			reg.pc++;
			break;
		case 0x6d:
			reg.l = reg.l;
			reg.pc++;
			break;
		case 0x6e:
			address = (reg.h << 8) | reg.l;
			reg.l = get_memory_value(address);
			reg.pc++;
			break;
		case 0x6f:
			reg.l = reg.a;
			reg.pc++;
			break;
		case 0x70:
			address = (reg.h << 8) | reg.l;
			set_memory_value(address, reg.b);
			reg.pc++;
			break;
		case 0x71:
			address = (reg.h << 8) | reg.l;
			set_memory_value(address, reg.c);
			reg.pc++;
			break;
		case 0x72:
			address = (reg.h << 8) | reg.l;
			set_memory_value(address, reg.d);
			reg.pc++;
			break;
		case 0x73:
			address = (reg.h << 8) | reg.l;
			set_memory_value(address, reg.e);
			reg.pc++;
			break;
		case 0x74:
			address = (reg.h << 8) | reg.l;
			set_memory_value(address, reg.h);
			reg.pc++;
			break;
		case 0x75:
			address = (reg.h << 8) | reg.l;
			set_memory_value(address, reg.l);
			reg.pc++;
			break;
		case 0x76:
			//TODO: HALT
			break;
		case 0x77:
			address = (reg.h << 8) | reg.l;
			set_memory_value(address, reg.a);
			reg.pc++;
			break;
		case 0x78:
			reg.a = reg.b;
			reg.pc++;
			break;
		case 0x79:
			reg.a = reg.c;
			reg.pc++;
			break;
		case 0x7a:
			reg.a = reg.d;
			reg.pc++;
			break;
		case 0x7b:
			reg.a = reg.e;
			reg.pc++;
			break;
		case 0x7c:
			reg.a = reg.h;
			reg.pc++;
			break;
		case 0x7d:
			reg.a = reg.l;
			reg.pc++;
			break;
		case 0x7e:
			address = (reg.h << 8) | reg.l;
			reg.a = get_memory_value(address);
			reg.pc++;
			break;
		case 0x7f:
			reg.a = reg.a;
			reg.pc++;
			break;
		case 0x80:
			add_8(reg.b);
			reg.pc++;
			break;
		case 0x81:
			add_8(reg.c);
			reg.pc++;
			break;
		case 0x82:
			add_8(reg.d);
			reg.pc++;
			break;
		case 0x83:
			add_8(reg.e);
			reg.pc++;
			break;
		case 0x84:
			add_8(reg.h);
			reg.pc++;
			break;
		case 0x85:
			add_8(reg.l);
			reg.pc++;
			break;
		case 0x86:
			address = (reg.h << 8) | reg.l;
			mem_value = get_memory_value(address);
			add_8(mem_value);
			reg.pc++;
			break;
		case 0x87:
			add_8(reg.a);
			reg.pc++;
			break;
		case 0x88:
			adc(reg.b);
			reg.pc++;
			break;
		case 0x89:
			adc(reg.c);
			reg.pc++;
			break;
		case 0x8a:
			adc(reg.d);
			reg.pc++;
			break;
		case 0x8b:
			adc(reg.e);
			reg.pc++;
			break;
		case 0x8c:
			adc(reg.h);
			reg.pc++;
			break;
		case 0x8d:
			adc(reg.l);
			reg.pc++;
			break;
		case 0x8e:
			address = (reg.h << 8) | reg.l;
			mem_value = get_memory_value(address);
			adc(mem_value);
			reg.pc++;
			break;
		case 0x8f:
			adc(reg.a);
			reg.pc++;
			break;
		case 0x90:
			sub_8(reg.b);
			reg.pc++;
			break;
		case 0x91:
			sub_8(reg.c);
			reg.pc++;
			break;
		case 0x92:
			sub_8(reg.d);
			reg.pc++;
			break;
		case 0x93:
			sub_8(reg.e);
			reg.pc++;
			break;
		case 0x94:
			sub_8(reg.h);
			reg.pc++;
			break;
		case 0x95:
			sub_8(reg.l);
			reg.pc++;
			break;
		case 0x96:
			address = (reg.h << 8) | reg.l;
			mem_value = get_memory_value(address);
			sub_8(mem_value);
			reg.pc++;
			break;
		case 0x97:
			sub_8(reg.a);
			reg.pc++;
			break;
		case 0x98:
			sbc(reg.b);
			reg.pc++;
			break;
		case 0x99:
			sbc(reg.c);
			reg.pc++;
			break;
		case 0x9a:
			sbc(reg.d);
			reg.pc++;
			break;
		case 0x9b:
			sbc(reg.e);
			reg.pc++;
			break;
		case 0x9c:
			sbc(reg.h);
			reg.pc++;
			break;
		case 0x9d:
			sbc(reg.l);
			reg.pc++;
			break;
		case 0x9e:
			address = (reg.h << 8) | reg.l;
			mem_value = get_memory_value(address);
			sbc(mem_value);
			reg.pc++;
			break;
		case 0x9f:
			sbc(reg.a);
			reg.pc++;
			break;
		default:
			printf("OP Code not recognized.\n");
			break;
	}
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
	set_flag(((hl & 0xfff) + (nn & 0xfff)) > 0xfff, H_FLAG);
	reg.h += higher;
	reg.l += lower;
}

void add_8(u_byte value){
	reg.f &= 0x0;
	u_byte lower = value & 0xf;
	u_byte a_lower = reg.a & 0xf;
	set_flag((reg.a + value) > 0xff, C_FLAG);
	set_flag((a_lower + lower) > 0xf, H_FLAG);
	reg.a += value;
	set_flag(reg.a == 0, Z_FLAG);
}

void adc(u_byte value){
	_Bool c_flag = get_flag(C_FLAG);
	reg.f &= 0x0;
	u_byte lower = value & 0xf;
	u_byte a_lower = reg.a & 0xf;
	set_flag((reg.a + value + c_flag) > 0xff, C_FLAG);
	set_flag((a_lower + lower + c_flag) > 0xf, H_FLAG);
	reg.a += value + c_flag;
	set_flag(reg.a == 0, Z_FLAG);
}

void sub_8(u_byte value){
	reg.f &= 0x0;
	reg.f |= 0x40;
	u_byte lower = value & 0xf;
	u_byte a_lower = reg.a & 0xf;
	set_flag((reg.a - value) < 0x0, C_FLAG);
	set_flag((a_lower - lower) < 0x0, H_FLAG);
	reg.a -= value;
	set_flag(reg.a == 0, Z_FLAG);
}

void sbc(u_byte value){
	_Bool c_flag = get_flag(C_FLAG);
	reg.f &= 0x0;
	reg.f |= 0x40;
	u_byte lower = value & 0xf;
	u_byte a_lower = reg.a & 0xf;
	set_flag((reg.a - value - c_flag) < 0x0, C_FLAG);
	set_flag((a_lower - lower - c_flag) < 0x0, H_FLAG);
	reg.a -= value;
	reg.a -= c_flag;
	set_flag(reg.a == 0, Z_FLAG);
}

void ld_xa_16(u_byte higher, u_byte lower){
	u_short address = (higher << 8) | lower;
	reg.a = get_memory_value(address);
}

void ld_8_immediate(u_byte *reg_pointer){
	*reg_pointer = get_memory_value(reg.pc + 1);
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
	set_flag(((*reg_pointer & 0x0f) == 0x0) && *reg_pointer, H_FLAG);
	set_flag(!--*reg_pointer, Z_FLAG);
}

void jr_conditional(_Bool flag, s_byte steps){
	if(flag){
		reg.pc += steps;
	}else{
		reg.pc += 2;
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
		if(!get_flag(C_FLAG)){
			if(get_flag(H_FLAG)){
				reg.a += 0xfa;
			}
		}else{
			if(get_flag(H_FLAG)){
				reg.a += 0x9a;
			}else{
				reg.a += 0xa0;
			}
			set_flag(1, C_FLAG);
		}
	}
}
