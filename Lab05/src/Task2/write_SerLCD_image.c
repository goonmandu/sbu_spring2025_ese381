#define F_CPU 4000000
#define SERLCD_ROWS 4
#define SERLCD_COLS 20

#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>

#include <spi/spi0.h>


void clear_display_buffer();
void update_SerLCD();
void write_c4();
void c4_planted();
void c4_timer(uint8_t seconds);
void c4_explosion();

char serlcd_buffer[SERLCD_ROWS][SERLCD_COLS+1];

int main() {
	uint8_t seconds = 40;
	
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
	
	_delay_ms(1000);
	write_c4();
	update_SerLCD();
	
	_delay_ms(500);
	sprintf(serlcd_buffer[1], "#  * * * * * * 7   #");
	update_SerLCD();
	_delay_ms(500);
	sprintf(serlcd_buffer[1], "#  * * * * * 7 3   #");
	update_SerLCD();
	_delay_ms(300);
	sprintf(serlcd_buffer[1], "#  * * * * 7 3 5   #");
	update_SerLCD();
	_delay_ms(300);
	sprintf(serlcd_buffer[1], "#  * * * 7 3 5 5   #");
	update_SerLCD();
	_delay_ms(300);
	sprintf(serlcd_buffer[1], "#  * * 7 3 5 5 6   #");
	update_SerLCD();
	_delay_ms(500);
	sprintf(serlcd_buffer[1], "#  * 7 3 5 5 6 0   #");
	update_SerLCD();
	_delay_ms(300);
	sprintf(serlcd_buffer[1], "#  7 3 5 5 6 0 8   #");
	update_SerLCD();
	_delay_ms(100);
	c4_planted();
	update_SerLCD();
	_delay_ms(1000);
		
	while (seconds != 0) {
		c4_timer(seconds--);
		update_SerLCD();
		_delay_ms(1000);
	}
	
	c4_explosion();
	update_SerLCD();
	
	while (1) { }
}

void write_c4() {
	sprintf(serlcd_buffer[0], "####################");
	sprintf(serlcd_buffer[1], "#                  #");
	sprintf(serlcd_buffer[2], "#                  #");
	sprintf(serlcd_buffer[3], "####################");
}

void c4_planted() {
	sprintf(serlcd_buffer[2], "#   Bomb planted   #");
}

void c4_timer(uint8_t seconds) {
	sprintf(serlcd_buffer[2], "#       %d:%02d       #", seconds / 60, seconds % 60);
}

void c4_explosion() {
	sprintf(serlcd_buffer[2], "#  Terrorists Win  #");
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