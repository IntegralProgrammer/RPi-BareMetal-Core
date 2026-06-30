#include "../../src/timer.h"
#include "../../src/uart.h"
#include "../../src/headphonejack.h"
#include "../../lib/midi/midi.h"
#include "../../lib/dsp/primitives/wavegen.h"

// Period of 44.1 kHz signal is approximately 23 uS long
#define LOOP_PERIOD_US 23
#define LOOP_AUX_PROCESSING_US 18

// Reset the MIDI parser to the initial state if no data has been received in the last 250 milliseconds
// at 44100 Hz this is (44100 * 250) / 1000 = 11025 samples
#define MIDI_PARSER_TIMEOUT_SAMPLES 11025

// MIDI CC events for controller knobs
#define ARTURIA_MINILAB_KNOB_1 74
#define ARTURIA_MINILAB_FADER_1 82

unsigned char outputGain;
unsigned int freqMultiplier;
unsigned int wave_t;

MIDIParser midiParser;

void handleKnobUpdate(unsigned char knob, unsigned char value)
{
  // Sanity check
  if (value > 127) {
    return;
  }

  if (knob == ARTURIA_MINILAB_KNOB_1) {
    freqMultiplier = value;
    freqMultiplier *= 5358;
    freqMultiplier /= 127;
    freqMultiplier += 594;
  } else if (knob == ARTURIA_MINILAB_FADER_1) {
    outputGain = value;
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

  outputGain = 0;
  freqMultiplier = 594; // Lowest frequency to generate is 100Hz
  wave_t = 0;

  headphoneDirectOutputInit();
}

void loop()
{
  // Run this loop at 44.1 kHz
  unsigned int loopStartTime = micros();

  /* 
   * Generate the sine signal
   * 
   * When running at 44.1 KHz sampling, sine_262144(wave_t) has a frequency of 0.168 Hz.
   * With a frequency multiplier of 594 we have a frequency of 100 Hz.
   * With a frequency multiplier of 5952 we have a frequency of 1000 Hz.
   * The frequency multiplier must therefore have a range of [594, 5952] across a MIDI knob input domain of [0, 127]
   * Given that sine_262144(wave_t) ranges [-2147483647, 2147483647] (32-bit signed range) we divide by 65536 to scale to the 16-bit range
   */
  int outputSample = sine_262144(wave_t);
  outputSample /= 65536;
  outputSample *= outputGain;
  outputSample /= 127;

  // Get ready for the next sample
  wave_t = (wave_t + freqMultiplier) & 262143;

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

  // Output the sample
  headphoneDirectOutputWriteSigned(outputSample);
}
