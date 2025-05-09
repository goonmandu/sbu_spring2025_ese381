#define F_CPU 4000000

#include <avr/io.h>
#include <util/delay.h>
#include "utils/ascii.h"
#include "usart/usart3.h"

int main(void) {
	USART3_init();
	USART3_serlcd_clear();
	_delay_ms(10);
	
    while (1) {
		for (char c = 'A'; c <= 'Z'; ++c) {
			USART3_send_char(c);
			_delay_ms(2);
		}
    }
}
