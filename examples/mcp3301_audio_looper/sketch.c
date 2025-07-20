#include "../../src/spi.h"
#include "../../src/timer.h"
#include "../../src/headphonejack.h"
#include "../../lib/dsp/sound_effects/looper.h"
#include "../../lib/debounce/debounce.h"
#include "../../lib/drivers/adc/mcp3301.h"

// Period of 44.1 kHz signal is approximately 23 uS long
#define LOOP_PERIOD_US 23

// Maximum number of samples in the audio loop (2646000 sample ~60 seconds of audio)
#define AUDIO_LOOP_BUFFER_LENGTH 2646000

// Number of samples (at 44.1 KHz) of button debounce time ~100ms
#define BUTTON_DEBOUNCE_TIME_SAMPLES 4410

#define PIN_RESET_BUTTON 25
#define PIN_TRIGGER_BUTTON 24

DSPLooper sfxLooper;
int audioLoopBuffer[AUDIO_LOOP_BUFFER_LENGTH];

unsigned char looperFilled;
unsigned char looperPlaying;

DebouncedButton buttonReset;
DebouncedButton buttonTrigger;

void headphoneDirectOutputWriteSigned(int val)
{
  unsigned int unsignedVal = 32768;
  unsignedVal += val;

  // Write the unsigned value to the headphone port
  headphoneDirectOutputWrite(unsignedVal);
}

void setup()
{
  initLooper(&sfxLooper, audioLoopBuffer, AUDIO_LOOP_BUFFER_LENGTH);
  looperFilled = 0;
  looperPlaying = 0;

  initDebouncedButton(&buttonReset, PIN_RESET_BUTTON, BUTTON_DEBOUNCE_TIME_SAMPLES, 1);
  initDebouncedButton(&buttonTrigger, PIN_TRIGGER_BUTTON, BUTTON_DEBOUNCE_TIME_SAMPLES, 1);

  spiInit();
  spiSetClockDivisor(250); // SYSCLOCK = 250MHz -> SYSCLOCK/250 = 1MHz

  // Direct (non-FIFO) mode to avoid buffer underrun noise
  headphoneDirectOutputInit();
}

void loop()
{
  // Run this loop at 44.1 kHz
  unsigned int loopStartTime = micros();

  // Read from the MCP3301 ADC and multiply by 8 to scale the 13-bit range to a 16-bit range
  unsigned int inputSample = 8 * readMCP3301();

  // Process the audio
  unsigned int outputSample = processLooper(&sfxLooper, inputSample);

  // Send the processed sample to the headphone jack
  headphoneDirectOutputWriteSigned(outputSample);

  // Read input buttons to determine record/play/stop functionalities
  // Reset button?
  if (debouncedButtonRead(&buttonReset)) {
    looperFilled = 0;
    looperPlaying = 0;
    stopLooper(&sfxLooper);
  }

  // Record/Play/Stop button?
  if (debouncedButtonRead(&buttonTrigger)) {
    if (looperFilled == 0) {
      // Begin initial recording
      looperFilled = 1;
      recordLooper(&sfxLooper);
    } else {
      // Toggle between play/stop
      if (looperPlaying) {
        looperPlaying = 0;
        stopLooper(&sfxLooper);
      } else {
        looperPlaying = 1;
        playLooper(&sfxLooper);
      }
    }
  }

  // Wait if necessary to ensure that this loop doesn't run any faster than 44.1 kHz
  while ((loopStartTime - micros()) < LOOP_PERIOD_US) {}
}
