#include "wavegen.h"
#include "wavegen_sine.h"

int sine_262144(unsigned int t) {
  unsigned int t_fixed = t & 262143;
  return SINE_WAVE_262144[t_fixed];
}
