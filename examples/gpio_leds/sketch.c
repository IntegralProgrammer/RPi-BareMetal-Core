#include "../../src/gpio.h"

void setup()
{
    pinMode(23, INPUT);
    pinMode(24, OUTPUT);
}

void loop()
{
    // Simple program to have the state of the LED follow the state of the switch
    //digitalWrite(24, digitalRead(23));

    // More complicated toggle on/off program
    // Start with the LED off
    digitalWrite(24, LOW);

    // Keep waiting until the button is pressed
    while (digitalRead(23) == HIGH) {}

    // Now wait until the button is released
    while (digitalRead(23) == LOW) {}

    // Turn the LED on
    digitalWrite(24, HIGH);

    // Keep waiting until the button is pressed
    while (digitalRead(23) == HIGH) {}

    // Now wait until the button is released
    while (digitalRead(23) == LOW) {}
}
