#include "../../src/gpio.h"

void setup()
{
    // Detect rising edges on GPIO 23
    pinMode(23, INPUT);
    detectAsyncRisingEdge(23);

    // LED is on GPIO 24
    pinMode(24, OUTPUT);
}

void loop()
{
    // Turn the LED off
    digitalWrite(24, LOW);

    // Keep waiting until the button is pressed
    waitForEvent(23);

    // Turn the LED on
    digitalWrite(24, HIGH);

    // Keep waiting until the button is pressed
    waitForEvent(23);
}
