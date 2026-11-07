#ifndef SOUNDFONT_H
#define SOUNDFONT_H

typedef struct {
	int* samples;
	unsigned int length;
	unsigned int loop_start;
	unsigned int loop_length;
	unsigned int steps_per_sample;
	unsigned int release_decay_rate_32bit; // A value of 4294967295 means no change. A lower value means envelope decay.
} SoundFontNote;

typedef struct {
	SoundFontNote* soundFontNote;
	unsigned int position1000;
	unsigned int amplitude_32bit;
	unsigned int envelope_change_rate_32bit; // A value of 4294967295 means no change. A lower value means envelope decay.
	unsigned char is_playing;
	unsigned char loop_enabled;
} SoundFontNotePlayback;

int readSample_SoundFontNotePlayback(SoundFontNotePlayback* playback);
void press_SoundFontNotePlayback(SoundFontNotePlayback* playback, unsigned int amplitude_32bit);
void release_SoundFontNotePlayback(SoundFontNotePlayback* playback);

#endif /* SOUNDFONT_H */
