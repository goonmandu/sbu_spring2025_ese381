/*
 * simple_comb_function_nb.c
 *
 * Created: 2/13/2025 7:02:53 PM
 * Author : Mingi Hwang
 */ 

#include <avr/io.h>

#define A (input.bits.b7)
#define B (input.bits.b6)
#define C (input.bits.b5)

typedef struct Bitfield {
	uint8_t b0 : 1;
	uint8_t b1 : 1;
	uint8_t b2 : 1;
	uint8_t b3 : 1;
	uint8_t b4 : 1;
	uint8_t b5 : 1;
	uint8_t b6 : 1;
	uint8_t b7 : 1;
} Bitfield;

typedef union Byte {
	Bitfield bits;
	uint8_t byte;
} Byte;

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
	
	Byte input;
	while (1) {
		input.byte = ~VPORTA.IN;
		VPORTD.OUT = ((A && !B) || (A && C) || (!B && C) || (!A && B && !C)) << 7;
	}
}
