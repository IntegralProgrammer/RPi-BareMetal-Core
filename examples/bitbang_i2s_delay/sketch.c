#include "../../src/headphonejack.h"
#include "../../src/clock.h"
#include "../../src/gpio.h"


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

#define EFFECT_SWITCH_PIN 7

// ~0.5 seconds of audio
#define CIRCULAR_AUDIO_BUFFER_LENGTH 8000

void setup()
{

    // Enable / Disable the effect by reading EFFECT_SWITCH_PIN
    pinMode(EFFECT_SWITCH_PIN, INPUT);

    // Read bits from DOUT_PIN
    pinMode(DOUT_PIN, INPUT);

    // Detect rising edges on LRCK_PIN
    pinMode(LRCK_PIN, INPUT);
    detectAsyncRisingEdge(LRCK_PIN);

    // Detect rising edges on BCLK_PIN
    pinMode(BCLK_PIN, INPUT);
    detectAsyncRisingEdge(BCLK_PIN);

    // Put a 16 KHz * 256 = 4.096 MHz clock on SCKI_PIN
    pinMode(SCKI_PIN, GPIO_ALT_FUNCTION_0);
    setClockDivisor(CM_GP0DIV, 122, 0); // 500 MHz divided by 122 ~= 4.098 MHz
    setClockSource(CM_GP0CTL, CLK_SRC_PLLD_500MHZ);
    enableClock(CM_GP0CTL);

    // Output audio on headphone jack - simple passthrough
    headphoneDirectOutputInit();
}

unsigned int readSample()
{
    unsigned int sample = 0;
    unsigned int pin_val;

    // Clear any previous BCLK events
    clearEvent(BCLK_PIN);

    // Skip over the first BCLK
    waitForEvent(BCLK_PIN);

    for (unsigned int i = 24; i > 0; i--) {
        waitForEvent(BCLK_PIN);
        pin_val = digitalRead(DOUT_PIN);
        sample |= (pin_val << (i-1));
    }
    return sample;
}

int convertToSigned16(unsigned int signed24)
{
    if (signed24 < 8388608) {
        // Number is positive
        return signed24 >> 8;
    } else {
        // Number is negative
        unsigned int absvalue = (((~signed24) & 0xffffff) + 1) & 0xffffff;
        return -1 * (absvalue >> 8);
    }
}


int circular_audio_buffer[CIRCULAR_AUDIO_BUFFER_LENGTH];
unsigned int circular_audio_buffer_index = 0;

void circular_audio_buffer_write(int sample, unsigned int offset)
{
    circular_audio_buffer[(circular_audio_buffer_index + offset) % CIRCULAR_AUDIO_BUFFER_LENGTH] += sample;
}

int circular_audio_buffer_read()
{
    int val = circular_audio_buffer[circular_audio_buffer_index];
    circular_audio_buffer[circular_audio_buffer_index] = 0;
    circular_audio_buffer_index = (circular_audio_buffer_index + 1) % CIRCULAR_AUDIO_BUFFER_LENGTH;
    return val;
}

void loop()
{
    // Wait for LRCK to rise
    waitForEvent(LRCK_PIN);

    // Read the 24 bit signed audio sample
    unsigned int audio_sample_24_signed = readSample();

    // Convert it to signed 16 bit
    int audio_sample_16_signed = convertToSigned16(audio_sample_24_signed);

    // Add the audio to the circular buffer
    circular_audio_buffer_write(audio_sample_16_signed, 0);

    // Only apply the effect if it is switched on
    if (digitalRead(EFFECT_SWITCH_PIN) == HIGH) {
        // Add a delayed version of the audio, at 75% of the original volume, 4000 samples (~250 ms) later
        circular_audio_buffer_write((3 * audio_sample_16_signed) / 4, 4000);
    }

    // Output the circular buffer to the headphone jack
    headphoneDirectOutputWrite(circular_audio_buffer_read() + 32768);
}
