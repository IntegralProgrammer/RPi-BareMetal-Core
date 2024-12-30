#include "reverb.h"

void initReverb(DSPReverb* dspCtx) {
	initCircularBuffer(&(dspCtx->dspBuffer));
}

int processReverb(DSPReverb* dspCtx, int sample) {
	CircularBuffer* dspBufferPtr = &(dspCtx->dspBuffer);
	circularBufferAdd(dspBufferPtr, 0, sample / 2);
	int output = circularBufferRead(dspBufferPtr);

	// Feedback output with delay and attenuation
	// Heavy reverb
	circularBufferAdd(dspBufferPtr, 4410, output / 2); // 100ms
	circularBufferAdd(dspBufferPtr, 11025, output / 4); // 250ms
	circularBufferAdd(dspBufferPtr, 15435, output / 8); // 350ms

	// Light reverb
	//circularBufferAdd(dspBufferPtr, 2205, output / 2); // 50ms

	circularBufferAdvance(dspBufferPtr);
	return output;
}
