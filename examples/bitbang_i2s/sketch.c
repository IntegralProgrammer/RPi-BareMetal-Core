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

void setup()
{

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

unsigned int convertToUnsigned16(unsigned int signed24) {
    if (signed24 < 8388608) {
        // Number is positive
        return 32768 + (signed24 >> 8);
    } else {
        // Number is negative
        unsigned int absvalue = (((~signed24) & 0xffffff) + 1) & 0xffffff;
        return 32768 - (absvalue >> 8);
    }
}

void loop()
{
    // Wait for LRCK to rise
    waitForEvent(LRCK_PIN);

    // Read the 24 bit signed audio sample
    unsigned int audio_sample_24_signed = readSample();

    // Convert it to unsigned 16 bit
    unsigned int audio_sample_16_unsigned = convertToUnsigned16(audio_sample_24_signed);

    // Output this sample
    headphoneDirectOutputWrite(audio_sample_16_unsigned);
}
