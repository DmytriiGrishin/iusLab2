#include "animation.h"
#include "led.h"
#include "system_timer.h"

u8 power_l=0x18;
u8 power_h=0xFA;
time next_frame;
u8 powers[8];
u8 leds_state[8];
u8 state_to_power[8];
u8 duty_cycle; 
const time FRAME_DELAY=2000;
void T1_ISR( void ) __interrupt (1);

void initialize_animation() {
	TMOD |= 0b00010000;
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

void enable_animation() {
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
	TH1 = power_h;
	TL1 = power_l;
	ET1 = 1;       
	TR1 = 1;      
}

void disable_animation() {
	ET1 = 0; 
	TR1 = 0; 
}
void update_leds() {
	u8 leds1 = 0;
	u8 i;
	for (i = 0; i < 7; i++) {
		leds1 |= powers[i] / 20 > duty_cycle ? 1 : 0;
		leds1 = leds1 << 1;
	}	
	leds1 |= powers[7] / 20 > duty_cycle ? 1 : 0;
	leds(leds1);
}	

void update_state() {
	u8 i;
	for (i = 0; i < 8; i++) {
		leds_state[i] = leds_state[i] == 7 ? 0 : leds_state[i] + 1;
	}
}

void update_duty_cycle() {
	if (duty_cycle >= 4) {
		duty_cycle = 0;
	} else {
		duty_cycle++;		
	}
}

void T1_ISR(void) __interrupt (1){
	TH1 = power_h;
	TL1 = power_l;
	update_duty_cycle();
	update_leds();
	if (get_ms() >= next_frame) {
		next_frame = get_ms() + FRAME_DELAY;
		update_state();
		update_powers();
	}
}
