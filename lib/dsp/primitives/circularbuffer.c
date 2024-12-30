#include "circularbuffer.h"

void initCircularBuffer(CircularBuffer* cb) {
	cb->index = 0;
	for (unsigned int i = 0; i < CIRCULAR_BUFFER_SIZE; i++) {
		cb->buffer[i] = 0;
	}
}

void circularBufferAdd(CircularBuffer* cb, unsigned int timeOffset, int value) {
	cb->buffer[(cb->index + timeOffset) % CIRCULAR_BUFFER_SIZE] += value;
}

unsigned int circularBufferRead(CircularBuffer* cb) {
	return cb->buffer[cb->index];
}

void circularBufferAdvance(CircularBuffer* cb) {
	cb->buffer[cb->index] = 0;
	cb->index = (cb->index + 1) % CIRCULAR_BUFFER_SIZE;
}
