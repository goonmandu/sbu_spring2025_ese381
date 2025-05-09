#include "buffer.h"

void buffer_init(volatile circular_buffer_t *cb) {
	cb->head = 0;
	cb->tail = 0;
	cb->count = 0;
}

bool buffer_is_empty(volatile circular_buffer_t *cb) {
	return cb->count == 0;
}

bool buffer_is_full(volatile circular_buffer_t *cb) {
	return cb->count == BUFFER_SIZE;
}

bool buffer_enqueue(volatile circular_buffer_t *cb, volatile uint8_t data) {
	if (buffer_is_full(cb)) {
		return false;
	}

	cb->buffer[cb->head] = data;
	cb->head = (cb->head + 1) % BUFFER_SIZE;
	cb->count++;

	return true;
}

bool buffer_dequeue(volatile circular_buffer_t *cb, volatile uint8_t *data) {
	if (buffer_is_empty(cb)) {
		return false;
	}

	*data = cb->buffer[cb->tail];
	cb->tail = (cb->tail + 1) % BUFFER_SIZE;
	cb->count--;

	return true;
}

volatile uint8_t buffer_peek(volatile circular_buffer_t *cb) {
	return cb->buffer[cb->tail];
}