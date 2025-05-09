/**
 * @file scd41.h
 * @brief Interface for interacting with the SCD41 CO? sensor via I2C.
 *
 * This module provides functions to start and stop periodic measurements,
 * check data availability, and read CO?, temperature, and humidity data
 * from the SCD41 sensor using the TWI0 interface.
 */

#ifndef SCD41_H_
#define SCD41_H_

/**
 * @struct scd41_data_t
 * @brief Container for raw and converted SCD41 sensor data.
 *
 * Holds raw 16-bit values and their CRCs as received from the sensor,
 * along with floating-point temperature in °C and relative humidity in %.
 */
typedef struct {
	uint16_t co2_raw;     /**< Raw CO? value from sensor */
	uint8_t co2_crc;      /**< CRC for raw CO? value */
	uint16_t temp_c_raw;  /**< Raw temperature value from sensor */
	uint8_t temp_c_crc;   /**< CRC for raw temperature value */
	uint16_t rh_raw;      /**< Raw relative humidity value from sensor */
	uint8_t rh_crc;       /**< CRC for raw relative humidity value */
	float temp_c;         /**< Calculated temperature in degrees Celsius */
	float rh;             /**< Calculated relative humidity in percent */
} scd41_data_t;

/**
 * Sends command to start periodic measurements on the SCD41.
 *
 * @param addr I2C address of the SCD41 sensor.
 * @return None
 */
void scd41_start_periodic_meas(uint8_t addr);

/**
 * Sends command to stop periodic measurements on the SCD41.
 *
 * @param addr I2C address of the SCD41 sensor.
 * @return None
 */
void scd41_stop_periodic_meas(uint8_t addr);

/**
 * Checks if new measurement data is available from the SCD41.
 *
 * @param addr I2C address of the SCD41 sensor.
 * @return 1 if data is ready, 0 otherwise.
 */
uint8_t scd41_has_data(uint8_t addr);

/**
 * Reads the latest measurement from the SCD41.
 *
 * @param addr I2C address of the SCD41 sensor.
 * @return scd41_data_t struct containing raw values, CRCs, and
 *         converted temperature and humidity.
 */
scd41_data_t scd41_read_data(uint8_t addr);

#endif /* SCD41_H_ */
