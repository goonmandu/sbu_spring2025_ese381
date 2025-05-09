/*
 * read_modify_write_sftw_sw0.c
 *
 * Created: 2/13/2025 6:21:48 PM
 * Author : Mingi Hwang
 */

#include <avr/io.h>

#define SWITCH_INPUT ((VPORTA.IN & 0b11111100) | (VPORTC.IN & 0b00000011))

int main() {
	// Enable pull-ups for switches and pushbuttons
	PORTC.PIN0CTRL |= 0x08;
	PORTC.PIN1CTRL |= 0x08;
	PORTA.PIN2CTRL |= 0x08;
	PORTA.PIN3CTRL |= 0x08;
	PORTA.PIN4CTRL |= 0x08;
	PORTA.PIN5CTRL |= 0x08;
	PORTA.PIN6CTRL |= 0x08;
	PORTA.PIN7CTRL |= 0x08;
	PORTB.PIN2CTRL |= 0x08;
	
	// Port D is output, initial LEDs state is OFF
	VPORTD.DIR = 0xFF;
	VPORTD.OUT = 0xFF;
	
	// Initial state of Port D outputs
	VPORTD.OUT = SWITCH_INPUT;
	
	while (1) {
		VPORTD.OUT = SWITCH_INPUT;
		// when button is pressed, b2-b0 of switch controls b5-b3 of LEDs
		while (!(VPORTB.IN & 0b00000100)) {
			VPORTD.OUT = ((VPORTD.OUT & 0b11000111) | ((SWITCH_INPUT & 0b00000111) << 3));
		}
	}
}
