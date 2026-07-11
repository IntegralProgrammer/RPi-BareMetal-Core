#include "tapped_delay_line.h"

// Modulo function not (yet) available in our libraries, therefore implementing here
unsigned int tappedDelayLineModulo(int n, unsigned int m) {
  int ret = n;
  while (ret < 0) {
    ret += m;
  }
  while (ret >= m) {
    ret -= m;
  }
  return ret;
}

void initTappedDelayLine(TappedDelayLine* tdl, int* buffer, unsigned int length) {
  tdl->buffer = buffer;
  tdl->length = length;
  tdl->index = 0;
  for (unsigned int i = 0; i < length; i++) {
    buffer[i] = 0;
  }
}

void tappedDelayLineWrite(TappedDelayLine* tdl, int value) {
  tdl->index = tappedDelayLineModulo((tdl->index + 1), tdl->length);
  tdl->buffer[tdl->index] = value;
}

int tappedDelayLineRead(TappedDelayLine* tdl, unsigned int lookback) {
  int idx = tdl->index;
  idx -= lookback;
  idx = tappedDelayLineModulo(idx, tdl->length);
  return tdl->buffer[idx];
}
