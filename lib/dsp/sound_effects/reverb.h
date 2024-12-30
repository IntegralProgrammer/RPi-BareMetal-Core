#include "../primitives/circularbuffer.h"

typedef struct {
	CircularBuffer dspBuffer;
} DSPReverb;

void initReverb(DSPReverb* dspCtx);
int processReverb(DSPReverb* dspCtx, int sample);
