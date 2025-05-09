#define F_CPU 4000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <usart/usart3.h>
#include <utils/buffer.h>

volatile uint8_t rxd_byte;
volatile uint8_t byte_to_tx;
volatile circular_buffer_t rx_buf;
volatile circular_buffer_t tx_buf;

int main(void) {
	USART3.DBGCTRL = 0x01;
	USART3.CTRLA = 0xA0;
	USART3_init_flex(9600, 8, 'N');
	
	PORTB.PIN2CTRL = 0x08;
	
	sei();
	
	while (1) {
		if (buffer_dequeue(&rx_buf, &rxd_byte)) {
			buffer_enqueue(&tx_buf, rxd_byte);
		}
	}
}

ISR(USART3_RXC_vect) {
	cli();
	buffer_enqueue(&rx_buf, USART3.RXDATAL);
	sei();
}

ISR(USART3_DRE_vect) {
	cli();
	// Uncomment the line below to enable conditional echo
	if (!(PORTB.IN & PIN2_bm))
	if (buffer_dequeue(&tx_buf, &byte_to_tx))
		USART3.TXDATAL = byte_to_tx;
	sei();
}