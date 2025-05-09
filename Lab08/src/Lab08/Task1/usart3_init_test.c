#define F_CPU 4000000

#include <avr/io.h>
#include <util/delay.h>

#include <usart/usart3.h>

int main(void) {
	USART3.DBGCTRL = 0x01;
	USART3_init_flex(9600, 8, 'N');
    while (1) {
		USART3_send_char('H');
		_delay_ms(500);
    }
}