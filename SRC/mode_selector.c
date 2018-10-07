#include "mode_selector.h"

// Use timer T1 for mode switching

u8 mode;

// interrupt service routine
void SERIAL_COMMUNICATION_ISR (void) __interrupt (5);

// setup timer T1 and load ISR for serial communication interrupt
void initialize_mode_selector() {
	IT1 = 1; // set interrupt type equal to 1 (edge triggered) 
	EX1 = 1; // enable external interrupt 1
	SetVector(0x2013, (void *) SERIAL_COMMUNICATION_ISR); // set our own service routine for the interrupt from T1
}

// at each interrupt from the timer switch animation mode
void SERIAL_COMMUNICATION_ISR( void ) __interrupt (5){
	if(mode == MODE_COUNTER) {
		mode = MODE_ANIMATION;
	}else{
		mode = MODE_COUNTER;
	}
}