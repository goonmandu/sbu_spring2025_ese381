/*
 * toggle_every_xxx_us.c
 *
 * Created: 2/18/2025 1:41:02 PM
 * Author : user
 */ 

#define F_CPU 4000000
#define FAST 31
#define MID 65
#define SLOW 134

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	// PC1 PC0 input, PB3 output
	PORTC.DIR &= 0x00;
    PORTB.DIR |= PIN3_bm;
	PORTC.PIN0CTRL |= 0x08;
	PORTC.PIN1CTRL |= 0x08;
	
	// Input variable
	volatile uint8_t *input = &PORTC.IN;
	uint8_t temp;
	
    while (1) {
		temp = ~(*input) & 0x03;
		PORTB_OUTSET = PIN3_bm;
		if (temp == 0x00) {
			_delay_loop_1(FAST);
		} else if (temp == 0x01) {
			_delay_loop_1(MID);
		} else if (temp == 0x02) {
			_delay_loop_1(SLOW);
		}
		PORTB_OUTCLR = PIN3_bm;
		if (temp == 0x00) {
			_delay_loop_1(FAST);
		} else if (temp == 0x01) {
			_delay_loop_1(MID);
		} else if (temp == 0x02) {
			_delay_loop_1(SLOW);
		}
    }
}
