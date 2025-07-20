#include "mcp3301.h"
#include "../../../src/spi.h"

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
