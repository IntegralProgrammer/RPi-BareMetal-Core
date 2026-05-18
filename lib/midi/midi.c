#include "midi.h"

void initMidiParser(MIDIParser* mp, unsigned int timeout) {
  mp->message_ready = 0;
  mp->message_byte_position = 0;
  mp->timeout_initial_value = timeout;
  mp->timeout_reset_countdown = timeout;
}

void feedMidiParser(MIDIParser* mp, unsigned char c) {
  mp->timeout_reset_countdown = mp->timeout_initial_value;
  if (mp->message_byte_position == 0) {
    mp->message_ready = 0;
    mp->message_type = c & MIDI_MESSAGE_MASK_TYPE;
    mp->channel = c & MIDI_MESSAGE_MASK_CHANNEL;
    mp->message_byte_position = 1;
  } else if (mp->message_byte_position == 1) {
    if (mp->message_type == MIDI_MESSAGE_TYPE_NOTE_OFF) {
      mp->message_note_off.note = c;
      mp->message_byte_position = 2;
    } else if (mp->message_type == MIDI_MESSAGE_TYPE_NOTE_ON) {
      mp->message_note_on.note = c;
      mp->message_byte_position = 2;
    } else if (mp->message_type == MIDI_MESSAGE_TYPE_POLYPHONIC_AFTERTOUCH) {
      mp->message_polyphonic_aftertouch.note = c;
      mp->message_byte_position = 2;
    } else if (mp->message_type == MIDI_MESSAGE_TYPE_CONTROL_CHANGE) {
      mp->message_control_change.controller_number = c;
      mp->message_byte_position = 2;
    } else if (mp->message_type == MIDI_MESSAGE_TYPE_PROGRAM_CHANGE) {
      mp->message_program_change.program_number = c;
      mp->message_byte_position = 0;
      mp->message_ready = 1;
    } else if (mp->message_type == MIDI_MESSAGE_TYPE_CHANNEL_AFTERTOUCH) {
      mp->message_channel_aftertouch.pressure = c;
      mp->message_byte_position = 0;
      mp->message_ready = 1;
    } else if (mp->message_type == MIDI_MESSAGE_TYPE_PITCH_WHEEL) {
      mp->message_pitch_wheel.value = c;
      mp->message_byte_position = 2;
    } else {
      // If the message_type is invalid, we must wait until the timeout occurs
      mp->message_byte_position = 255;
    }
  } else if (mp->message_byte_position == 2) {
    if (mp->message_type == MIDI_MESSAGE_TYPE_NOTE_OFF) {
      mp->message_note_off.velocity = c;
    } else if (mp->message_type == MIDI_MESSAGE_TYPE_NOTE_ON) {
      mp->message_note_on.velocity = c;
    } else if (mp->message_type == MIDI_MESSAGE_TYPE_POLYPHONIC_AFTERTOUCH) {
      mp->message_polyphonic_aftertouch.pressure = c;
    } else if (mp->message_type == MIDI_MESSAGE_TYPE_CONTROL_CHANGE) {
      mp->message_control_change.controller_value = c;
    } else if (mp->message_type == MIDI_MESSAGE_TYPE_PITCH_WHEEL) {
      mp->message_pitch_wheel.value |= ((unsigned int) c) << 8;
    }
    mp->message_ready = 1;
    mp->message_byte_position = 0;
  }
}

void countdownMidiParser(MIDIParser* mp) {
  if ((mp->timeout_reset_countdown) > 0) {
    mp->timeout_reset_countdown -= 1;
  }
  if ((mp->timeout_reset_countdown) == 0) {
    mp->message_byte_position = 0;
  }
}
