#include <twi/twi0_master.h>
#include "lm75.h"

uint16_t read_LM75_temp_bytes(uint8_t addr) {
	uint8_t temp_bytes[2];
	TWI_readBytes(addr, temp_bytes, 2);
	return ((temp_bytes[0] << 8) | (temp_bytes[1])) & 0xFF80;
}

float lm75_to_celsius(uint16_t bytes) {
	bytes >>= 7;
	if (bytes & 0x0100) {
		bytes |= 0xFE00;
	}
	return 0.5 * (int16_t)bytes;
}