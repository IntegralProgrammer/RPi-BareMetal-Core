#include "chorus_parametric.h"
#include "../primitives/wavegen.h"

/*
 * Assume samples being processed are signed 16 bit
 * Since we're working with 32 bit signed integers, multiplying a 16 bit
 * integer by 64 (2 ** 6) increases its width to a maximum of
 * (16 + 6 = 22) bits which is still within the 32 bit range. This will
 * allow us to minimize numerical noise when doing arithmetic.
 */
#define INPUT_SCALE_FACTOR 64

/*
 * Get the audio sample from index_x1000 ago where index_x1000 is the
 * time measured in thousandths of a sample. If index_x1000 is not divisible
 * by 1000 and therefore doesn't point to an actual recorded sample, linearly
 * interpolate between the two real samples which index_x1000 lies between.
 */
int chorusReadInterpolatedDelayedSample(TappedDelayLine* tdl, unsigned int index_x1000) {
	unsigned int index_floor = index_x1000 / 1000;
	unsigned int index_ceil = index_floor + 1;
	unsigned int distance_from_lower = index_x1000 - (index_floor * 1000);
	unsigned int distance_from_upper = 1000 - distance_from_lower;
	int lower_sample = tappedDelayLineRead(tdl, index_floor);
	int upper_sample = tappedDelayLineRead(tdl, index_ceil);

	// A greater distance from the upper index (and therefore a smaller distance from lower index) means the lower sample is emphasized
	lower_sample *= distance_from_upper;

	// A greater distance from the lower index (and therefore a smaller distance from the upper index) means the upper sample is emphasized
	upper_sample *= distance_from_lower;

	// Mix together and return the two weighted samples
	int mixed = lower_sample + upper_sample;
	mixed /= 1000;
	return mixed;
}

void initChorusParametric(DSPChorusParametric* dspCtx, int* delay_buffer, unsigned int delay_buffer_length) {
	initTappedDelayLine(&(dspCtx->dspBuffer), delay_buffer, delay_buffer_length);
	dspCtx->oscillator_t = 0;
	dspCtx->base_delay = 0;
	dspCtx->depth = 0;
	dspCtx->rate = 0;
	dspCtx->ratio = 0;
}

int processChorusParametric(DSPChorusParametric* dspCtx, int sample) {
	TappedDelayLine* dspBufferPtr = &(dspCtx->dspBuffer);

	// Calculate the delay for this sample ... at 44.1 kHz sampling, the sine oscillator can adjust frequency in steps of 0.168 Hz / 4 = 0.042 Hz
	int this_delay = sine_262144(dspCtx->oscillator_t / 4);
	this_delay /= 65536; // scale from 32 bit range to 16 bit range ... ranges (-/+ 32768)
	this_delay *= dspCtx->depth;
	this_delay /= 32768;
	this_delay += dspCtx->base_delay;

	// Keep track of previous samples and they will need to be played back with varying delay
	tappedDelayLineWrite(dspBufferPtr, INPUT_SCALE_FACTOR * sample);

	// Advance the LFO
	dspCtx->oscillator_t = (dspCtx->oscillator_t + dspCtx->rate) & 1048575;

	// Mix the original signal with the delayed signal to get the output signal
	int scaled_original_signal = INPUT_SCALE_FACTOR * sample;
	scaled_original_signal *= (255 - dspCtx->ratio);
	scaled_original_signal /= 255;

	int scaled_delayed_signal = chorusReadInterpolatedDelayedSample(dspBufferPtr, (this_delay > 0) ? this_delay : 0);
	scaled_delayed_signal *= dspCtx->ratio;
	scaled_delayed_signal /= 255;

	// Signal output
	return (scaled_original_signal + scaled_delayed_signal) / INPUT_SCALE_FACTOR;
}

/*
 * Set the base delay to `delay` thousandths of a sample
 */
void setChorusParametricBaseDelay(DSPChorusParametric* dspCtx, unsigned int delay) {
	dspCtx->base_delay = delay;
}

/*
 * Set the depth of the delay variation to `depth` thousandths of a sample
 */
void setChorusParametricDepth(DSPChorusParametric* dspCtx, unsigned int depth) {
	dspCtx->depth = depth;
}

/*
 * At a `rate` of 1, the period of the delay variation LFO is (262144 * 4 = 1048576) samples.
 * The period of the delay variation LFO therefore is `1048576 / rate` samples
 */
void setChorusParametricRate(DSPChorusParametric* dspCtx, unsigned int rate) {
	dspCtx->rate = rate;
}

/*
 * Set the mix ratio of the original signal to the modulated delayed signal.
 * (ratio == 0) --> 100% original signal, 0% modulated delayed signal
 * (ratio == 255) --> 0% original signal, 100% modulated delayed signal
 */
void setChorusParametricMixRatio(DSPChorusParametric* dspCtx, unsigned char ratio) {
	dspCtx->ratio = ratio;
}
