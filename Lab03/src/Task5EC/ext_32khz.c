/*
 * ext_32khz.c
 *
 * Created: 2/18/2025 5:22:11 PM
 * Author : user
 */ 

#include <avr/io.h>

int main(void) {
	CPU_CCP = CCP_IOREG_gc;  // 0xD8
	CLKCTRL.MCLKCTRLA = 0b10000010;
	
	// Without this part, the 4MHz internal HF oscillator
	// is enabled as a failsafe.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = 0b10000001;
	
	while (1) {
		
	}
}
