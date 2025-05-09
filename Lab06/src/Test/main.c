#define F_CPU 4000000
#define SSD1306_ADDR 0x3C

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include <twi/twi0_master.h>

const uint8_t MUXRATIO[2] = {0xA8, 0x3F};
const uint8_t DISPLAYOFFSET[2] = {0xD3, 0x00};
const uint8_t DISPLAYSTARTLINE = 0x40;
const uint8_t SEGMENTREMAP = 0xA1;
const uint8_t COMOSD = 0xC0;
const uint8_t COMPINHWCONF[2] = {0xDA, 0x02};
const uint8_t CONTRASTCONTROL[2] = {0x81, 0x7F};
const uint8_t ENTIREDISPLAYON = 0xA4;
const uint8_t NORMALDISPLAY = 0xA7;
const uint8_t OSCFREQ[2] = {0xD5, 0x80};
const uint8_t CHGPUMPREG[2] = {0x8D, 0x10};
const uint8_t DISPLAYON = 0xAF;

int main(void) {
	TWI_initHost();
	TWI_initPins();
		
    while (1) {
		TWI_sendBytes(SSD1306_ADDR, MUXRATIO, 2);
		// TWI_sendBytes(SSD1306_ADDR, DISPLAYOFFSET, 2);
		// TWI_sendByte(SSD1306_ADDR, DISPLAYSTARTLINE);
		// TWI_sendByte(SSD1306_ADDR, SEGMENTREMAP);
		// TWI_sendByte(SSD1306_ADDR, COMOSD);
		// TWI_sendBytes(SSD1306_ADDR, COMPINHWCONF, 2);
		TWI_sendBytes(SSD1306_ADDR, CONTRASTCONTROL, 2);
		TWI_sendByte(SSD1306_ADDR, ENTIREDISPLAYON);
		TWI_sendByte(SSD1306_ADDR, NORMALDISPLAY);
		
		
		// TWI_sendBytes(SSD1306_ADDR, OSCFREQ, 2);
		
		// Disable Charge Pump Regulator
		// TWI_sendBytes(SSD1306_ADDR, CHGPUMPREG, 2);
		
		// Turn Display On
		TWI_sendByte(SSD1306_ADDR, DISPLAYON);
		_delay_ms(100);
    }
}
