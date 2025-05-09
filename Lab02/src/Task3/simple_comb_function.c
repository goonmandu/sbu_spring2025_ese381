/*
 * simple_comb_function.c
 *
 * Created: 2/13/2025 3:36:21 PM
 * Author : Mingi Hwang
 */ 

#include <avr/io.h>

#define A (~VPORTA.IN & 0b10000000)
#define B (~VPORTA.IN & 0b01000000)
#define C (~VPORTA.IN & 0b00100000)

int main() {
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
	
	//     A * ~B
	// +   A *  C
	// +  ~B *  C
	// +  ~A *  B * ~C
	
	while (1) {
		VPORTD.OUT = ~(((A && !B) || (A && C) || (!B && C) || (!A && B && !C)) << 7);
	}
}
