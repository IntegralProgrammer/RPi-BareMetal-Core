#ifndef CIRCULAR_BUFFER_ARBITRARY_LENGTH_H
#define CIRCULAR_BUFFER_ARBITRARY_LENGTH_H

typedef struct {
  int* buffer;
  unsigned int length;
  unsigned int index;
} CircularBufferArbitraryLength;

void initCircularBufferArbitraryLength(CircularBufferArbitraryLength* cb, int* buffer, unsigned int length);
void circularBufferArbitraryLengthAdd(CircularBufferArbitraryLength* cb, unsigned int timeOffset, int value);
unsigned int circularBufferArbitraryLengthRead(CircularBufferArbitraryLength* cb);
void circularBufferArbitraryLengthAdvance(CircularBufferArbitraryLength* cb);

#endif /* CIRCULAR_BUFFER_ARBITRARY_LENGTH_H */
