#include "serlcd.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include <twi/twi0_master.h>

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

void update_SerLCD(uint8_t addr) {
	clear_SerLCD(addr);
	for (uint8_t idx = 0; idx < SERLCD_ROWS; ++idx) {
		send_string_SerLCD(addr, serlcd_buffer[idx], SERLCD_COLS);
	}
}