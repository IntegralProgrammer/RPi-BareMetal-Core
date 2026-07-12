#include "../primitives/tapped_delay_line.h"

typedef struct {
	TappedDelayLine dspBuffer;
	unsigned int oscillator_t;
	unsigned int base_delay;
	unsigned int depth;
	unsigned int rate;
	unsigned char ratio;
} DSPChorusParametric;

void initChorusParametric(DSPChorusParametric* dspCtx, int* delay_buffer, unsigned int delay_buffer_length);
int processChorusParametric(DSPChorusParametric* dspCtx, int sample);
void setChorusParametricBaseDelay(DSPChorusParametric* dspCtx, unsigned int delay);
void setChorusParametricDepth(DSPChorusParametric* dspCtx, unsigned int depth);
void setChorusParametricRate(DSPChorusParametric* dspCtx, unsigned int rate);
void setChorusParametricMixRatio(DSPChorusParametric* dspCtx, unsigned char ratio);
