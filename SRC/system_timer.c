#include "system_timer.h"
#include "led.h"

// Use timer T2 as a system timer (for counting milliseconds)

u16 cnt = 0;
u8 levelo;

const u8 LEVEL_ON = 0xF0;
const u8 LEVEL_OFF = 0x0F;

time cur_ms;

// interrupt service routine
void T2_ISR( void ) __interrupt ( 2 );

void initialize_system_timer() {
	cur_ms = 0;
	cnt = 0;
	
	SetVector(0x202B, (void *)T2_ISR); // set our own service routine for the interrupt from T2

	levelo = LEVEL_ON;
	leds(levelo);
	
	// generate interrupt each millisecond
	TH2 = (-1000) >> 8; // TH2 (highest 8 bits of timer 2 counting register) = 1111 1100
	TL2 = (-1000) & 0xFF; // TL2 (lowest 8 bits of timer 2 counting register) = 0001 1000
	// temporarily store the contents of the counter register
	RCAP2H = (-1000) >> 8; // RCAP2H (highest 8 bits of timer 2 capture register) = 1111 1100
	RCAP2L = (-1000) & 0xFF; // RCAP2L (lowest 8 bits of timer 2 capture register) = 0001 1000
	
	ET2=1;	// enable (interrupt from) timer 2
	TR2=1;  // start timer 2
}

// get current number of milliseconds, counted by timer
time get_ms(void){
	return cur_ms;
}

time get_ms_after(time t0){
	return cur_ms-t0;
}

void delay_ms(time t){
	time now = get_ms();
	while(get_ms_after(now) < t){}
}

void T2_ISR( void ) __interrupt ( 2 ){
	cur_ms++;
}
