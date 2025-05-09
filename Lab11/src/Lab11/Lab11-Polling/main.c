#define F_CPU 4000000UL
#define SCD41_ADDR 0x62
#define SERLCD_ADDR 0x72
#define SERLCD_ROWS 4
#define SERLCD_COLS 20

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <string.h>

#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"

char serlcd_buffer[SERLCD_ROWS][SERLCD_COLS+1];

const char clear_lcd[2] = {'|', '-'};
const char backlight_lcd[5] = {'|', '+', 0x5B, 0xCE, 0xFA};

void fill_buffer();
void update_SerLCD(uint8_t addr);

typedef struct {
	uint16_t co2_raw;     /**< Raw CO? value from sensor */
	uint8_t co2_crc;      /**< CRC for raw CO2 value */
	uint16_t temp_c_raw;  /**< Raw temperature value from sensor */
	uint8_t temp_c_crc;   /**< CRC for raw temperature value */
	uint16_t rh_raw;      /**< Raw relative humidity value from sensor */
	uint8_t rh_crc;       /**< CRC for raw relative humidity value */
	float temp_c;         /**< Calculated temperature in degrees Celsius */
	float rh;             /**< Calculated relative humidity in percent */
} scd41_data_t;

scd41_data_t scd41_data;

volatile uint8_t poll_scd41;

int main(void) {
	poll_scd41 = 0;
	int16_t error = NO_ERROR;
	bool data_ready = false;
	uint16_t co2_concentration = 0;
	int32_t temperature = 0;
	int32_t relative_humidity = 0;
	
	// wait for MCU/I2C device voltages to stabilize
	_delay_ms(2000);
	
	// Initialize TWI0
	sensirion_i2c_hal_init();
	
	// Initialize Timer/Counter A
	// Setup
	// TCA0.SINGLE.CTRLD = 0x00;
	// TCA0.SINGLE.CTRLA |= TCA_SINGLE_CLKSEL_DIV1024_gc;
	// TCA0.SINGLE.CTRLB |= TCA_SINGLE_WGMODE_FRQ_gc | TCA_SINGLE_CMP0EN_bm;
	// TCA0.SINGLE.CMP0 = 3900;
	// Enable CMP0 interrupt and the module itself
	// TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP0_bm;
	// TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
	
	// Enable global interrupts
	sei();
	
    scd4x_start_periodic_measurement();
	sensirion_i2c_hal_write(SERLCD_ADDR, backlight_lcd, 5);
	
	while (1) {
		poll_scd41 = 1;
		_delay_ms(200);
		if (poll_scd41) {
			// TCA0.SINGLE.INTCTRL &= ~TCA_SINGLE_CMP0_bm;
			poll_scd41 = 0;
			if (scd4x_get_data_ready_flag(&data_ready) != NO_ERROR) continue;
			if (data_ready) {
				if (scd4x_read_measurement(&co2_concentration,
				                           &temperature,
										   &relative_humidity) != NO_ERROR) {
					continue;	   
				}
		
				scd41_data.co2_raw = co2_concentration;
				scd41_data.temp_c = temperature / 1000.0f;
				scd41_data.rh = relative_humidity / 1000.0f;
		
				fill_buffer();
				update_SerLCD(SERLCD_ADDR);
			}
			// TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP0_bm;
		}
	}
}

/*
ISR(TCA0_CMP0_vect) {
	cli();
	poll_scd41 = 1;
	sei();
}
 */

void fill_buffer() {
	int8_t ipart, fpart;
	sprintf(serlcd_buffer[0], "%-20s", "    SCD41 Output");
	
	ipart = (int8_t)scd41_data.temp_c;
	fpart = (int8_t)((scd41_data.temp_c - ipart) * 10);
	if (fpart < 0) fpart *= -1;
	sprintf(serlcd_buffer[1], "%-20s", (char[21]){0});
	snprintf(serlcd_buffer[1], 21, "Temp:   %2d.%1d C", ipart, fpart);
	for (int i = strlen(serlcd_buffer[1]); i < 20; i++) serlcd_buffer[1][i] = ' ';
	serlcd_buffer[1][20] = '\0';

	ipart = scd41_data.rh;
	sprintf(serlcd_buffer[2], "%-20s", (char[21]){0});
	snprintf(serlcd_buffer[2], 21, "RelHum: %d %%", ipart);
	for (int i = strlen(serlcd_buffer[2]); i < 20; i++) serlcd_buffer[2][i] = ' ';
	serlcd_buffer[2][20] = '\0';

	sprintf(serlcd_buffer[3], "%-20s", (char[21]){0});
	snprintf(serlcd_buffer[3], 21, "CO2:    %d ppm", scd41_data.co2_raw);
	for (int i = strlen(serlcd_buffer[3]); i < 20; i++) serlcd_buffer[3][i] = ' ';
	serlcd_buffer[3][20] = '\0';
}

void update_SerLCD(uint8_t addr) {
	sensirion_i2c_hal_write(addr, clear_lcd, 2);
	for (uint8_t idx = 0; idx < SERLCD_ROWS; ++idx) {
		sensirion_i2c_hal_write(addr, serlcd_buffer[idx], SERLCD_COLS);
	}
}