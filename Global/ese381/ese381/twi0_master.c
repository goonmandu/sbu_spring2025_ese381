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

#ifndef F_CPU
#define F_CPU 4000000UL  // default to 4 MHz
#endif /* F_CPU */

#include "twi0_master.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define TWI_READ true
#define TWI_WRITE false

#define TWI_IS_CLOCKHELD() TWI0.MSTATUS & TWI_CLKHOLD_bm
#define TWI_IS_BUSERR() TWI0.MSTATUS & TWI_BUSERR_bm
#define TWI_IS_ARBLOST() TWI0.MSTATUS & TWI_ARBLOST_bm

#define CLIENT_NACK() TWI0.MSTATUS & TWI_RXACK_bm
#define CLIENT_ACK() !(TWI0.MSTATUS & TWI_RXACK_bm)

#define TWI_IS_BUSBUSY() ((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc) == TWI_BUSSTATE_BUSY_gc)

#define TWI_WAIT() while (!((TWI_IS_CLOCKHELD()) || (TWI_IS_BUSERR()) || (TWI_IS_ARBLOST()) || (TWI_IS_BUSBUSY())))


bool isTWIBad(void) {
	//Checks for: NACK, ARBLOST, BUSERR, Bus Busy
	if ((((TWI0.MSTATUS) & (TWI_RXACK_bm | TWI_ARBLOST_bm | TWI_BUSERR_bm)) > 0) || (TWI_IS_BUSBUSY())) {
		return true;
	}
	return false;
}

void TWI_initHost(void) {
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
	TWI0.MBAUD = 15;
	
	//[No ISRs] and Host Mode
	TWI0.MCTRLA = TWI_ENABLE_bm;
}

void TWI_initPins(void) {
	// Use PA2 and PA3 (default)
	
	//Output I/O
	PORTA.DIRSET = PIN2_bm | PIN3_bm;

	#ifdef TWI_ENABLE_PULLUPS
	//Enable Pull-Ups
	PORTA.PINCONFIG = PORT_PULLUPEN_bm;
	#endif

	//Select RA2/RA3
	PORTA.PINCTRLUPD = PIN2_bm | PIN3_bm;
}

bool _startTWI(uint8_t addr, bool read) {
	//If the Bus is Busy
	if (TWI_IS_BUSBUSY()) {
		return false;
	}
	
	//Send Address
	TWI0.MADDR = (addr << 1) | read;
	
	//Wait...
	TWI_WAIT();
	
	if (isTWIBad()) {
		//Stop the Bus
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return false;
	}
	
	//TWI Started
	return true;
}

//Internal Function: Reads len bytes from TWI, then issues a bus STOP
void _readFromTWI(uint8_t* data, uint8_t len) {
	uint8_t bCount = 0;
	
	//Release the clock hold
	
	TWI0.MSTATUS = TWI_CLKHOLD_bm;
	
	//TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;
	
	while (bCount < len) {
		//Wait...
		TWI_WAIT();
		
		//Store data
		data[bCount] = TWI0.MDATA;

		//Increment the counter
		bCount += 1;
		
		if (bCount != len) {
			//If not done, then ACK and read the next byte
			TWI0.MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;
		}
	}
	
	//NACK and STOP the bus
	TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
}

//Internal Function: Write len bytes to TWI. Does NOT STOP the bus. Returns true if successful
bool _writeToTWI(uint8_t* data, uint8_t len) {
	uint8_t count = 0;
	
	while (count < len) {
		//Write a byte
		TWI0.MDATA = data[count];
		
		//Wait...
		TWI_WAIT();
		
		//If the client NACKed, then abort the write
		if (CLIENT_NACK()) {
			return false;
		}
		
		//Increment the counter
		count++;
	}
	
	return true;
}

bool TWI_sendByte(uint8_t addr, uint8_t data) {
	//Address Client Device (Write)
	if (!_startTWI(addr, TWI_WRITE)) {
		return false;
	}
	
	bool success = _writeToTWI(&data, 1);
	
	//Stop the bus
	TWI0.MCTRLB = TWI_MCMD_STOP_gc;
	
	return success;
}

bool TWI_sendBytes(uint8_t addr, uint8_t* data, uint8_t len) {
	//Address Client Device (Write)
	if (!_startTWI(addr, TWI_WRITE)) {
		return false;
	}
	
	//Write the bytes to the client
	bool success = _writeToTWI(data, len);

	//Stop the bus
	TWI0.MCTRLB = TWI_MCMD_STOP_gc;
	
	return success;
}

bool TWI_readByte(uint8_t addr, uint8_t* data) {
	//Address Client Device (Read)
	if (!_startTWI(addr, TWI_READ)) {
		return false;
	}
	
	//Read byte from client
	_readFromTWI(data, 1);

	return true;
}


bool TWI_readBytes(uint8_t addr, uint8_t* data, uint8_t len) {
	//Address Client Device (Read)
	if (!_startTWI(addr, TWI_READ)) {
		return false;
	}
	
	//Read bytes from client
	_readFromTWI(data, len);
	
	return true;
}

bool TWI_sendAndReadBytes(uint8_t addr, uint8_t regAddress, uint8_t* data, uint8_t len, uint8_t wait_between_wr) {
	//Address Client Device (Write)
	if (!_startTWI(addr, TWI_WRITE)) {
		return false;
	}
	
	//Write register address
	if (!_writeToTWI(&regAddress, 1)) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return false;
	}
	
	if (wait_between_wr > 0) _delay_ms(1);
	
	//Restart the TWI Bus in READ mode
	TWI0.MADDR |= TWI_READ;
	TWI0.MCTRLB = TWI_MCMD_REPSTART_gc;
	
	//Wait...
	TWI_WAIT();
	
	if (isTWIBad()) {
		//Stop the TWI Bus if an error occurred
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return false;
	}
	
	//Read the data from the client
	_readFromTWI(data, len);
	
	return true;
}

bool TWI_sendBytesAndReadBytes(uint8_t addr, uint8_t *data_to_send, uint8_t send_len, uint8_t *data_read, uint8_t read_len, uint8_t wait_between_wr) {
	//Address Client Device (Write)
	if (!_startTWI(addr, TWI_WRITE)) {
		return false;
	}
	
	//Write register address
	if (!_writeToTWI(data_to_send, send_len)) {
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return false;
	}
	
	if (wait_between_wr > 0) _delay_ms(1);
	
	//Restart the TWI Bus in READ mode
	TWI0.MADDR |= TWI_READ;
	TWI0.MCTRLB = TWI_MCMD_REPSTART_gc;
	
	//Wait...
	TWI_WAIT();
	
	if (isTWIBad()) {
		//Stop the TWI Bus if an error occurred
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return false;
	}
	
	//Read the data from the client
	_readFromTWI(data_read, read_len);
	
	return true;
}