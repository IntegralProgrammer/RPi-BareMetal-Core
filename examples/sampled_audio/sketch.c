#include "../../src/headphonejack.h"
#include "../../src/gpio.h"
#include "audio_clip.h"

void setup()
{
    headphoneOutputInit();
}

void loop()
{
    for (unsigned int i = 0; i < audio_clip_len; i++) {
        while (headphoneOutputReady() == 0) {}
        headphoneOutputWrite(audio_clip[i]);
    }
}
