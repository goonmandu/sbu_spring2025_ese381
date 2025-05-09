#define SERLCD_ROWS 4
#define SERLCD_COLS 20
#define LM74_BITS 16
#define F_CPU 4000000

#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <spi/spi0.h>


void clear_display_buffer();
void update_SerLCD();
void u16_to_padded_bin(uint16_t num, char *buf, uint8_t terminate);

char serlcd_buffer[SERLCD_ROWS][SERLCD_COLS+1];
char lm74_bin[LM74_BITS+1];

int main() {
	volatile uint16_t lm74_result;
	
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
	
	sprintf(serlcd_buffer[0], "LM74 Binary Output  ");
	update_SerLCD();
	
	while (1) {
		init_spi0_LM74();
		lm74_result = read_spi0_LM74();
		u16_to_padded_bin(lm74_result, lm74_bin, 1);
		sprintf(serlcd_buffer[1], "%s    ", lm74_bin);
		init_spi0_SerLCD();
		update_SerLCD();
		_delay_ms(1000);
	}
}

void u16_to_padded_bin(uint16_t num, char *buf, uint8_t terminate) {
	for (int i = 0; i < 16; ++i) {
		buf[i] = (num & (1 << (15 - i))) ? '1' : '0';
	}
	if (terminate) buf[16] = '\0';
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