#include "../../src/timer.h"
#include "../../src/uart.h"
#include "../../src/headphonejack.h"
#include "../../lib/midi/midi.h"
#include "../../lib/dsp/sound_effects/soundfont.h"
#include "soundfont_violin/violin.h"

// Period of 44.1 kHz signal is approximately 23 uS long
#define LOOP_PERIOD_US 23
#define LOOP_AUX_PROCESSING_US 18

// Reset the MIDI parser to the initial state if no data has been received in the last 250 milliseconds
// at 44100 Hz this is (44100 * 250) / 1000 = 11025 samples
#define MIDI_PARSER_TIMEOUT_SAMPLES 11025

// MIDI CC events for controller knobs/sliders
#define ARTURIA_MINILAB_FADER_1 82

// MIDI events for key presses
#define ARTURIA_MINILAB_LOWEST_NOTE 48
#define ARTURIA_MINILAB_HIGHEST_NOTE 72
#define ARTURIA_MINILAB_NOTES_COUNT 25

// From C4 (MIDI #60) to C6 (MIDI #84)
SoundFontNotePlayback violinPlayback[ARTURIA_MINILAB_NOTES_COUNT] = {
  // MIDI Note 60
  {
    .soundFontNote = &(violinSoundFontNotes[6])
  },
  // MIDI Note 61
  {
    .soundFontNote = &(violinSoundFontNotes[7])
  },
  // MIDI Note 62
  {
    .soundFontNote = &(violinSoundFontNotes[8])
  },
  // MIDI Note 63
  {
    .soundFontNote = &(violinSoundFontNotes[9])
  },
  // MIDI Note 64
  {
    .soundFontNote = &(violinSoundFontNotes[10])
  },
  // MIDI Note 65
  {
    .soundFontNote = &(violinSoundFontNotes[11])
  },
  // MIDI Note 66
  {
    .soundFontNote = &(violinSoundFontNotes[12])
  },
  // MIDI Note 67
  {
    .soundFontNote = &(violinSoundFontNotes[13])
  },
  // MIDI Note 68
  {
    .soundFontNote = &(violinSoundFontNotes[14])
  },
  // MIDI Note 69
  {
    .soundFontNote = &(violinSoundFontNotes[15])
  },
  // MIDI Note 70
  {
    .soundFontNote = &(violinSoundFontNotes[16])
  },
  // MIDI Note 71
  {
    .soundFontNote = &(violinSoundFontNotes[17])
  },
  // MIDI Note 72
  {
    .soundFontNote = &(violinSoundFontNotes[18])
  },
  // MIDI Note 73
  {
    .soundFontNote = &(violinSoundFontNotes[19])
  },
  // MIDI Note 74
  {
    .soundFontNote = &(violinSoundFontNotes[20])
  },
  // MIDI Note 75
  {
    .soundFontNote = &(violinSoundFontNotes[21])
  },
  // MIDI Note 76
  {
    .soundFontNote = &(violinSoundFontNotes[22])
  },
  // MIDI Note 77
  {
    .soundFontNote = &(violinSoundFontNotes[23])
  },
  // MIDI Note 78
  {
    .soundFontNote = &(violinSoundFontNotes[24])
  },
  // MIDI Note 79
  {
    .soundFontNote = &(violinSoundFontNotes[25])
  },
  // MIDI Note 80
  {
    .soundFontNote = &(violinSoundFontNotes[26])
  },
  // MIDI Note 81
  {
    .soundFontNote = &(violinSoundFontNotes[27])
  },
  // MIDI Note 82
  {
    .soundFontNote = &(violinSoundFontNotes[28])
  },
  // MIDI Note 83
  {
    .soundFontNote = &(violinSoundFontNotes[29])
  },
  // MIDI Note 84
  {
    .soundFontNote = &(violinSoundFontNotes[29])
  }
};

unsigned char outputVolume;

MIDIParser midiParser;

void handleKnobUpdate(unsigned char knob, unsigned char value)
{
  // Sanity check
  if (value > 127) {
    return;
  }

  if (knob == ARTURIA_MINILAB_FADER_1) {
    outputVolume = value;
  }
}

void handleNoteOn(unsigned char note, unsigned char velocity)
{
  unsigned int scaled_velocity = 33818640;
  scaled_velocity *= velocity;
  if ((note >= ARTURIA_MINILAB_LOWEST_NOTE) && (note <= ARTURIA_MINILAB_HIGHEST_NOTE)) {
    press_SoundFontNotePlayback(&(violinPlayback[note - ARTURIA_MINILAB_LOWEST_NOTE]), scaled_velocity);
  }
}

void handleNoteOff(unsigned char note)
{
  if ((note >= ARTURIA_MINILAB_LOWEST_NOTE) && (note <= ARTURIA_MINILAB_HIGHEST_NOTE)) {
    release_SoundFontNotePlayback(&(violinPlayback[note - ARTURIA_MINILAB_LOWEST_NOTE]));
  }
}

void handleMidi()
{
  if (uartAvailable()) {
    feedMidiParser(&midiParser, uartRead());
    if (midiParser.message_ready) {
      if (midiParser.message_type == MIDI_MESSAGE_TYPE_CONTROL_CHANGE) {
        handleKnobUpdate(midiParser.message_control_change.controller_number, midiParser.message_control_change.controller_value);
      } else if (midiParser.message_type == MIDI_MESSAGE_TYPE_NOTE_ON) {
        handleNoteOn(midiParser.message_note_on.note, midiParser.message_note_on.velocity);
      } else if (midiParser.message_type == MIDI_MESSAGE_TYPE_NOTE_OFF) {
        handleNoteOff(midiParser.message_note_off.note);
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

  // Initialize the SoundFont - all keys should start in their released position
  for (unsigned int i = 0; i < ARTURIA_MINILAB_NOTES_COUNT; i++) {
    release_SoundFontNotePlayback(&(violinPlayback[i]));
  }

  outputVolume = 0;

  // Direct (non-FIFO) mode to avoid buffer underrun noise
  headphoneDirectOutputInit();

}

void loop()
{
  // Run this loop at 44.1 kHz
  unsigned int loopStartTime = micros();

  /*
   * Generate the audio
   * Mixing the output of all synth voices here is CPU intensive and a
   * glitch where notes sound detuned can occur when multiple notes are
   * played at once. Adjusting the loop so that only a subset of the notes
   * are read and mixed can help alleviate this problem.
   * 
   * For future work, support should be added so that only N synth voices
   * can be active at a given moment and only these N synth voices are
   * added to the mix.
   */
  int outputSample = 0;
  for (unsigned int i = 0; i < ARTURIA_MINILAB_NOTES_COUNT; i++) {
    outputSample += readSample_SoundFontNotePlayback(&(violinPlayback[i]));
  }

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

  // Send the processed sample to the headphone jack
  headphoneDirectOutputWriteSigned(outputSample);
}
