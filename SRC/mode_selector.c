#include "mode_selector.h"
u8 mode;

void SERIAL_COMMUNICATION_ISR (void) __interrupt (1);
void initialize_mode_selector() {
	IT0 = 1;  
	EX0 = 1; 
	SetVector(0x2003, (void *) SERIAL_COMMUNICATION_ISR); 
}
void SERIAL_COMMUNICATION_ISR( void ) __interrupt (1){
	if(mode == MODE_COUNTER) {
		mode = MODE_ANIMATION;
	}else{
		mode = MODE_COUNTER;
	}
}