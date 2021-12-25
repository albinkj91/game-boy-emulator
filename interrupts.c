#include "global.h"

_Bool ime = false;

void enable_interrupts(){
	ime = true;
}

void disable_interrupts(){
	ime = false;
}
