#include "../../src/spi.h"
#include "../../src/timer.h"
#include "../../src/uart.h"
#include "../../src/headphonejack.h"
#include "../../lib/midi/midi.h"
#include "../../lib/dsp/sound_effects/chorus_parametric.h"
#include "../../lib/drivers/adc/mcp3301.h"

// Period of 44.1 kHz signal is approximately 23 uS long
#define LOOP_PERIOD_US 23
#define LOOP_AUX_PROCESSING_US 18

// Maximum number of samples that a sample can be delayed by (~1 second)
#define AUDIO_CHORUS_BUFFER_LENGTH 44100

// Reset the MIDI parser to the initial state if no data has been received in the last 250 milliseconds
// at 44100 Hz this is (44100 * 250) / 1000 = 11025 samples
#define MIDI_PARSER_TIMEOUT_SAMPLES 11025

// MIDI CC events for controller knobs
#define ARTURIA_MINILAB_KNOB_1 74
#define ARTURIA_MINILAB_KNOB_2 71
#define ARTURIA_MINILAB_KNOB_3 76
#define ARTURIA_MINILAB_KNOB_4 77

#define ARTURIA_MINILAB_FADER_1 82

DSPChorusParametric sfxChorus;
int audioChorusBuffer[AUDIO_CHORUS_BUFFER_LENGTH];

unsigned char outputVolume;

MIDIParser midiParser;

void handleKnobUpdate(unsigned char knob, unsigned char value)
{
  // Sanity check
  if (value > 127) {
    return;
  }

  if (knob == ARTURIA_MINILAB_KNOB_1) {
    // Base delay varies from 0 samples (0 ms) to 4410 samples (~100 ms)
    // ... multiply by 1000 since delay is measured in thousandths of a sample
    setChorusParametricBaseDelay(&sfxChorus, (4410000 * value) / 127);
  } else if (knob == ARTURIA_MINILAB_KNOB_2) {
    // Delay variation varies from +/- 0 samples (0 ms) to +/- 44 samples (~1 ms)
    // ... multiply by 1000 since delay is measured in thousandths of a sample
    setChorusParametricDepth(&sfxChorus, (44000 * value) / 127);
  } else if (knob == ARTURIA_MINILAB_KNOB_3) {
    // Delay variation rate varies from 0 Hz to (127 * 0.042 = 5.334) Hz
    setChorusParametricRate(&sfxChorus, value);
  } else if (knob == ARTURIA_MINILAB_KNOB_4) {
    // Mix ratio varies from 0 to 254 where 0 is 100% original signal and 255 is 100% modulated signal
    setChorusParametricMixRatio(&sfxChorus, 2 * value);
  } else if (knob == ARTURIA_MINILAB_FADER_1) {
    outputVolume = value;
  }
}

void handleMidi()
{
  if (uartAvailable()) {
    feedMidiParser(&midiParser, uartRead());
    if (midiParser.message_ready) {
      if (midiParser.message_type == MIDI_MESSAGE_TYPE_CONTROL_CHANGE) {
        handleKnobUpdate(midiParser.message_control_change.controller_number, midiParser.message_control_change.controller_value);
      }
    }
  }
}

void headphoneDirectOutputWriteSigned(int val)
{
  unsigned int unsignedVal = 32768;
  unsignedVal += val;

  // Write the unsigned value to the headphone port
  headphoneDirectOutputWrite(unsignedVal);
}

void setup()
{
  // Initialize the UART
  uartInit(31250);

  // Initialize the MIDI Parser
  initMidiParser(&midiParser, MIDI_PARSER_TIMEOUT_SAMPLES);

  initChorusParametric(&sfxChorus, audioChorusBuffer, AUDIO_CHORUS_BUFFER_LENGTH);

  outputVolume = 0;

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
  int inputSample = 8 * readMCP3301();

  // Process the audio
  int outputSample = processChorusParametric(&sfxChorus, inputSample);

  // Adjust the volume
  outputSample *= outputVolume;
  outputSample /= 127;

  // MIDI countdown timer
  countdownMidiParser(&midiParser);

  // Handle MIDI controller events ...
  // ... ensure that it runs at least once
  handleMidi();

  // ... then run again in this loop if time permits
  while ((micros() - loopStartTime) < LOOP_AUX_PROCESSING_US) {
    handleMidi();
  }

  // Wait if necessary to ensure that this loop doesn't run any faster than 44.1 kHz
  while ((micros() - loopStartTime) < LOOP_PERIOD_US) {}

  // Send the processed sample to the headphone jack mixed with the input sample at the set ratio
  headphoneDirectOutputWriteSigned(outputSample);
}
