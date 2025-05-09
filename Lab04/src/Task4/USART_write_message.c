#include <avr/io.h>
#include "utils/ascii.h"
#include "usart/usart3.h"


int main(void) {
	USART3_init();
	
	USART3_serlcd_clear();
	USART3_send_string("Hello World         ");
	USART3_send_string("What happens now");
	
	while (1) {
		
	}
}
