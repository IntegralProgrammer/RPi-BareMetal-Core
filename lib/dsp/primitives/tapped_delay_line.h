#ifndef TAPPED_DELAY_LINE_H
#define TAPPED_DELAY_LINE_H

typedef struct {
  int* buffer;
  unsigned int length;
  unsigned int index;
} TappedDelayLine;

void initTappedDelayLine(TappedDelayLine* tdl, int* buffer, unsigned int length);
void tappedDelayLineWrite(TappedDelayLine* tdl, int value);
int tappedDelayLineRead(TappedDelayLine* tdl, unsigned int lookback);

#endif /* TAPPED_DELAY_LINE_H */
