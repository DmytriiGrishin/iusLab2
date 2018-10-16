#include "mode_selector.h"
#include "max.h"
u8 mode;

void SERIAL_COMMUNICATION_ISR (void) __interrupt (1);
void initialize_mode_selector() {
	IT0 = 1;  
	EX0 = 1; 
	unsigned char old_ena = read_max(ENA);
	write_max(ENA, old_ena | 0x20);
	SetVector(0x2003, (void *) SERIAL_COMMUNICATION_ISR); 
}
void SERIAL_COMMUNICATION_ISR( void ) __interrupt (1){
	if(mode == MODE_COUNTER) {
		mode = MODE_ANIMATION;
	}else{
		mode = MODE_COUNTER;
	}
}