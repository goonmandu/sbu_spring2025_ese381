#ifndef BUFFER_H_
#define BUFFER_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 128

typedef struct {
	uint8_t buffer[BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;
	uint8_t count;
} circular_buffer_t;

void buffer_init(volatile circular_buffer_t *cb);
bool buffer_is_empty(volatile circular_buffer_t *cb);
bool buffer_is_full(volatile circular_buffer_t *cb);
bool buffer_enqueue(volatile circular_buffer_t *cb, volatile uint8_t byte);
bool buffer_dequeue(volatile circular_buffer_t *cb, volatile uint8_t *byte);
volatile uint8_t buffer_peek(volatile circular_buffer_t *cb);

#endif /* BUFFER_H_ */