#define F_CPU 4000000
#define TWI_SERLCD_INTERVAL_US 150
#define SERLCD_ADDR 0x72
#define SERLCD_ROWS 4
#define SERLCD_COLS 20
#define LM75_ADDR 0x48

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include <stdio.h>

#include <twi/twi0_master.h>

void send_string_SerLCD(uint8_t addr, uint8_t *str, uint8_t len);
void clear_SerLCD(uint8_t addr);
void xiangling();
void update_SerLCD(uint8_t addr);

unsigned char serlcd_buffer[SERLCD_ROWS][SERLCD_COLS+1];

int main(void) {
	TWI_initHost();
	TWI_initPins();
	
	clear_SerLCD(SERLCD_ADDR);
	xiangling();
	update_SerLCD(SERLCD_ADDR);
	
	while (1) {

	}
}

void send_string_SerLCD(uint8_t addr, uint8_t *str, uint8_t len) {
	for (uint8_t idx = 0; idx < len; ++idx) {
		TWI_sendByte(addr, str[idx]);
		_delay_us(TWI_SERLCD_INTERVAL_US);
	}
}

void clear_SerLCD(uint8_t addr) {
	TWI_sendByte(addr, '|');
	_delay_us(TWI_SERLCD_INTERVAL_US);
	TWI_sendByte(addr, '-');
	_delay_us(TWI_SERLCD_INTERVAL_US);
}

void xiangling() {
	//                         01234567890123456789
	sprintf(serlcd_buffer[0], "Raiden E, Chevy Q E,");
	sprintf(serlcd_buffer[1], "Bennett Q E,        ");
	sprintf(serlcd_buffer[2], "Xiangling E Q,      ");
	sprintf(serlcd_buffer[3], "Raiden Q 3(N3C) N1C ");
}

void update_SerLCD(uint8_t addr) {
	for (uint8_t idx = 0; idx < SERLCD_ROWS; ++idx) {
		send_string_SerLCD(addr, serlcd_buffer[idx], SERLCD_COLS);
	}
}
