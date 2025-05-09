#include <avr/io.h>

typedef union {
	uint8_t byte;
	struct {
		uint8_t b0 : 1;
		uint8_t b1 : 1;
		uint8_t b2 : 1;
		uint8_t b3 : 1;
		uint8_t b4 : 1;
		uint8_t b5 : 1;
		uint8_t b6 : 1;
		uint8_t b7 : 1;
	} bit;
} bits;

uint8_t f(bits in) {
	return !(in.bit.b5) && (in.bit.b6 || in.bit.b7);
}

int main() {
	PORTC.PIN5CTRL |= 0x08;
	PORTC.PIN6CTRL |= 0x08;
	PORTC.PIN7CTRL |= 0x08;
	PORTC.DIR &= 0x1F;
	PORTD.DIR |= 0x80;
	
	uint8_t *pc_ptr = &PORTC.IN;
	bits in;
	
	while (1) {
		in.byte = *pc_ptr;
		if (f(in)) {
			PORTD.OUT |= 0x80;
		} else {
			PORTD.OUT &= ~0x80;
		}
	}
}