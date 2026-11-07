/*
 * Based on the free Valiant Violin v2 GM Soundfont available at https://musical-artifacts.com/artifacts/1796
 */

#include "../../../lib/dsp/sound_effects/soundfont.h"
#include "samples/violin_055.h"
#include "samples/violin_058.h"
#include "samples/violin_061.h"
#include "samples/violin_064.h"
#include "samples/violin_067.h"
#include "samples/violin_070.h"
#include "samples/violin_073.h"
#include "samples/violin_076.h"
#include "samples/violin_079.h"
#include "samples/violin_082.h"
#include "samples/violin_085.h"
#include "samples/violin_088.h"
#include "samples/violin_091.h"
#include "samples/violin_094.h"

SoundFontNote violinSoundFontNotes[42] = {
	// MIDI Note 55
	{
		.samples = violin_055,
		.length = 386402, // (end + 1)
		.loop_start = 73725, // (loop_start)
		.loop_length = 26140, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 56
	{
		.samples = violin_055,
		.length = 386402, // (end + 1)
		.loop_start = 73725, // (loop_start)
		.loop_length = 26140, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 57
	{
		.samples = violin_055,
		.length = 386402, // (end + 1)
		.loop_start = 73725, // (loop_start)
		.loop_length = 26140, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 58
	{
		.samples = violin_058,
		.length = 259820, // (end + 1)
		.loop_start = 98966, // (loop_start)
		.loop_length = 7807, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 59
	{
		.samples = violin_058,
		.length = 259820, // (end + 1)
		.loop_start = 98966, // (loop_start)
		.loop_length = 7807, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 60
	{
		.samples = violin_058,
		.length = 259820, // (end + 1)
		.loop_start = 98966, // (loop_start)
		.loop_length = 7807, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 61
	{
		.samples = violin_061,
		.length = 261602, // (end + 1)
		.loop_start = 117535, // (loop_start)
		.loop_length = 25262, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 62
	{
		.samples = violin_061,
		.length = 261602, // (end + 1)
		.loop_start = 117535, // (loop_start)
		.loop_length = 25262, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 63
	{
		.samples = violin_061,
		.length = 261602, // (end + 1)
		.loop_start = 117535, // (loop_start)
		.loop_length = 25262, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 64
	{
		.samples = violin_064,
		.length = 265895, // (end + 1)
		.loop_start = 134211, // (loop_start)
		.loop_length = 17619, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 65
	{
		.samples = violin_064,
		.length = 265895, // (end + 1)
		.loop_start = 134211, // (loop_start)
		.loop_length = 17619, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 66
	{
		.samples = violin_064,
		.length = 265895, // (end + 1)
		.loop_start = 134211, // (loop_start)
		.loop_length = 17619, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 67
	{
		.samples = violin_067,
		.length = 249660, // (end + 1)
		.loop_start = 98725, // (loop_start)
		.loop_length = 8767, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 68
	{
		.samples = violin_067,
		.length = 249660, // (end + 1)
		.loop_start = 98725, // (loop_start)
		.loop_length = 8767, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 69
	{
		.samples = violin_067,
		.length = 249660, // (end + 1)
		.loop_start = 98725, // (loop_start)
		.loop_length = 8767, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 70
	{
		.samples = violin_070,
		.length = 260361, // (end + 1)
		.loop_start = 115049, // (loop_start)
		.loop_length = 8612, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 71
	{
		.samples = violin_070,
		.length = 260361, // (end + 1)
		.loop_start = 115049, // (loop_start)
		.loop_length = 8612, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 72
	{
		.samples = violin_070,
		.length = 260361, // (end + 1)
		.loop_start = 115049, // (loop_start)
		.loop_length = 8612, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 73
	{
		.samples = violin_073,
		.length = 261397, // (end + 1)
		.loop_start = 119741, // (loop_start)
		.loop_length = 8671, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 74
	{
		.samples = violin_073,
		.length = 261397, // (end + 1)
		.loop_start = 119741, // (loop_start)
		.loop_length = 8671, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 75
	{
		.samples = violin_073,
		.length = 261397, // (end + 1)
		.loop_start = 119741, // (loop_start)
		.loop_length = 8671, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 76
	{
		.samples = violin_076,
		.length = 248149, // (end + 1)
		.loop_start = 104556, // (loop_start)
		.loop_length = 8518, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 77
	{
		.samples = violin_076,
		.length = 248149, // (end + 1)
		.loop_start = 104556, // (loop_start)
		.loop_length = 8518, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 78
	{
		.samples = violin_076,
		.length = 248149, // (end + 1)
		.loop_start = 104556, // (loop_start)
		.loop_length = 8518, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 79
	{
		.samples = violin_079,
		.length = 269256, // (end + 1)
		.loop_start = 82155, // (loop_start)
		.loop_length = 32408, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 80
	{
		.samples = violin_079,
		.length = 269256, // (end + 1)
		.loop_start = 82155, // (loop_start)
		.loop_length = 32408, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 81
	{
		.samples = violin_079,
		.length = 269256, // (end + 1)
		.loop_start = 82155, // (loop_start)
		.loop_length = 32408, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 82
	{
		.samples = violin_082,
		.length = 256590, // (end + 1)
		.loop_start = 69762, // (loop_start)
		.loop_length = 16851, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 83
	{
		.samples = violin_082,
		.length = 256590, // (end + 1)
		.loop_start = 69762, // (loop_start)
		.loop_length = 16851, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 84
	{
		.samples = violin_082,
		.length = 256590, // (end + 1)
		.loop_start = 69762, // (loop_start)
		.loop_length = 16851, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 85
	{
		.samples = violin_085,
		.length = 271525, // (end + 1)
		.loop_start = 87410, // (loop_start)
		.loop_length = 8598, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 86
	{
		.samples = violin_085,
		.length = 271525, // (end + 1)
		.loop_start = 87410, // (loop_start)
		.loop_length = 8598, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 87
	{
		.samples = violin_085,
		.length = 271525, // (end + 1)
		.loop_start = 87410, // (loop_start)
		.loop_length = 8598, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 88
	{
		.samples = violin_088,
		.length = 300050, // (end + 1)
		.loop_start = 74757, // (loop_start)
		.loop_length = 25597, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 89
	{
		.samples = violin_088,
		.length = 300050, // (end + 1)
		.loop_start = 74757, // (loop_start)
		.loop_length = 25597, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 90
	{
		.samples = violin_088,
		.length = 300050, // (end + 1)
		.loop_start = 74757, // (loop_start)
		.loop_length = 25597, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 91
	{
		.samples = violin_091,
		.length = 266446, // (end + 1)
		.loop_start = 69763, // (loop_start)
		.loop_length = 33119, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 92
	{
		.samples = violin_091,
		.length = 266446, // (end + 1)
		.loop_start = 69763, // (loop_start)
		.loop_length = 33119, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 93
	{
		.samples = violin_091,
		.length = 266446, // (end + 1)
		.loop_start = 69763, // (loop_start)
		.loop_length = 33119, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 94
	{
		.samples = violin_094,
		.length = 246693, // (end + 1)
		.loop_start = 94629, // (loop_start)
		.loop_length = 24649, // (loop_end - loop_start + 1)
		.steps_per_sample = 1000,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 95
	{
		.samples = violin_094,
		.length = 246693, // (end + 1)
		.loop_start = 94629, // (loop_start)
		.loop_length = 24649, // (loop_end - loop_start + 1)
		.steps_per_sample = 1059,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	},
	// MIDI Note 96
	{
		.samples = violin_094,
		.length = 246693, // (end + 1)
		.loop_start = 94629, // (loop_start)
		.loop_length = 24649, // (loop_end - loop_start + 1)
		.steps_per_sample = 1122,
		.release_decay_rate_32bit = 4293923324 // 0.001 ** (1 / (0.644343 * 44100)) * (2 ** 32 - 1)
	}
};
