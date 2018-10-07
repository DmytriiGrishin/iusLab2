#include "aduc812.h"
#include "led.h"
#include "animation.h"
#include "mode_selector.h"
#include "counter.h"
#include "system_timer.h"

void delay ( unsigned long ms );

void main( void ) {
	
	initialize_system_timer(); // start counting milliseconds by timer 2
	initialize_mode_selector(); // start switching modes (animation/counting)
	initialize_animation();
	initialize_counter();
	
	// disable animation by default
	mode = MODE_COUNTER;
	
	EA = 1;
	
	while (1){
		// if animation should be enabled, enable it and wait while it should be turned off
		if (mode == MODE_ANIMATION){
			enable_animation();
			
			while(mode == MODE_ANIMATION){
				delay_ms(1);
			}
			
			disable_animation();
		}
		
		// if animation is disabled, show value from counter (timer T0)
		if (mode == MODE_COUNTER){
			while (mode == MODE_COUNTER){
				leds (get_count());
				delay_ms(1);
			}
		}
	}
}