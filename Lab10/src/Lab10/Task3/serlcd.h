#ifndef SERLCD_H_
#define SERLCD_H_

#ifndef F_CPU
	#warning "F_CPU is not defined! Defaulting to 4 MHz."
	#define F_CPU 4000000UL
#endif /* F_CPU is not defined */

#define TWI_SERLCD_INTERVAL_US 150
#define SERLCD_ROWS 4
#define SERLCD_COLS 20

#include <stdint.h>

/**
 * The 4x20 character buffer for the SerLCD.
 */
char serlcd_buffer[SERLCD_ROWS][SERLCD_COLS+1];

/**
 * Sends char array to the SerLCD out of TWI0.
 *
 * @param addr I2C slave address of the SerLCD.
 * @param str Pointer to start of char array to be sent.
 * @param len Length of char array.
 * @return None
 */
void send_string_SerLCD(uint8_t addr, uint8_t *str, uint8_t len);

/**
 * Sends the byte sequence |- out of TWI0 to clear the SerLCD.
 *
 * @param addr I2C slave address of the SerLCD.
 * @return None
 */
void clear_SerLCD(uint8_t addr);

/**
 * Sends contents of `serlcd_buffer` out of TWI0.
 *
 * @param addr I2C slave address of the SerLCD.
 * @return None
 */
void update_SerLCD(uint8_t addr);

/**
 * Sends byte sequence to turn off the SerLCD backlight.
 *
 * @param addr I2C slave address of the SerLCD.
 * @return None
 */
void turn_backlight_off(uint8_t addr);

/**
 * Sets the RGB background color of the SerLCD backlight.
 *
 * @param addr I2C slave address of the SerLCD.
 * @param r Red intensity (0–255).
 * @param g Green intensity (0–255).
 * @param b Blue intensity (0–255).
 * @return None
 */
void serlcd_set_background_rgb(uint8_t addr, uint8_t r, uint8_t g, uint8_t b);


#endif /* SERLCD_H_ */