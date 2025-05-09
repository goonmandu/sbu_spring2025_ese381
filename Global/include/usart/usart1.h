#ifndef USART1_H_
#define USART1_H_

#include <string.h>

/**
 * Initializes USART1 to 9600 8N1 and sets the pin directions.
 */
void USART1_init(void);

/**
 * Initializes USART1 to the specified baud rate, data bits, and parity bit.<br>
 * Assumes 4 MHz clock.
 *
 * @param baud Baud rate in bps.
 * @param data_bits Number of bits per frame: 5, 6, 7, or 8.
 * @param parity 'N', 'O', or 'E'.
 * @return None
 */
void USART1_init_flex(volatile uint32_t baud, uint8_t data_bits, unsigned char parity);

/**
 * Blockingly reads a byte from USART1 by polling the RXCIF flag.
 *
 * @param None
 * @return Byte read from USART1.
 */
uint8_t USART1_read(void);

/**
 * Blockingly sends a byte out of USART1 by polling the DREIF flag.
 *
 * @param c Byte to be sent.
 * @return None
 */
void USART1_send_char(uint8_t c);

/**
 * Blockingly sends a null-terminated ASCII string out of USART1
 * by calling `USART1_send_char` multiple times.
 *
 * @param buf Pointer to char array to be sent.
 * @return None
 */
void USART1_send_string(const char *buf);

/**
 * Blockingly sends the sequence |- out of USART1 to clear
 * a SparkFun SerLCD.
 *
 * @param None
 * @return None
 */
void USART1_serlcd_clear(void);

/**
 * Blockingly sends the sequence +rgb to control the backlight
 * RGB value on an RGB SparkFun SerLCD.
 *
 * @param r Red. 0 to 255.
 * @param g Green. 0 to 255.
 * @param b Blue. 0 to 255.
 * @return None
 */
void USART1_serlcd_backlight(uint8_t r, uint8_t g, uint8_t b);



#endif /* USART1_H_ */