typedef struct {
	int* loopBuffer;
	unsigned int loopBufferLength;
	unsigned int endOfLoop;
	unsigned int loopPosition;
	unsigned char looperState;
} DSPLooper;

void initLooper(DSPLooper* dspCtx, int* loopBuffer, unsigned int loopBufferLength);
int processLooper(DSPLooper* dspCtx, int sample);

void stopLooper(DSPLooper* dspCtx);
void recordLooper(DSPLooper* dspCtx);
void playLooper(DSPLooper* dspCtx);
