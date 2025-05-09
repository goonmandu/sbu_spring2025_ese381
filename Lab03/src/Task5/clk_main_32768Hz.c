/*
 * clk_main_32768Hz.c
 *
 * Created: 2/18/2025 4:59:41 PM
 * Author : user
 */ 

#define F_CPU 4000000

#include <avr/io.h>

int main(void) {
	CPU_CCP = CCP_IOREG_gc;  // 0xD8
	CLKCTRL.MCLKCTRLA = 0b10000001;
	
	while (1) {
		
	}
}
