/*
 * parallel_in_parallel_out_struct.c
 *
 * Created: 2/11/2025 12:39:48 PM
 * Author : Mingi Hwang
 */

#include <avr/io.h>

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
	
	// Port D is output, initial LEDs state is OFF
	VPORTD.DIR = 0xFF;
	VPORTD.OUT = 0xFF;
	
	while (1) {
		VPORTD.OUT = (VPORTA.IN & 0b11111100) | (VPORTC.IN & 0b00000011);
	}
}
