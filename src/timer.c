#include "timer.h"

extern unsigned int GET32(unsigned int);

/*
 * Returns the number of microseconds since the start of the program.
 * Wraps around after 4,294,967,295 uS or approximately 71 minutes.
 */
unsigned int micros() {
  return GET32(CLO);
}

/*
 * Wait for t microseconds, then continue
 */
void delayUS(unsigned int t) {
  unsigned int start_time = micros();
  while ((micros() - start_time) < t) {}
}
