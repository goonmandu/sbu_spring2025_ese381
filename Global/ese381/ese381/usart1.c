#include <avr/io.h>
#include "usart1.h"

void USART1_init() {
	USART1.BAUD = 1667;
	USART1.CTRLC = 0x03;
	USART1.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
	VPORTB.DIR = 0x01;
}

void USART1_init_flex(uint32_t baud, uint8_t data_bits, unsigned char parity) {
	uint8_t CTRLC = 0x00;
	CTRLC |= (data_bits - 5);
	if (parity == 'O') {
		CTRLC |= 0x03 << 4;
	} else if (parity == 'E') {
		CTRLC |= 0x02 << 4;
	}
	USART1.CTRLC = CTRLC;
	USART1.BAUD = (uint16_t)((64ULL * 4000000ULL) / (16ULL * baud));
	PORTC_DIRSET = 0x01;
	PORTC_DIRCLR = 0x02;
	USART1.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}

uint8_t USART1_read() {
	while (!(USART1.STATUS & USART_RXCIF_bm));  // wait for data to be fully received
	return USART1.RXDATAL;
}

void USART1_send_char(uint8_t c) {
	while (!(USART1.STATUS & USART_DREIF_bm));  // wait for prev data to be fully sent
	USART1.TXDATAL = c;
}

void USART1_send_string(const char *buf) {
	for (int i = 0; i < strlen(buf); ++i) {
		USART1_send_char(buf[i]);
	}
}

void USART1_serlcd_clear() {
	USART1_send_char('|');
	USART1_send_char('-');
}

void USART1_serlcd_backlight(uint8_t r, uint8_t g, uint8_t b) {
	USART1_send_char('+');
	USART1_send_char(r);
	USART1_send_char(g);
	USART1_send_char(b);
}
