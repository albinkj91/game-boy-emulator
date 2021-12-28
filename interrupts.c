#include "global.h"

_Bool ime = 0;

void enable_interrupts(){
	ime = 1;
}

void disable_interrupts(){
	ime = 0;
}
