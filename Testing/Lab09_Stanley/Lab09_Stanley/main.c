/*
 * receive_payload.c
 *
 * Created: 4/16/2025 1:02:31 PM
 * Authors : Katherine Trusinski and Stanley Cokro
 * Description: Receives AT commands, parses them, and extracts payload.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>

#define USART1_BAUD_RATE(BAUD_RATE) ((float)(4000000 * 64 / (16 * (float)BAUD_RATE)) + 0.5)
#define F_CPU 4000000

// Buffer sizes, using the values that the Professor recommended in lecture
#define RECEIVED_BUFFER_SIZE 80
#define PAYLOAD_BUFFER_SIZE 40

// Buffers and control variables
char received_message[RECEIVED_BUFFER_SIZE] = {0}; // Buffer for complete message
static volatile uint8_t received_index = 0;              // Index for received message
static volatile uint8_t message_complete = 0;            // Flag for message completion

char payload[PAYLOAD_BUFFER_SIZE] = {0};          // Buffer for extracted payload

// Additional fields for parsing
static char RCV_preamble[10] = {0};
static volatile unsigned int txmtr_address = 0;
static volatile unsigned int rcv_data_len = 0;
static volatile int RSSI = 0;
static volatile int SNR = 0;

// Function prototypes
void USART1_Init(uint32_t baud, uint8_t data_bits, unsigned char parity);
void parse_received_message(void);

int main(void)
{
    USART1_Init(115200, 8, 'd'); // Initialize USART1
    sei(); // Enable global interrupts

    while (1)
    {
        if (message_complete) // Check if a complete message is received
        {
            message_complete = 0; // Reset the flag
            parse_received_message(); // Parse the message
            // Process payload as needed
        }
    }

    return 0;
}

// Function to initialize USART1
void USART1_Init(uint32_t baud, uint8_t data_bits, unsigned char parity)
{
    // Configure pins for TX and RX
    PORTC.DIR &= ~PIN1_bm; // RXD as input
    PORTC.DIR |= PIN0_bm;  // TXD as output
    PORTC.OUT |= PIN0_bm;  // TXD idle state high

    // Set the baud rate
    USART1.BAUD = USART1_BAUD_RATE(baud);

    // Enable TX, RX, and RX Complete interrupt
    USART1.CTRLB |= (USART_RXEN_bm | USART_TXEN_bm);
    USART1.CTRLA |= (USART_RXCIE_bm);

    // Configure parity
    uint8_t p;
    switch (parity)
    {
        case 'd':
            p = USART_PMODE_DISABLED_gc;
            break;
        case 'e':
            p = USART_PMODE_EVEN_gc;
            break;
        case 'o':
            p = USART_PMODE_ODD_gc;
            break;
        default:
            return;
    }

    // Configure data bits
    uint8_t d;
    switch (data_bits)
    {
        case 8:
            d = USART_CHSIZE_8BIT_gc;
            break;
        case 7:
            d = USART_CHSIZE_7BIT_gc;
            break;
        case 6:
            d = USART_CHSIZE_6BIT_gc;
            break;
        case 5:
            d = USART_CHSIZE_5BIT_gc;
            break;
        default:
            return;
    }

    // Configure USART control settings
    USART1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | p | USART_SBMODE_1BIT_gc | d;
}

// USART RX Complete interrupt service routine
ISR(USART1_RXC_vect)
{
    cli(); // Clear global interrupts

    char received_char = USART1.RXDATAL; // Read received character

    if (received_char == '\n') // End of message
    {
        message_complete = 1; // Mark message as complete
        received_index = 0;   // Reset index for the next message
    }
    else if (received_index < RECEIVED_BUFFER_SIZE - 1)
    {
        received_message[received_index++] = received_char; // Store character in buffer
        received_message[received_index] = '\0';            // Null-terminate
    }

    sei(); // Re-enable global interrupts
}

// Function to parse the received message
void parse_received_message(void)
{
    sscanf(received_message, "%[^=]=%u,%u,%[^,],%d,%d,'\r''\n'",
           RCV_preamble, &txmtr_address, &rcv_data_len, payload, &RSSI, &SNR);
}