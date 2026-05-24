#include "reverb_parametric.h"

/*
 * Assume samples being processed are signed 16 bit
 * Since we're working with 32 bit signed integers, multiplying a 16 bit
 * integer by 64 (2 ** 6) increases its width to a maximum of
 * (16 + 6 = 22) bits which is still within the 32 bit range. This will
 * allow us to minimize numerical noise when doing arithmetic.
 */
#define INPUT_SCALE_FACTOR 64

void initReverbParametric(DSPReverbParametric* dspCtx, int* delay_buffer, unsigned int delay_buffer_length) {
	initCircularBufferArbitraryLength(&(dspCtx->dspBuffer), delay_buffer, delay_buffer_length);
	for (unsigned int i = 0; i < REVERB_PARAMETRIC_NUM_ECHO; i++) {
		dspCtx->delays[i] = 0;
		dspCtx->gains[i] = 0;
	}
}

int processReverbParametric(DSPReverbParametric* dspCtx, int sample) {
	CircularBufferArbitraryLength* dspBufferPtr = &(dspCtx->dspBuffer);
	circularBufferArbitraryLengthAdd(dspBufferPtr, 0, (INPUT_SCALE_FACTOR * sample) / (REVERB_PARAMETRIC_NUM_ECHO + 1));
	int output = circularBufferArbitraryLengthRead(dspBufferPtr);

	// Feedback output with delay and attenuation
	// (REVERB_PARAMETRIC_NUM_ECHO + 2) ensures that LOOP GAIN < 1
	for (unsigned int i = 0; i < REVERB_PARAMETRIC_NUM_ECHO; i++) {
		int scaled_output = output;
		scaled_output *= dspCtx->gains[i];
		scaled_output /= (127 * (REVERB_PARAMETRIC_NUM_ECHO + 2));
		circularBufferArbitraryLengthAdd(dspBufferPtr, dspCtx->delays[i], scaled_output);
	}

	circularBufferArbitraryLengthAdvance(dspBufferPtr);

	return output / INPUT_SCALE_FACTOR;
}

void setReverbParametricDelay(DSPReverbParametric* dspCtx, unsigned int index, unsigned int delay) {
	if (index >= REVERB_PARAMETRIC_NUM_ECHO) {
		return;
	}
	dspCtx->delays[index] = delay;
}

void setReverbParametricGain(DSPReverbParametric* dspCtx, unsigned int index, unsigned int gain) {
	if (index >= REVERB_PARAMETRIC_NUM_ECHO) {
		return;
	}
	if (gain > 127) {
		return;
	}
	dspCtx->gains[index] = gain;
}
