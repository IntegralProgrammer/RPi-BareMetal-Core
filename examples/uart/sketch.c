#include "../../src/uart.h"

const char* greetingMessage = "RPi-BareMetal-Core UART DEMO\r\nAny keys entered will be echoed back\r\n\n";

void setup()
{
  uartInit(115200);

  // Print the greeting message
  uartPrint((char*)greetingMessage);
}

void loop()
{
  // Echo back any received characters
  if (uartAvailable()) {
    uartWrite(uartRead());
  }
}
