#include "../../src/gpio.h"
#include "../../src/clock.h"

void setup()
{
    // Detect rising edges on GPIO 23
    pinMode(23, INPUT);
    detectAsyncRisingEdge(23);

    // Clock signal (GPCLK0) on GPIO 4
    pinMode(4, GPIO_ALT_FUNCTION_0);

    // Set the GPCLK0 clock divisor
    // We start with a 19.2 MHz clock and divide by 25 to get a 768 KHz clock
    setClockDivisor(CM_GP0DIV, 25, 0);

    // Set the GPCLK0 clock source to the 19.2 MHz oscillator
    setClockSource(CM_GP0CTL, CLK_SRC_ROOT_19_2MHZ);

    // Enable the GPCLK0 clock
    enableClock(CM_GP0CTL);

    // LED is on GPIO 24
    pinMode(24, OUTPUT);
}

void loop()
{
    // Turn the LED off - it should stay off for 10 seconds
    digitalWrite(24, LOW);

    // Wait for 7,680,000 cycles, this is 10 seconds
    for (unsigned int i = 0; i < 7680000; i++) {
        waitForEvent(23);
    }

    // Turn the LED on - it should stay on for 10 seconds
    digitalWrite(24, HIGH);

    // Wait for 7,680,000 cycles, this is 10 seconds
    for (unsigned int i = 0; i < 7680000; i++) {
        waitForEvent(23);
    }
}
