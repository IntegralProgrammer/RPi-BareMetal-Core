#include "../../src/spi.h"
#include "../../src/uart.h"

const char* greetingMessage = "RPi-BareMetal-Core SPI Loopback DEMO\r\n\n";
const char* loopMessage = "Press any key to read/write from/to the SPI interface\r\n";
const char* hexChars = "0123456789ABCDEF";
unsigned int debugCounter;

void setup()
{
  debugCounter = 0;
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

  // SPI Test sending 16 bits in a transaction
  spiBeginTransaction();
  unsigned char rxByteA = spiTransfer((debugCounter >> 8) & 0xFF);
  unsigned char rxByteB = spiTransfer(debugCounter & 0xFF);
  spiEndTransaction();

  // Show the data that was received
  uartWrite('0');
  uartWrite('x');
  uartWrite((char)hexChars[(rxByteA >> 4) & 0x0F]);
  uartWrite((char)hexChars[rxByteA & 0x0f]);
  uartWrite((char)hexChars[(rxByteB >> 4) & 0x0F]);
  uartWrite((char)hexChars[rxByteB & 0x0F]);
  uartWrite('\r');
  uartWrite('\n');

  // Increment the debugCounter
  debugCounter = (debugCounter + 1) & 0xFFFF;
}
