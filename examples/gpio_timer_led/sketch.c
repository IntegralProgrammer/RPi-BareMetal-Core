#include "../../src/gpio.h"
#include "../../src/timer.h"

#define LED_PIN 25

void delayMicros(unsigned int time)
{
    unsigned int start = micros();
    while ((micros() - start) < time) {}
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    // Turn the LED on
    digitalWrite(LED_PIN, HIGH);

    // Wait 5 seconds
    delayMicros(5000000);

    // Turn the LED off
    digitalWrite(LED_PIN, LOW);

    // Wait 5 seconds
    delayMicros(5000000);
}
