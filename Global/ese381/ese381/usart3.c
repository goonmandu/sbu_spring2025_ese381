#include <avr/io.h>
#include "usart3.h"

void USART3_init() {
	USART3.BAUD = 1667;
	USART3.CTRLC = 0x03;
	USART3.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
	VPORTB.DIR = 0x01;
}

void USART3_init_flex(uint32_t baud, uint8_t data_bits, unsigned char parity) {
	uint8_t CTRLC = 0x00;
	CTRLC |= (data_bits - 5);
	if (parity == 'O') {
		CTRLC |= 0x03 << 4;
	} else if (parity == 'E') {
		CTRLC |= 0x02 << 4;
	}
	USART3.CTRLC = CTRLC;
	USART3.BAUD = (uint16_t)((64ULL * 4000000ULL) / (16ULL * baud));
	PORTB_DIRSET = 0x01;
	PORTB_DIRCLR = 0x02;
	USART3.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}

uint8_t USART3_read() {
	while (!(USART3.STATUS & USART_RXCIF_bm));  // wait for data to be fully received
	return USART3.RXDATAL;
}

void USART3_send_char(uint8_t c) {
	while (!(USART3.STATUS & USART_DREIF_bm));  // wait for prev data to be fully sent
	USART3.TXDATAL = c;
}

void USART3_send_string(const char *buf) {
	for (int i = 0; i < strlen(buf); ++i) {
		USART3_send_char(buf[i]);
	}
}

void USART3_serlcd_clear() {
	USART3_send_char('|');
	USART3_send_char('-');
}

void USART3_serlcd_backlight(uint8_t r, uint8_t g, uint8_t b) {
	USART3_send_char('+');
	USART3_send_char(r);
	USART3_send_char(g);
	USART3_send_char(b);
}
