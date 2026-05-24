#include "circularbuffer_arbitrary_length.h"

// Modulo function not (yet) available in our libraries, therefore implementing here
unsigned int circularBufferModulo(unsigned int n, unsigned int m) {
  unsigned int ret = n;
  while (ret >= m) {
    ret -= m;
  }
  return ret;
}

void initCircularBufferArbitraryLength(CircularBufferArbitraryLength* cb, int* buffer, unsigned int length) {
  cb->buffer = buffer;
  cb->length = length;
  cb->index = 0;
  for (unsigned int i = 0; i < length; i++) {
    buffer[i] = 0;
  }
}

void circularBufferArbitraryLengthAdd(CircularBufferArbitraryLength* cb, unsigned int timeOffset, int value) {
  cb->buffer[circularBufferModulo((cb->index + timeOffset), cb->length)] += value;
}

unsigned int circularBufferArbitraryLengthRead(CircularBufferArbitraryLength* cb) {
  return cb->buffer[cb->index];
}

void circularBufferArbitraryLengthAdvance(CircularBufferArbitraryLength* cb) {
  cb->buffer[cb->index] = 0;
  cb->index = circularBufferModulo((cb->index + 1), cb->length);
}
