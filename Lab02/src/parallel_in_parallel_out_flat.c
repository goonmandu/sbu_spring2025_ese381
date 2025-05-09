/*
 * parallel_in_parallel_out_flat.c
 *
 * Created: 2/11/2025 11:46:21 AM
 * Author : Mingi Hwang
 */ 

#include <avr/io.h>

int main() {
	// Enable pull-ups for switches and pushbuttons
	PORTC_PIN0CTRL |= 0x08;
	PORTC_PIN1CTRL |= 0x08;
	PORTA_PIN2CTRL |= 0x08;
	PORTA_PIN3CTRL |= 0x08;
	PORTA_PIN4CTRL |= 0x08;
	PORTA_PIN5CTRL |= 0x08;
	PORTA_PIN6CTRL |= 0x08;
	PORTA_PIN7CTRL |= 0x08;
	
	// Port D is output, initial LEDs state is OFF
	VPORTD_DIR = 0xFF;
	VPORTD_OUT = 0xFF;
	
	while (1) {
		VPORTD_OUT = (VPORTA_IN & 0b11111100) | (VPORTC_IN & 0b00000011);
	}
}
