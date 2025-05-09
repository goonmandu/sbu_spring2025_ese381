/**
 * TO THE PRELAB GRADER:
 *
 * This program is intended to be the solution code for both Task 2 and Task 3
 * as Task 3 is simply an extension of Task 2 with none of the Task 2 components
 * modified.
 */

#define SERLCD_ADDR 0x72
#define LM75_ADDR 0x48
#define F_CPU 4000000UL

#include <util/delay.h>
#include <stdio.h>

// #define TWI_ENABLE_PULLUPS
#include <twi/twi0_master.h>
#include "serlcd.h"
#include "lm75.h"

void fill_buffer();
float c_to_f(float c);
extern unsigned char serlcd_buffer[SERLCD_ROWS][SERLCD_COLS+1];

uint16_t lm75_bytes;

int main(void) {
	float deg_c, deg_f;
	int8_t ipart, fpart;
	
	TWI_initHost();
	TWI_initPins();
	
	clear_SerLCD(SERLCD_ADDR);
	fill_buffer();
	
	while (1) {
		lm75_bytes = read_LM75_temp_bytes(LM75_ADDR);
		deg_c = lm75_to_celsius(lm75_bytes);
		
		deg_f = c_to_f(deg_c);
		ipart = (int8_t)deg_c;
		fpart = (int8_t)((deg_c - ipart) * 10);
		if (fpart < 0) fpart *= -1;
		sprintf(serlcd_buffer[1], "      %3d.%1d C       " , ipart, fpart);
		ipart = (int8_t)deg_f;
		fpart = (int8_t)((deg_f - ipart) * 10);
		if (fpart < 0) fpart *= -1;
		sprintf(serlcd_buffer[2], "      %3d.%1d F       " , ipart, fpart);
		update_SerLCD(SERLCD_ADDR);
		_delay_ms(500);
	}
}

/**
 * Fills `serlcd_buffer` with Lab 07 template.
 *
 * @param None
 * @return None
 */
void fill_buffer() {
	//                         01234567890123456789
	sprintf(serlcd_buffer[0], "  LM75 Temperature  ");
	sprintf(serlcd_buffer[1], "                    ");
	sprintf(serlcd_buffer[2], "                    ");
	sprintf(serlcd_buffer[3], "                    ");
}

/**
 * Converts celsius to fahrenheit.
 *
 * @param c Celsius.
 * @return Fahrenheit.
 */
float c_to_f(float c) {
	return c * 1.8f + 32;
}