#include "../../src/spi.h"
#include "../../src/uart.h"

const char* greetingMessage = "RPi-BareMetal-Core SPI MCP3201 DEMO\r\n\n";
const char* loopMessage = "Press any key to read from the MCP3201\r\n";
const char* digitChars = "0123456789";

void setup()
{
  uartInit(115200);
  spiInit();

  // Print the greeting message
  uartPrint((char*)greetingMessage);
}

void loop()
{
  // User prompt
  uartPrint((char*)loopMessage);

  // Wait for input
  while (uartAvailable() == 0) {}
  uartRead();

  // 16 bits SPI transaction to read from the MCP3201
  spiBeginTransaction();
  unsigned int analogValue = spiTransfer(0x00); // Value sent here doesn't matter since MOSI is not connected to the MCP3201
  analogValue = analogValue & 0x1F;
  analogValue = analogValue << 7;
  analogValue = analogValue | ((spiTransfer(0x00) & 0xFE) >> 1); // Value sent here doesn't matter since MOSI is not connected to the MCP3201
  spiEndTransaction();

  // Only the last 12 bits contain valid data
  analogValue = analogValue & 0x0FFF;

  // Show the data that was received in decimal format (thousands, hundreds, tens, ones)
  // Thousands
  uartWrite((char)digitChars[(analogValue / 1000) % 10]);

  // Hundreds
  uartWrite((char)digitChars[(analogValue / 100) % 10]);

  // Tens
  uartWrite((char)digitChars[(analogValue / 10) % 10]);

  // Ones
  uartWrite((char)digitChars[(analogValue / 1) % 10]);
  uartWrite('\r');
  uartWrite('\n');
}
