#include <avr/io.h>
#include "spi0.h"


void init_spi0(void) {
	SPI0.CTRLB = (0 << SPI_BUFEN_bp) | (0 << SPI_BUFWR_bp) | (1 << SPI_SSD_bp) | SPI_MODE_0_gc;
	SPI0.CTRLA = (0 << SPI_DORD_bp) | (1 << SPI_MASTER_bp) | (0 << SPI_CLK2X_bp) | SPI_PRESC_DIV4_gc;
	SPI0.CTRLA |= (1 << SPI_ENABLE_bp);
	SPI0.DATA = 0x00;
	while (!(SPI0.INTFLAGS & SPI_IF_bm)) { }
}

void init_spi0_SerLCD(void) {
	SPI0.CTRLB |= SPI_MODE_0_gc;
	SPI0.CTRLA |= (0 << SPI_DORD_bp) | (0 << SPI_CLK2X_bp) | SPI_PRESC_DIV16_gc;
}

void init_spi0_LM74(void) {
	SPI0.CTRLB |= SPI_MODE_0_gc;
	SPI0.CTRLA |= (0 << SPI_DORD_bp) | (0 << SPI_CLK2X_bp) | SPI_PRESC_DIV4_gc;
}

uint16_t read_spi0_LM74() {
	uint16_t ret;
	uint8_t byte_read;
	
	select_slave_LM74();
	byte_read = exchange_byte(0x00);
	deselect_slave_LM74();
	ret = byte_read << 8;                     // put MSB in return variable

	select_slave_LM74();
	byte_read = exchange_byte(0x00);
	deselect_slave_LM74();
	ret |= byte_read;                         // put LSB in return variable
	
	return ret;
}

void send_byte_SerLCD(uint8_t data) {
	select_slave_SerLCD();
	exchange_byte(data);
	deselect_slave_SerLCD();
}

void select_slave_SerLCD() {
	PORTA_OUTCLR = 0x80;
}

void deselect_slave_SerLCD() {
	PORTA_OUTSET = 0x80;
}

void select_slave_LM74() {
	PORTA.OUTCLR = 0x08;
}

void deselect_slave_LM74() {
	PORTA_OUTSET = 0x08;
}

uint8_t exchange_byte(uint8_t data) {
	
	SPI0.DATA = data;
	while (!(SPI0.INTFLAGS & SPI_IF_bm)) { }
	return SPI0.DATA;
}

uint8_t normalize_range(uint8_t from_min, uint8_t from_val, uint8_t from_max, uint8_t result_min, uint8_t result_max) {
	if (from_max == from_min) {
		return result_min;  // Avoid division by zero
	}
	uint16_t numerator = (uint16_t)(from_val - from_min) * (result_max - result_min);
	uint16_t denominator = (uint16_t)(from_max - from_min);
	return result_min + (numerator / denominator);
}

void set_backlight_rgb(uint8_t r, uint8_t g, uint8_t b) {
	r = normalize_range(0, r, 255, 128, 157);
	g = normalize_range(0, g, 255, 158, 187);
	b = normalize_range(0, b, 255, 188, 217);

	send_byte_SerLCD('|');
	send_byte_SerLCD(r);

	send_byte_SerLCD('|');
	send_byte_SerLCD(g);

	send_byte_SerLCD('|');
	send_byte_SerLCD(b);
}