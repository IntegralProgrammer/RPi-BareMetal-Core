#define MIDI_MESSAGE_TYPE_NOTE_OFF 0x80
#define MIDI_MESSAGE_TYPE_NOTE_ON 0x90
#define MIDI_MESSAGE_TYPE_POLYPHONIC_AFTERTOUCH 0xA0
#define MIDI_MESSAGE_TYPE_CONTROL_CHANGE 0xB0
#define MIDI_MESSAGE_TYPE_PROGRAM_CHANGE 0xC0
#define MIDI_MESSAGE_TYPE_CHANNEL_AFTERTOUCH 0xD0
#define MIDI_MESSAGE_TYPE_PITCH_WHEEL 0xE0

#define MIDI_MESSAGE_MASK_TYPE 0xF0
#define MIDI_MESSAGE_MASK_CHANNEL 0x0F

typedef struct {
  unsigned char note;
  unsigned char velocity;
} MIDIMessageNoteOff;

typedef struct {
  unsigned char note;
  unsigned char velocity;
} MIDIMessageNoteOn;

typedef struct {
  unsigned char note;
  unsigned char pressure;
} MIDIMessagePolyphonicAftertouch;

typedef struct {
  unsigned char controller_number;
  unsigned char controller_value;
} MIDIMessageControlChange;

typedef struct {
  unsigned char program_number;
} MIDIMessageProgramChange;

typedef struct {
  unsigned char pressure;
} MIDIMessageChannelAftertouch;

typedef struct {
  unsigned int value;
} MIDIMesssagePitchWheel;

typedef struct {
  unsigned int timeout_initial_value;
  unsigned int timeout_reset_countdown;
  unsigned char message_ready;
  unsigned char message_byte_position;
  unsigned char message_type;
  unsigned char channel;
  union {
    MIDIMessageNoteOff message_note_off;
    MIDIMessageNoteOn message_note_on;
    MIDIMessagePolyphonicAftertouch message_polyphonic_aftertouch;
    MIDIMessageControlChange message_control_change;
    MIDIMessageProgramChange message_program_change;
    MIDIMessageChannelAftertouch message_channel_aftertouch;
    MIDIMesssagePitchWheel message_pitch_wheel;
  };
} MIDIParser;


void initMidiParser(MIDIParser*, unsigned int);
void feedMidiParser(MIDIParser*, unsigned char);
void countdownMidiParser(MIDIParser*);
