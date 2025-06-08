#include "../../src/spi.h"
#include "../../src/timer.h"
#include "../../src/headphonejack.h"

// Period of 44.1 kHz signal is approximately 23 uS long
#define LOOP_PERIOD_US 23

int readMCP3301()
{
  unsigned int rawValue;
  unsigned int rawValueTwosCompliment;
  int positiveAnalogValue;
  int negativeAnalogValue;

  // 16 bits SPI transaction to read from the MCP3301
  spiBeginTransaction();
  rawValue = spiTransfer(0x00); // Value sent here doesn't matter since MOSI is not connected to the MCP3301
  rawValue = rawValue & 0x1F;
  rawValue = rawValue << 8;
  rawValue = rawValue | (spiTransfer(0x00) & 0xFF); // Value sent here doesn't matter since MOSI is not connected to the MCP3301
  spiEndTransaction();

  // Only the last 13 bits contain valid data
  rawValue = rawValue & 0x1FFF;

  // Positive version of rawValue
  positiveAnalogValue = rawValue & 0xFFF;

  // Calculate the negative version of rawValue
  rawValueTwosCompliment = (((~rawValue) + 1) & 0xFFF);
  negativeAnalogValue = -1 * rawValueTwosCompliment;

  // Based on the SIGN bit, return either positiveAnalogValue or negativeAnalogValue
  if ((rawValue >> 12) & 0x01) {
    // Sign bit is set therefore value is negative
    return negativeAnalogValue;
  } else {
    // Sign bit is clear therefore value is positive
    return positiveAnalogValue;
  }
}

void headphoneDirectOutputWriteSigned(int val)
{
  unsigned int unsignedVal = 32768;
  unsignedVal += val;

  // Write the unsigned value to the headphone port
  headphoneDirectOutputWrite(unsignedVal);
}

void setup()
{
  spiInit();
  spiSetClockDivisor(250); // SYSCLOCK = 250MHz -> SYSCLOCK/250 = 1MHz

  // Direct (non-FIFO) mode to avoid buffer underrun noise
  headphoneDirectOutputInit();
}

void loop()
{
  // Run this loop at 44.1 kHz
  unsigned int loopStartTime = micros();

  // Read from the MCP3301 ADC and send the sample to the headphone jack PWM DAC
  // Multiply by 8 to scale the 13-bit range to a 16-bit range
  headphoneDirectOutputWriteSigned(8 * readMCP3301());

  // Wait if necessary to ensure that this loop doesn't run any faster than 44.1 kHz
  while ((loopStartTime - micros()) < LOOP_PERIOD_US) {}
}
