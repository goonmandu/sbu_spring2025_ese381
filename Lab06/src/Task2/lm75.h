#ifndef LM75_H_
#define LM75_H_

#include <stdint.h>

/**
 * Reads the two-byte result from an LM75 temperature sensor.
 *
 * @param addr I2C slave address of the LM75.
 * @return 2 bytes from the LM75.
 */
uint16_t read_LM75_temp_bytes(uint8_t addr);

/**
 * Converts the LM75 raw data (2 bytes) to celsius.
 *
 * @param bytes Two bytes read from the LM75.
 * @return Temperature in celsius.
 */
float lm75_to_celsius(uint16_t bytes);

#endif /* LM75_H_ */