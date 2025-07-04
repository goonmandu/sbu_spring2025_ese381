/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// Sensirion AG's copyright notice applies to the following portions:
// - Instructions
// - Function signatures
// - Function docstrings
// - Local include statements

/**
 * Copyright 2025 Mingi Hwang

 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the �Software�),
 * to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Mingi Hwang's copyright notice applies to the following portions:
// - Function implementations
// - Comments within function implementations
// - ...and everything else explicitly not mentioned in the Sensirion
//    AG copyright notice portion.

/**************************************************************************
 *                                                                        *
 *  ####   ###      #   #  ###  #####     #####  ###  #    #  #### #   #  *
 *  #   # #   #     ##  # #   #   #         #   #   # #    # #     #   #  *
 *  #   # #   #     # # # #   #   #         #   #   # #    # #     #####  *
 *  #   # #   #     #  ## #   #   #         #   #   # #    # #     #   #  *
 *  ####   ###      #   #  ###    #         #    ###   ###    #### #   #  *
 *                                                                        *
 *          ##### #   # #  ####      ####  ###  ####  ##### # #           *
 *            #   #   # # #         #     #   # #   # #     # #           *
 *            #   ##### #  ###      #     #   # #   # ####  # #           *
 *            #   #   # #     #     #     #   # #   # #                   *
 *            #   #   # # ####       ####  ###  ####  ##### # #           *
 *                                                                        *
 *              ...unless you know what you're getting into.              *
 **************************************************************************/
 
/**
 * Authored by Mingi Hwang, as part of ESE 381.
 *
 * -- Target MCU & Configuration --
 *   AVR128DB48 @ 4 MHz
 *   TWI0 with default PORTMUX settings (PA2 and PA3)
 * 
 * -- Changelog --
 * 30 April 2025:
 *   Initial version with no STOP condition bug
 *
 * -- Description --
 * This implementation uses TWI0 Master-mode interrupt flags
 * via polling to determine the byte exchange timing. For some
 * reason, in full-speed operation, the bus is pulled low after
 * a write operation even though all control paths execute a
 * STOP condition.
 */
 

#define F_CPU 4000000UL
#define SLAVE_NACK 1
#define LOST_ARBITRATION 2
#define BUS_ERROR 3

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "sensirion_i2c_hal.h"
#include "sensirion_common.h"
#include "sensirion_config.h"


/*
 * INSTRUCTIONS
 * ============
 *
 * Implement all functions where they are marked as IMPLEMENT.
 * Follow the function specification in the comments.
 */

/**
 * Select the current i2c bus by index.
 * All following i2c operations will be directed at that bus.
 *
 * THE IMPLEMENTATION IS OPTIONAL ON SINGLE-BUS SETUPS (all sensors on the same
 * bus)
 *
 * @param bus_idx   Bus index to select
 * @returns         0 on success, an error code otherwise
 */
int16_t sensirion_i2c_hal_select_bus(uint8_t bus_idx) {
    /**
	 * Not implemented for AVR128DB48
     */
    return NOT_IMPLEMENTED_ERROR;
}

/**
 * Initialize all hard- and software components that are needed for the I2C
 * communication.
 */
void sensirion_i2c_hal_init(void) {
	/*****************************
	 * TWI0 Module Configuration *
	 *****************************/
	
    //Standard 100kHz TWI, 4 Cycle Hold, 50ns SDA Hold Time
    TWI0.CTRLA = TWI_SDAHOLD_50NS_gc;
    
    //Clear Dual Control
    TWI0.DUALCTRL = 0x00;
    
    //Enable Run in Debug
    TWI0.DBGCTRL = TWI_DBGRUN_bm;
    
    //Clear MSTATUS (write 1 to flags). BUSSTATE set to idle
    TWI0.MSTATUS = TWI_RIF_bm | TWI_WIF_bm | TWI_CLKHOLD_bm | TWI_RXACK_bm |
				   TWI_ARBLOST_bm | TWI_BUSERR_bm | TWI_BUSSTATE_IDLE_gc;
    
    //100 kHz, 4 MHz CLK_PER
    TWI0.MBAUD = 15;
    
	TWI0.MCTRLA = TWI_ENABLE_bm;
	
	/********************************
	 * Pin Directions Configuration *
	 ********************************/
	
	// Use PA2 and PA3 (default)
	
	//Output I/O
	PORTA.DIRSET = PIN2_bm | PIN3_bm;
}

/**
 * Release all resources initialized by sensirion_i2c_hal_init().
 */
void sensirion_i2c_hal_free(void) {
    /* TODO:IMPLEMENT or leave empty if no resources need to be freed */
}

/**
 * Execute one read transaction on the I2C bus, reading a given number of bytes.
 * If the device does not acknowledge the read command, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to read from
 * @param data    pointer to the buffer where the data is to be stored
 * @param count   number of bytes to read from I2C and store in the buffer
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_hal_read(uint8_t address, uint8_t* data, uint16_t count) {
	// Put slave address on bus
	TWI0.MADDR = (address << 1) | 1;
	
	// Check for errors
	if (TWI0.MSTATUS & TWI_RXACK_bm) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return SLAVE_NACK;
	}
	if (TWI0.MSTATUS & TWI_ARBLOST_bm) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return LOST_ARBITRATION;
	}
	if (TWI0.MSTATUS & TWI_BUSERR_bm) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return BUS_ERROR;
	}
	
	uint16_t i = 0;
	for (; i < count - 1; ++i) {
		while (!(TWI0.MSTATUS & TWI_RIF_bm)) { }
		*(data + i) = TWI0.MDATA;
		TWI0.MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;
	}
	
	while (!(TWI0.MSTATUS & TWI_RIF_bm)) { }
	*(data + i) = TWI0.MDATA;
	TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
		
	// Success, return 0
	return 0;
}

/**
 * Execute one write transaction on the I2C bus, sending a given number of
 * bytes. The bytes in the supplied buffer must be sent to the given address. If
 * the slave device does not acknowledge any of the bytes, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to write to
 * @param data    pointer to the buffer containing the data to write
 * @param count   number of bytes to read from the buffer and send over I2C
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_hal_write(uint8_t address, const uint8_t* data,
                               uint16_t count) {
	// Send slave address in write mode
    TWI0.MADDR = (address << 1) | 0;
	
	
	// Check for errors
	if (TWI0.MSTATUS & TWI_RXACK_bm) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return SLAVE_NACK;
	}
	if (TWI0.MSTATUS & TWI_ARBLOST_bm) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return LOST_ARBITRATION;
	}
	if (TWI0.MSTATUS & TWI_BUSERR_bm) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return BUS_ERROR;
	}
	
	for (uint16_t i = 0; i < count; ++i) {
		while (!(TWI0.MSTATUS & TWI_WIF_bm)) { }
			
		if (TWI0.MSTATUS & TWI_RXACK_bm) {
			TWI0.MCTRLB = TWI_MCMD_STOP_gc;
			return SLAVE_NACK;
		}
		
		TWI0.MDATA = *(data + i);
	}
	
	while (!(TWI0.MSTATUS & TWI_WIF_bm)) { }
	TWI0.MCTRLB = TWI_MCMD_STOP_gc;
	
	// Success, return 0
	return 0;
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * Despite the unit, a <10 millisecond precision is sufficient.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_i2c_hal_sleep_usec(uint32_t useconds) {
    for (uint32_t iters = 0; iters < useconds / 23 + 1; ++iters) {
		_delay_us(1);
	}
}
