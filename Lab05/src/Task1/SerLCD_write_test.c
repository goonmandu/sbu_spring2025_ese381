#include <avr/io.h>

#include <string.h>

#include <spi/spi0.h>


int main() {
	PORTMUX.SPIROUTEA = PORTMUX_SPI0_DEFAULT_gc;
	PORTA.DIR = 0xD8;	/* PA7   PA6   PA5   PA4   PA3  */
						/* LCD   SCK   MISO  MOSI  LM74 */
						/* OUT   OUT   IN    OUT   OUT  */
						/*  1     1     0     1     1   */
	PORTA_OUTSET = 0x88;
	
	init_spi0();
    init_spi0_SerLCD();
	//                  0         1         2         3         4         5         6         7         
	//                  01234567890123456789012345678901234567890123456789012345678901234567890123456789
	const char *oppa = "Been fishing all my life for the catch, a Xiangling paradise";
	
	send_byte_SerLCD('|');
	send_byte_SerLCD('-');

	for (int i = 0; i < strlen(oppa); ++i) {
		send_byte_SerLCD(oppa[i]);
	}
	
    while (1) {
		
    }
}
