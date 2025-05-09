/* TO THE PRELAB GRADER:
 *
 * This program is for both Task 2 and 3 as the payload is stored
 * in the variable declared `char payload[BUF_SIZE+1]` anyways.
 */

#define F_CPU 4000000UL
#define BUF_SIZE 128
#define BENCH_NUMBER 17
#define LW1_ADDR BENCH_NUMBER
#define LW2_ADDR (BENCH_NUMBER + 30)

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <usart/usart1.h>

const char *condition = "+RCV";
char preamble[5];
uint16_t transmitter_address;

volatile char rx_buf[BUF_SIZE+1];
volatile char tx_buf[BUF_SIZE+1];
volatile uint8_t rx_len = 0;
volatile uint8_t tx_len = 0;
volatile uint8_t tx_idx = 0;

uint8_t payload_length = 0;
char payload[BUF_SIZE+1];

uint8_t end_of_tx();
uint8_t string_equals(char *lhs, char *rhs, uint8_t chars);
uint8_t got_target_string();
// UNUSED: uint8_t copy_string_and_get_length(char *dest, const char *src, char terminator);

int main(void) {
    USART1_init_flex(115200, 8, 'N');
    USART1.DBGCTRL = 0x01;
    USART1.CTRLA = USART_RXCIE_bm;

    sei();

    while (1) {
        if (end_of_tx()) {
            // USART1.CTRLA = 0x00;
            if (got_target_string()) {
                sprintf(tx_buf, "AT+SEND=%u,%u,%s\r\n", LW2_ADDR, payload_length, payload);
                tx_len = strlen(tx_buf);
                *rx_buf = '\0';
                rx_len = 0;
                USART1.CTRLA = USART_DREIE_bm;
            } else {
                *rx_buf = '\0';
                rx_len = 0;
            }
        }
    }
}

ISR(USART1_RXC_vect) {
    cli();
    if (rx_len >= BUF_SIZE) {
        sei();
        return;
    }
    rx_buf[rx_len] = USART1.RXDATAL;
    rx_len++;
    sei();
}

ISR(USART1_DRE_vect) {
    cli();
    if (tx_idx < tx_len) {
        USART1.TXDATAL = tx_buf[tx_idx];
        tx_idx++;
    } else {
        tx_len = 0;
        tx_idx = 0;
        USART1.CTRLA = USART_RXCIE_bm;
    }
    sei();
}

uint8_t end_of_tx() {
    if (rx_buf[rx_len-1] == '\n' && rx_buf[rx_len-2] == '\r') {
        rx_buf[rx_len] = '\0';
        return 1;
    }
    return 0;
}

/*
 * Compares two strings for equality.
 * Returns 1 if two are equal, and 0 otherwise.
 * `chars` can be specified to check the
 * first n characters of the two strings.
 * Leave chars = 0 to check entire string.
 */
uint8_t string_equals(char *lhs, char *rhs, uint8_t chars) {
    if (chars) {
        while (*lhs != '\0' || *rhs != '\0') {
            if (*lhs != *rhs) return 0;
            lhs++; rhs++; chars--;
            if (chars == 0) return 1;
        }
        return *lhs == '\0' && *rhs == '\0';
    } else {
        while (*lhs != '\0' || *rhs != '\0') {
            if (*lhs != *rhs) return 0;
            lhs++; rhs++;
        }
        return *lhs == '\0' && *rhs == '\0';
    }
}

uint8_t got_target_string() {
    // +RCV=69,8,Testing!,-43,41
    uint8_t checking_idx = 0;
    char payload_length_str[4];
    uint8_t pls_idx = 0;

    // Store preamble and transmitter address
    if (!string_equals(rx_buf, condition, 4)) return 0;
    sscanf(rx_buf, "%[^=]=%lu", preamble, transmitter_address);

    // Store payload length
    while (rx_buf[checking_idx++] != '.') { }
    while (rx_buf[checking_idx] != ',') {
        payload_length_str[pls_idx] = rx_buf[checking_idx];
        pls_idx++;
        checking_idx++;
    }
    payload_length_str[pls_idx] = '\0';
    payload_length = atoi(payload_length_str);
    checking_idx++;
    for (uint8_t payload_idx = 0; payload_idx < payload_length; ++checking_idx) {
        payload[payload_idx] = rx_buf[checking_idx];
    }
    *(payload + payload_length) = '\0';
    return string_equals(preamble, condition, 0);
}

/* UNUSED: Specify `terminator` to use as the terminator. */
uint8_t copy_string_and_get_length(char *dest, const char *src, char terminator) {
    uint8_t ret = 0;
    while (*src != '\0') {
        *dest++ = *src++;
        ret++;
    }
    if (terminator >= 0) {
        *dest = terminator;
    }
    return ret;
}