#ifndef SPI0_H_
#define SPI0_H_

/**
 * Initializes SPI0 to master-only mode.
 *
 * @param None
 * @return None
 */
void init_spi0(void);

/**
 * Initializes SPI0 to be compatible with the SerLCD.
 *
 * @param None
 * @return None
 */
void init_spi0_SerLCD(void);

/**
 * Initializes SPI0 to be compatible with the LM74.
 *
 * @param None
 * @return None
 */
void init_spi0_LM74(void);

/**
 * Reads two bytes from an LM74 via SPI0 and returns the 16-bit response.
 *
 * @param None
 * @return LM74 output.
 */
uint16_t read_spi0_LM74(void);

/**
 * Sends a byte out of SPI0. Assumes /SS = PA7.
 *
 * @param data Byte to be sent.
 * @return None
 */
void send_byte_SerLCD(uint8_t data);

/**
 * Pulls PA3 low.
 *
 * @param None
 * @return None
 */
void select_slave_SerLCD();

/**
 * Pulls PA3 high.
 *
 * @param None
 * @return None
 */
void deselect_slave_SerLCD();

/**
 * Pulls PA7 low.
 *
 * @param None
 * @return None
 */
void select_slave_LM74();

/**
 * Pulls PA7 high.
 *
 * @param None
 * @return None
 */
void deselect_slave_LM74();

/**
 * Sends and receives one byte from SPI0.
 * Does not automatically select/deselect slave device.
 *
 * @param data Data to be sent.
 * @return Data in SPI0.DATA after exchange.
 */
uint8_t exchange_byte(uint8_t data);

/**
 * (This function should not be here, but I am too lazy. Whatever.)
 * Normalizes a uint8_t to another range.
 *
 * @param from_min Minimum value of original range.
 * @param from_val Value to normalize.
 * @param from_max Maximum value of original range.
 * @param result_min Minimum value of normalized range.
 * @param result_max Maximum value of normalized range.
 * @return Normalized number.
 */
uint8_t normalize_range(uint8_t from_min, uint8_t from_val, uint8_t from_max, uint8_t result_min, uint8_t result_max);

/**
 * Normalizes r, g, b to be between 0-29 (inclusive) and sends the sequence
 * |r|g|b
 *
 * @param r Red. 0 to 255.
 * @param g Green. 0 to 255.
 * @param b Blue. 0 to 255.
 * @return None
 */
void set_backlight_rgb(uint8_t r, uint8_t g, uint8_t b);

#endif /* SPI0_H_ */