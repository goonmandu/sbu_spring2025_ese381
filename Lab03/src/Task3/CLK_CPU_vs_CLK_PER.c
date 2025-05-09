/*
 * CLK_CPU_vs_CLK_PER.c
 *
 * Created: 2/18/2025 1:15:35 PM
 * Author : user
 */ 

// Compile under assumption that
// CPU runs at 1 MHz.
#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	// Routes CLKOUT to PA7
	CPU_CCP = CCP_IOREG_gc;  // 0xD8
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm;  // 0x80
		
	// Enables prescaler to DIV/4
	CPU_CCP = CCP_IOREG_gc;  // 0xD8
	CLKCTRL.MCLKCTRLB |= 0b00000011;  //
	
	// Set PB3 as output
	PORTB_DIR |= PIN3_bm;
		
	while (1) {
		// LED should blink at 1 Hz if CLK_CPU is 1 MHz.
		PORTB_OUTSET = PIN3_bm;
		_delay_ms(500);
		PORTB_OUTCLR = PIN3_bm;
		_delay_ms(500);
    }
}

