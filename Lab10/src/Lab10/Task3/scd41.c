#include <twi/twi0_master.h>
#include "scd41.h"

static const uint8_t start_meas[2] = {0x21, 0xB1};
static const uint8_t read_meas[2]  = {0xEC, 0x05};
static const uint8_t stop_meas[2]  = {0x3F, 0x86};
static const uint8_t data_ready[2] = {0xE4, 0xB8};
	
void scd41_start_periodic_meas(uint8_t addr) {
	TWI_sendBytes(addr, start_meas, 2);
}

void scd41_stop_periodic_meas(uint8_t addr) {
	TWI_sendBytes(addr, stop_meas, 2);
}

uint8_t scd41_has_data(uint8_t addr) {
	uint8_t scd41_data_ready_response[3];
	TWI_sendBytesAndReadBytes(addr, data_ready, 2, scd41_data_ready_response, 3, 1);
	//                               0bXXXXX000  and 0b00000000  =  DATA NOT READY
	return (scd41_data_ready_response[0] & 0x07) || (scd41_data_ready_response[1]);
}

scd41_data_t scd41_read_data(uint8_t addr) {
	scd41_data_t ret;
	uint8_t scd41_response[9];
	TWI_sendBytesAndReadBytes(addr, read_meas, 2, scd41_response, 9, 1);
	
	ret.co2_raw = scd41_response[0] << 8 | scd41_response[1];
	ret.co2_crc = scd41_response[2];
	ret.temp_c_raw = scd41_response[3] << 8 | scd41_response[4];
	ret.temp_c_crc = scd41_response[5];
	ret.rh_raw = scd41_response[6] << 8 | scd41_response[7];
	ret.rh_crc = scd41_response[8];
	
	ret.temp_c = -45.0f + 175.0f * ret.temp_c_raw / 65536.0f;
	ret.rh = 100.0f * ret.rh_raw / 65536.0f;
	
	return ret;
}