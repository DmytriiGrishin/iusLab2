#include "animation.h"
#include "led.h"
#include "system_timer.h"

// Timer T1 (mode switching) is being used for PWM (pulse width modulation)

// Brightness of bulbs
u8 power=0;

// LEVEL_ON
u8 power_l1=0xF0;
u8 power_h1=0xFF;

u8 direction;
// time of next frame
time next_frame;


const time FRAME_DELAY=500;

void T1_ISR( void ) __interrupt (1);

u8 powers[8];
u8 leds_state[8];
u8 state_to_power[8];

void initialize_animation() {
	TMOD |= 0b00010000; //Timer1, internal generator of frequency, 16 bit counter
	SetVector(0x201B, (void *)T1_ISR);
	state_to_power[0] = 100;
	state_to_power[1] = 0;
	state_to_power[2] = 0;
	state_to_power[3] = 20;
	state_to_power[4] = 40;
	state_to_power[5] = 60;	
	state_to_power[6] = 80;	
	state_to_power[7] = 100;	
}

void update_powers() {
	u8 i;
	for (i = 0; i < 8; i++) {
		powers[i] = state_to_power[leds_state[i]];
	}
}
void enable_animation(){
	direction = INCREASE_BRIGHTNESS;
	duty_cycle = 0;
	next_frame = get_ms() + FRAME_DELAY;

		
	leds_state[0] = 0;
	leds_state[1] = 1;
	leds_state[2] = 2;
	leds_state[3] = 3;
	leds_state[4] = 4;
	leds_state[5] = 5;	
	leds_state[6] = 6;	
	leds_state[7] = 7;	
	update_powers();
	TH1 = power_h1;
	TL1 = power_l1;
	
	ET1 = 1;       //enable (interrupts from) timer 1
	TR1 = 1;       //allow timer to count
}

void disable_animation(){
	ET1 = 0; // disable (interrupts from) timer 1
	TR1 = 0; // stop counting
}

u8 duty_cycle;
void update_leds() {
	u8 leds = 0;
	u8 i;
	for (i = 0; i < 8; i++) {
		leds |= powers[i] / 20 > duty_cycle ? 1 : 0;
		leds = leds << 1;
	}	
	leds(leds);
	if (duty_cycle >= 4) {
		duty_cycle = 0;
	}	

}	
void update_state() {
	u8 i;
	for (i = 0; i < 8; i++) {
		leds_state[i] = leds_state[i] == 7 ? 0 : leds_state[i] + 1;
	}
}
void T1_ISR( void ) __interrupt (1){
	duty_cycle++;
	TH1 = power_h1;
	TL1 = power_l1;
	update_leds();
	if (get_ms() >= next_frame) {
		next_frame = get_ms() + FRAME_DELAY;
		update_state();
		update_powers();
	}
}
