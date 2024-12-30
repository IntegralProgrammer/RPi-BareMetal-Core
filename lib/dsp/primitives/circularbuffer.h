#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

// 2 seconds of audio
#define CIRCULAR_BUFFER_SIZE 88200

typedef struct {
	int buffer[CIRCULAR_BUFFER_SIZE];
	unsigned int index;
} CircularBuffer;

void initCircularBuffer(CircularBuffer* cb);
void circularBufferAdd(CircularBuffer* cb, unsigned int timeOffset, int value);
unsigned int circularBufferRead(CircularBuffer* cb);
void circularBufferAdvance(CircularBuffer* cb);

#endif /* CIRCULAR_BUFFER_H */
