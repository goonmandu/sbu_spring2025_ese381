#define F_CPU 4000000UL
#define SCD41_ADDR 0x62
#define SERLCD_ADDR 0x72

#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include <string.h>

#include <twi/twi0_master.h>
#include "serlcd.h"
#include "scd41.h"

extern char serlcd_buffer[SERLCD_ROWS][SERLCD_COLS+1];

scd41_data_t scd41_data;

void fill_buffer();
void initializing();
// void TWI_initHost_test(void);

int main() {
	TWI_initHost();
	TWI_initPins();
	
	turn_backlight_off(SERLCD_ADDR);
	// serlcd_set_background_rgb(SERLCD_ADDR, 0xF5, 0xA9, 0xB8);
	initializing();
	update_SerLCD(SERLCD_ADDR);
	
	scd41_start_periodic_meas(SCD41_ADDR);
	_delay_ms(1000);
	
	while (1) {
		while (!scd41_has_data(SCD41_ADDR)) {
			_delay_ms(250);
		}
		scd41_data = scd41_read_data(SCD41_ADDR);
		fill_buffer();
		update_SerLCD(SERLCD_ADDR);
	}
}

/*
void TWI_initHost_test(void) {
	//Standard 100kHz TWI, 4 Cycle Hold, 50ns SDA Hold Time
	TWI0.CTRLA = TWI_SDAHOLD_50NS_gc;
	
	//Clear Dual Control
	TWI0.DUALCTRL = 0x00;
	
	//Enable Run in Debug
	TWI0.DBGCTRL = TWI_DBGRUN_bm;
	
	//Clear MSTATUS (write 1 to flags). BUSSTATE set to idle
	TWI0.MSTATUS = TWI_RIF_bm | TWI_WIF_bm | TWI_CLKHOLD_bm | TWI_RXACK_bm |
	TWI_ARBLOST_bm | TWI_BUSERR_bm | TWI_BUSSTATE_IDLE_gc;
	
	//Set for 100kHz from a 4MHz oscillator
	TWI0.MBAUD = 0;
	
	//[No ISRs] and Host Mode
	TWI0.MCTRLA = TWI_ENABLE_bm;
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

void initializing() {
	sprintf(serlcd_buffer[0], "%-20s", "    SCD41 Output");
	//                        "01234567890123456789"
	sprintf(serlcd_buffer[1], "Waiting for SCD41...");
	sprintf(serlcd_buffer[2], "                    ");
	sprintf(serlcd_buffer[3], "                    ");
}