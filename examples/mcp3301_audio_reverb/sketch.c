#include "../../src/spi.h"
#include "../../src/timer.h"
#include "../../src/uart.h"
#include "../../src/headphonejack.h"
#include "../../lib/midi/midi.h"
#include "../../lib/dsp/sound_effects/reverb_parametric.h"
#include "../../lib/drivers/adc/mcp3301.h"

// Period of 44.1 kHz signal is approximately 23 uS long
#define LOOP_PERIOD_US 23
#define LOOP_AUX_PROCESSING_US 18

// Maximum number of samples that a sample can be delayed by (~10 seconds)
#define AUDIO_REVERB_BUFFER_LENGTH 441000

// Reset the MIDI parser to the initial state if no data has been received in the last 250 milliseconds
#define MIDI_PARSER_TIMEOUT_MS 250

// MIDI CC events for controller knobs
#define ARTURIA_MINILAB_KNOB_1 74
#define ARTURIA_MINILAB_KNOB_2 71
#define ARTURIA_MINILAB_KNOB_3 76
#define ARTURIA_MINILAB_KNOB_4 77
#define ARTURIA_MINILAB_KNOB_5 93
#define ARTURIA_MINILAB_KNOB_6 18
#define ARTURIA_MINILAB_KNOB_7 19
#define ARTURIA_MINILAB_KNOB_8 16

#define ARTURIA_MINILAB_FADER_1 82
#define ARTURIA_MINILAB_FADER_2 83

DSPReverbParametric sfxReverb;
int audioReverbBuffer[AUDIO_REVERB_BUFFER_LENGTH];

unsigned char originalSignalOutputGain;
unsigned char processedSignalOutputGain;

MIDIParser midiParser;

void handleKnobUpdate(unsigned char knob, unsigned char value)
{
  // Sanity check
  if (value > 127) {
    return;
  }

  if (knob == ARTURIA_MINILAB_KNOB_1) {
    setReverbParametricDelay(&sfxReverb, 0, 256 * value);
  } else if (knob == ARTURIA_MINILAB_KNOB_5) {
    setReverbParametricGain(&sfxReverb, 0, value);
  } else if (knob == ARTURIA_MINILAB_KNOB_2) {
    setReverbParametricDelay(&sfxReverb, 1, 256 * value);
  } else if (knob == ARTURIA_MINILAB_KNOB_6) {
    setReverbParametricGain(&sfxReverb, 1, value);
  } else if (knob == ARTURIA_MINILAB_KNOB_3) {
    setReverbParametricDelay(&sfxReverb, 2, 256 * value);
  } else if (knob == ARTURIA_MINILAB_KNOB_7) {
    setReverbParametricGain(&sfxReverb, 2, value);
  } else if (knob == ARTURIA_MINILAB_KNOB_4) {
    setReverbParametricDelay(&sfxReverb, 3, 256 * value);
  } else if (knob == ARTURIA_MINILAB_KNOB_8) {
    setReverbParametricGain(&sfxReverb, 3, value);
  } else if (knob == ARTURIA_MINILAB_FADER_1) {
    originalSignalOutputGain = value;
  } else if (knob == ARTURIA_MINILAB_FADER_2) {
    processedSignalOutputGain = value;
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

unsigned int mixSamples(int sample_a, int sample_b, unsigned char gain_a, unsigned char gain_b)
{
  int scaled_sample_a = sample_a;
  int scaled_sample_b = sample_b;
  scaled_sample_a *= gain_a;
  scaled_sample_a /= 127;
  scaled_sample_b *= gain_b;
  scaled_sample_b /= 127;
  return scaled_sample_a + scaled_sample_b;
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
  initMidiParser(&midiParser, MIDI_PARSER_TIMEOUT_MS);

  initReverbParametric(&sfxReverb, audioReverbBuffer, AUDIO_REVERB_BUFFER_LENGTH);
  originalSignalOutputGain = 0;
  processedSignalOutputGain = 0;

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
  unsigned int outputSample = processReverbParametric(&sfxReverb, inputSample);

  // Send the processed sample to the headphone jack mixed with the input sample at the set ratio
  headphoneDirectOutputWriteSigned(mixSamples(outputSample, inputSample, originalSignalOutputGain, processedSignalOutputGain));

  // Handle MIDI controller events ...
  // ... ensure that it runs at least once
  handleMidi();

  // ... then run again in this loop if time permits
  while ((loopStartTime - micros()) < LOOP_AUX_PROCESSING_US) {
    handleMidi();
  }

  // Wait if necessary to ensure that this loop doesn't run any faster than 44.1 kHz
  while ((loopStartTime - micros()) < LOOP_PERIOD_US) {}
}
