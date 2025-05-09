/*
 * test.c
 *
 * Created: 2/28/2025 2:52:03 PM
 * Author : userESD
 */ 

#include <avr/io.h>


int main(void)
{
	VPORTB.DIR = 0xFF;
    /* Replace with your application code */
    while (1) 
    {
		VPORTB.OUT = 0x02;
		// VPORTB.OUT = 0x00;
    }
}

