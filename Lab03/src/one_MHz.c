/*
 * one_MHz.c
 *
 * Created: 2/18/2025 11:34:32 AM
 * Author : user
 */ 

#define F_CPU 4000000

#include <avr/io.h>

int main(void) {
    CPU_CCP = CCP_IOREG_gc;  // 0xD8
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm;  // 0x80
	CPU_CCP = CCP_IOREG_gc;  // 0xD8
	CLKCTRL.OSCHFCTRLA &= 0b11000011;
	
    while (1) {
		
    }
}