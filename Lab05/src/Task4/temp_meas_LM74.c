#define SERLCD_ROWS 4
#define SERLCD_COLS 20
#define F_CPU 4000000

#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <spi/spi0.h>


void clear_display_buffer();
void update_SerLCD();
float lm74_to_celsius(uint16_t result);
float c_to_f(float c);

char serlcd_buffer[SERLCD_ROWS][SERLCD_COLS+1];

int main() {
	volatile uint16_t lm74_result;
	volatile float deg_c, deg_f;
	volatile int8_t ipart, fpart;
	
	PORTMUX.SPIROUTEA = PORTMUX_SPI0_DEFAULT_gc;
	PORTA.DIR = 0xD8;	/* PA7   PA6   PA5   PA4   PA3  */
						/* LCD   SCK   MISO  MOSI  LM74 */
						/* OUT   OUT   IN    OUT   OUT  */
						/*  1     1     0     1     1   */
	PORTA_OUTSET = 0x88;
	
	init_spi0();
	init_spi0_SerLCD();
	
	clear_display_buffer();
	update_SerLCD();
	//                         01234567890123456789
	sprintf(serlcd_buffer[0], "  LM74 Temperature  ");
	update_SerLCD();
	
	while (1) {
		init_spi0_LM74();
		lm74_result = read_spi0_LM74();
		deg_c = lm74_to_celsius(lm74_result);
		
		// uint16_t test = 0b1111100010101101;
		// deg_c = lm74_to_celsius(test);
		
		deg_f = c_to_f(deg_c);
		ipart = (int8_t)deg_c;
		fpart = (int8_t)((deg_c - ipart) * 10);
		if (fpart < 0) fpart *= -1;
		sprintf(serlcd_buffer[1], "      %3d.%1d C       " , ipart, fpart);
		ipart = (int8_t)deg_f;
		fpart = (int8_t)((deg_f - ipart) * 10);
		if (fpart < 0) fpart *= -1;
		sprintf(serlcd_buffer[2], "      %3d.%1d F       " , ipart, fpart);
		init_spi0_SerLCD();
		update_SerLCD();
		_delay_ms(500);
	}
}

float lm74_to_celsius(uint16_t result) {
	result >>= 3;
	if (result & 0x1000) {
		result |= 0xE000;
	}
	// return 0.0625 * ( *(int16_t *) &result);
	return 0.0625 * (int16_t)result;
}

float c_to_f(float c) {
	return c * 1.8f + 32;
}


void clear_display_buffer() {
	for (int i = 0; i < SERLCD_ROWS; ++i) {
		memset(serlcd_buffer[i], ' ', SERLCD_COLS);
		serlcd_buffer[i][SERLCD_COLS] = '\0';
	}
}

void update_SerLCD() {
	send_byte_SerLCD('|');
	send_byte_SerLCD('-');
	for (int i = 0; i < SERLCD_ROWS; ++i) {
		for (int k = 0; k < SERLCD_COLS; ++k) {
			send_byte_SerLCD(serlcd_buffer[i][k]);
		}
	}
}
