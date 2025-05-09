/*
 * blink_LED0_1sec.c
 *
 * Created: 2/18/2025 11:43:28 AM
 * Author : user
 */ 

#define F_CPU 4000000

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	CPU_CCP = CCP_IOREG_gc;  // 0xD8
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm;  // 0x80
	PORTB_DIR |= PIN3_bm;
	
    while (1) {
		PORTB_OUTSET = PIN3_bm;
		_delay_ms(500);
		PORTB_OUTCLR = PIN3_bm;
		_delay_ms(500);
    }
}
