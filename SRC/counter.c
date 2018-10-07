#include "counter.h"

// Use timer T0 as a counter

void initialize_counter() {
	TMOD |= 0b00000100; // counter mode for timer T0
	TH0=TL0=0;	// make counter register be equal to zero
	TR0=1;      // allow timer to count (make timer 0 act as a counter by counting pulses from external input pin 3.4)
}

u8 get_count() {
	return TL0;
}