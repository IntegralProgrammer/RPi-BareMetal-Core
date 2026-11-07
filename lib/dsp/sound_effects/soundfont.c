#include "soundfont.h"

unsigned int wrapPosition1000_SoundFontNote(SoundFontNote* soundFontNote, unsigned int position1000, unsigned char enable_wrap) {
	unsigned int wrapped_position1000 = position1000;
	while (enable_wrap && ((wrapped_position1000 / 1000) >= (soundFontNote->loop_start + soundFontNote->loop_length))) {
		wrapped_position1000 -= 1000 * soundFontNote->loop_length;
	}
	return wrapped_position1000;
}

unsigned int wrapIndex_SoundFontNote(SoundFontNote* soundFontNote, unsigned int i, unsigned char enable_wrap) {
	unsigned int wrapped_i = i;
	while (enable_wrap && (wrapped_i >= (soundFontNote->loop_start + soundFontNote->loop_length))) {
		wrapped_i -= soundFontNote->loop_length;
	}
	return wrapped_i;
}

int get_SoundFontNote_sample_nonlooping(SoundFontNote* soundFontNote, unsigned int i) {
	if (i < soundFontNote->length) {
		return soundFontNote->samples[i];
	}
	return 0;
}

int get_SoundFontNote_sample1000(SoundFontNote* soundFontNote, unsigned int i, unsigned char loop_enabled) {
	// Get linearly interpolated sample
	unsigned int i_floor = i / 1000;
	unsigned int i_ceil = i_floor + 1;
	unsigned int distance_from_lower = i - (i_floor * 1000);
	unsigned int distance_from_upper = 1000 - distance_from_lower;
	int lower_sample = get_SoundFontNote_sample_nonlooping(soundFontNote, wrapIndex_SoundFontNote(soundFontNote, i_floor, loop_enabled));
	int upper_sample = get_SoundFontNote_sample_nonlooping(soundFontNote, wrapIndex_SoundFontNote(soundFontNote, i_ceil, loop_enabled));

	// A greater distance from the upper index (and therefore a smaller distance from lower index) means the lower sample is emphasized
	lower_sample *= distance_from_upper;

	// A greater distance from the lower index (and therefore a smaller distance from the upper index) means the upper sample is emphasized
	upper_sample *= distance_from_lower;

	// Mix together and return the two weighted samples
	int mixed = lower_sample + upper_sample;
	mixed /= 1000;
	return mixed;
}

int readSample_SoundFontNotePlayback(SoundFontNotePlayback* playback) {
	int output = 0;
	// Return silence if this note isn't playing
	if (playback->is_playing == 0) {
		return output;
	}

	// Get the current sample value
	output = get_SoundFontNote_sample1000(playback->soundFontNote, playback->position1000, (playback->loop_enabled && (playback->soundFontNote->loop_length > 0)));

	// Scale the output based on the amplitude envelope
	// `output` is 16-bit signed integer, therefore is in range [-32768, 32767]
	// It is held by a 32-bit signed integer with a range of [-2147483648, 2147483647]
	// Therefore it is safe to multiply `output` by 65535 since the result will always be in the 32-bit range
	unsigned int scale_16bit = playback->amplitude_32bit >> 16;
	output *= scale_16bit;
	output /= 65535;

	// Update the amplitude envelope
	unsigned long long updated_envelope = playback->amplitude_32bit;
	updated_envelope *= playback->envelope_change_rate_32bit;
	updated_envelope /= 4294967295;
	playback->amplitude_32bit = updated_envelope;

	// Step forward for the given amount of millisamples, looping back to the start of the loop when required
	playback->position1000 = wrapPosition1000_SoundFontNote(playback->soundFontNote, playback->position1000 + playback->soundFontNote->steps_per_sample, playback->loop_enabled && (playback->soundFontNote->loop_length > 0));

	if ((playback->position1000) >= (1000 * playback->soundFontNote->length)) {
		// Note is finished
		playback->is_playing = 0;
	}

	return output;
}

void press_SoundFontNotePlayback(SoundFontNotePlayback* playback, unsigned int amplitude_32bit) {
	playback->position1000 = 0;
	playback->amplitude_32bit = amplitude_32bit;
	playback->envelope_change_rate_32bit = 4294967295;
	playback->is_playing = 1;
	playback->loop_enabled = 1;
}

void release_SoundFontNotePlayback(SoundFontNotePlayback* playback) {
	playback->loop_enabled = 0;
	playback->envelope_change_rate_32bit = playback->soundFontNote->release_decay_rate_32bit;
}
