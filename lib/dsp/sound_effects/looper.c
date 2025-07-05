#include "looper.h"

#define LOOPER_STATE_STOP 0
#define LOOPER_STATE_RECORDING 1
#define LOOPER_STATE_PLAYBACK 2

void initLooper(DSPLooper* dspCtx, int* loopBuffer, unsigned int loopBufferLength) {
	dspCtx->loopBuffer = loopBuffer;
	dspCtx->loopBufferLength = loopBufferLength;
	dspCtx->endOfLoop = 0;
	dspCtx->loopPosition = 0;
	dspCtx->looperState = LOOPER_STATE_STOP;
}

int processLooper(DSPLooper* dspCtx, int sample) {
	int outputSample = sample;

	if (dspCtx->looperState == LOOPER_STATE_PLAYBACK) {
		// Add the current part of the loop to the outputSample
		outputSample += dspCtx->loopBuffer[dspCtx->loopPosition];

		/*
		 * Increment the loopPosition looping back around at the end of
		 * the loop
		 */
		dspCtx->loopPosition += 1;
		if (dspCtx->loopPosition >= dspCtx->endOfLoop) {
			dspCtx->loopPosition = 0;
		}
	}

	if (dspCtx->looperState == LOOPER_STATE_RECORDING) {
		// Set the current part of the loop to the input sample
		dspCtx->loopBuffer[dspCtx->loopPosition] = sample;
		dspCtx->endOfLoop = dspCtx->loopPosition;

		/*
		 * Increment the loopPosition but go directly to playback mode if we've filled
		 * the loop buffer
		 */
		dspCtx->loopPosition += 1;
		if (dspCtx->loopPosition >= dspCtx->loopBufferLength) {
			playLooper(dspCtx);
		}
	}

	return outputSample;
}

void stopLooper(DSPLooper* dspCtx) {
	dspCtx->looperState = LOOPER_STATE_STOP;
	dspCtx->loopPosition = 0;
}

void recordLooper(DSPLooper* dspCtx) {
	dspCtx->looperState = LOOPER_STATE_RECORDING;
	dspCtx->loopPosition = 0;
}

void playLooper(DSPLooper* dspCtx) {
	dspCtx->looperState = LOOPER_STATE_PLAYBACK;
	dspCtx->loopPosition = 0;
}
