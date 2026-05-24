#include "../primitives/circularbuffer_arbitrary_length.h"

#define REVERB_PARAMETRIC_NUM_ECHO 4

typedef struct {
	CircularBufferArbitraryLength dspBuffer;
	unsigned int delays[REVERB_PARAMETRIC_NUM_ECHO];
	unsigned int gains[REVERB_PARAMETRIC_NUM_ECHO];
} DSPReverbParametric;

void initReverbParametric(DSPReverbParametric* dspCtx, int* delay_buffer, unsigned int delay_buffer_length);
int processReverbParametric(DSPReverbParametric* dspCtx, int sample);
void setReverbParametricDelay(DSPReverbParametric* dspCtx, unsigned int index, unsigned int delay);
void setReverbParametricGain(DSPReverbParametric* dspCtx, unsigned int index, unsigned int gain);
