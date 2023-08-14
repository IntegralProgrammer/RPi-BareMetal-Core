Simple program which bit-bangs the I2S protocol to read from a PCM1802 ADC and output the sampled audio on the Raspberry Pi's headphone jack

If _GPIO 7_ is pulled to _0_, the audio is simply passed through from input to output.

If _GPIO 7_ is pulled to _1_, a delayed (~250 ms) version of the input audio signal is mixed in at 75% of the original volume.

PCM1802 based breakout boards are available online for low cost.

Audio is sampled from the _right_ audio channel on the PCM1802 at 24 bits (16 KHz) and is played back through the Raspberry Pi's headphone
jack at 14 bits (16 KHz).

## Hardware Assembly

Set the _FMT0_ and _MODE0_ and _MODE1_ jumpers on the PCM1802 board so that it operates in I2S _master_ mode at 256fs. Due to a manufacturing
issue, you may need to provide 3.3V to the jumpers (see https://www.pjrc.com/pcm1802-breakout-board-needs-hack/).

Connect +5V on the PCM1802 to +5V on the Raspberry Pi, connect GND on the PCM1802 to GND on the Raspberry Pi, connect POW on the PCM1802 to
+3.3V on the Raspberry Pi and connect the remaining signals as described in `sketch.c`.

When connecting GPIO 7 to +3.3V or to GND, use a 1k resistor.

```c
/*
* These GPIO pins are located on the P5 header (https://www.raspberrypi-spy.co.uk/2012/09/raspberry-pi-p5-header/)
* in the hope that eventually the BCM2835 I2S hardware interface could be used. As the I2S protocol is currently
* only being done in software (bit-bang), these signals can be moved over to available GPIO pins on the regular 26
* pin header as these pins are only used for generic I/O functionality.
*/
#define BCLK_PIN 28
#define LRCK_PIN 29
#define DOUT_PIN 30

/*
* This must remain as GPIO4 as clock signal generation is not available on other GPIO pins
*/
#define SCKI_PIN 4
```

Apply an input audio signal to the _RIN_ pin on the PCM1802 and connect headphones/speakers to the Raspberry Pi's headphone jack.

Connect GPIO 7 to 3.3V via 1k resistor to enable the delay effect.

Connect GPIO7 to GND via 1k resistor to disable the delay effect.

## Demo

#### Passthrough (D Major Scale)

[passthrough.webm](https://github.com/IntegralProgrammer/RPi-BareMetal-Core/assets/5349728/c34a1a3e-f976-4a85-8d1b-580e675de1c1)

#### Delay Effect Enabled (D Major Scale)

[delay.webm](https://github.com/IntegralProgrammer/RPi-BareMetal-Core/assets/5349728/08de6628-9c28-47af-b735-497ec96027eb)
