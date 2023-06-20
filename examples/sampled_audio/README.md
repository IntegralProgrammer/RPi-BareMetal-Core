Simple program which plays an audio sample (single channel, 16-bit, 44.1 KHz) on an infinite loop through the Raspberry Pi's headphone jack

To create the `audio_clip.h` file:

- First convert the input audio clip to 16 bit, 44.1 KHz, single channel, unsigned, raw PCM data

```bash
ffmpeg -i input_file.wav -acodec pcm_u16le -f u16le -ac 1 -ar 44100 audio_clip.pcm
```

- Then use `utilities/binary_file_to_16bit.py` to generate `audio_clip.h`

```bash
python3 binary_file_to_16bit.py --binary_file audio_clip.pcm --header_file audio_clip.h --data_name audio_clip
```
