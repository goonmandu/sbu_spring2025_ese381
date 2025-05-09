/**
 * Adapted by Mingi Hwang on 24 Mar 2025 from original source:
 * https://github.com/microchip-pic-avr-examples/avr128db48-bare-metal-twi-mplab/tree/master/twi-host.X/peripherals/TWI
 * 
 * ORIGINAL SOURCE LICENSE STATEMENT
 *
 * (c) 2020 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip software
 * and any derivatives exclusively with Microchip products. You're responsible
 * for complying with 3rd party license terms applicable to your use of 3rd
 * party software (including open source software) that may accompany Microchip
 * software.
 *
 * SOFTWARE IS "AS IS." NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
 * MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
 * HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
 * THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
 * CLAIMS RELATED TO THE SOFTWARE WILL NOT EXCEED AMOUNT OF FEES, IF ANY,
 * YOU PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 */

#ifndef TWI0_MASTER_H
#define	TWI0_MASTER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdbool.h>
#include <stdint.h>

/**
 * Prof. Short does not want us to use internal pull-ups.
 * External 10k PURs will be used.
 * If defined, internal pull-up resistors will be used
 */

// #define TWI_ENABLE_PULLUPS
	
/** 
 * Initializes the TWI peripheral in Host Mode.
 */
void TWI_initHost(void);
    
/**
 * Initializes the I/O used by the TWI peripheral.
 */
void TWI_initPins(void);
    
/**
 * @param addr - Client Device Address
 * @param data - Data to Send
 * 
 * Sends a byte of data to the specified address.
 * Returns true if successful, false if it failed
 */
bool TWI_sendByte(uint8_t addr, uint8_t data);
    
/**
 * @param addr - Client Device Address
 * @param data - Pointer to the data to send
 * @param len - Number of Bytes to Send 
 * 
 * Attempts to send len bytes of data to the client.
 * Returns true if successful, false if it failed
 */
bool TWI_sendBytes(uint8_t addr, uint8_t* data, uint8_t len);

/**
 * @param addr - Client Device Address
 * @param data - Where the byte received should be stored
 *
 * Attempts to read 1 byte of data from the client.
 * Returns true if successful, fails if unable to get data.
 */
bool TWI_readByte(uint8_t addr, uint8_t* data);
    
/**
 * @param addr - Client Device Address
 * @param data - Where the bytes received should be stored
 * @param len - Number of Bytes to Send 
 * 
 * Attempts to read len bytes of data from the client.
 * Returns true if successful, false if it failed to get data.
 */
bool TWI_readBytes(uint8_t addr, uint8_t* data, uint8_t len);
    
/**
 * @param addr - Client Device Address
 * @param regAddress - Address of Register to Read From
 * @param data - Where the bytes received should be stored
 * @param len - Number of Bytes to Read
 * 
 * Writes a single byte (regAddress) to the client, then restarts and attempts to read len of data.
 * Returns true if successful, false if it failed to get data.
 */
bool TWI_sendAndReadBytes(uint8_t addr, uint8_t regAddress, uint8_t* data, uint8_t len, uint8_t wait_between_wr);

/**
 * @param addr Client Device Address
 * @param data_to_send Data to send to client device.
 * @param send_len Length of data to send to client device.
 * @param data_read Where the bytes received should be stored
 * @param read_len - Number of Bytes to Read
 * 
 * Writes a single byte (regAddress) to the client, then restarts and attempts to read len of data.
 * Returns true if successful, false if it failed to get data.
 */
bool TWI_sendBytesAndReadBytes(uint8_t addr, uint8_t *data_to_send, uint8_t send_len, uint8_t *data_read, uint8_t read_len, uint8_t wait_between_wr);
    
#ifdef	__cplusplus
}
#endif

#endif	/* TWI0_MASTER_H */
