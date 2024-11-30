#include "../../src/spi.h"
#include "../../src/headphonejack.h"

unsigned int readAudioInput()
{
  unsigned int analogValue;

  // 16 bits SPI transaction to read from the MCP3201
  spiBeginTransaction();
  analogValue = spiTransfer(0x00); // Value sent here doesn't matter since MOSI is not connected to the MCP3201
  analogValue = analogValue & 0x1F;
  analogValue = analogValue << 7;
  analogValue = analogValue | ((spiTransfer(0x00) & 0xFE) >> 1); // Value sent here doesn't matter since MOSI is not connected to the MCP3201
  spiEndTransaction();

  // Only the last 12 bits contain valid data
  analogValue = analogValue & 0x0FFF;
  return analogValue;
}

void setup()
{
  spiInit();
  spiSetClockDivisor(250); // SYSCLOCK = 250MHz -> SYSCLOCK/250 = 1MHz

  headphoneOutputInit();
}

void loop()
{
  // Read the analog input value
  unsigned int audioInputValue = readAudioInput();

  // Wait for space in the output buffer queue
  while (headphoneOutputReady() == 0) {}

  // Send the sample to the headphone jack output queue (and convert 12 bit to 16 bit)
  headphoneOutputWrite(audioInputValue << 4);
}
