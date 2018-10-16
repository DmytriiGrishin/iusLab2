#include "counter.h"

void initialize_counter() {
	TMOD |= 0b00000100; 
	TH0=TL0=0;	
	TR0=1;      
}

u8 get_count() {
	return TL0;
}