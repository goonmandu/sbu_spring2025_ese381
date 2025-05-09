#include <avr/io.h>
#include "usart/usart3.h"
#include "utils/ascii.h"

int main(void) {
	
	USART3_init();
	
	uint8_t received;
	uint8_t to_send;

    while (1) {
		received = USART3_read();
		to_send = complement_case(received);
		USART3_send_char(to_send);
    }
}
