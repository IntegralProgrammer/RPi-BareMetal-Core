#include "gpio.h"
#include "uart.h"

extern unsigned int GET32(unsigned int);
extern void PUT32(unsigned int, unsigned int);

unsigned int lookupBaudRegValue(unsigned int baudrate)
{
  // Currently only supporting 115200 for now
  // Value is calculated as (SYSCLK_FREQ / (8 * baudrate)) - 1;
  return 270;
}

void uartInit(unsigned int baudrate)
{
  // Set GPIO14 / GPIO15 into UART alt function mode
  pinMode(14, GPIO_ALT_FUNCTION_5);
  pinMode(15, GPIO_ALT_FUNCTION_5);

  // Disable pull-up/pull-down on GPIO14 / GPIO15
  setPullUpDown(14, PULL_UP_DOWN_DISABLED);
  setPullUpDown(15, PULL_UP_DOWN_DISABLED);

  // Enable the mini UART peripheral
  PUT32(AUX_ENABLES_REGISTER, GET32(AUX_ENABLES_REGISTER) | MASK_MINI_UART_ENABLE);

  // Disable auto flow control and rx/tx (for now)
  PUT32(AUX_MU_CNTL_REGISTER, 0x00);

  // Disable mini UART interrupts
  PUT32(AUX_MU_IER_REGISTER, 0x00);

  // UART will use 8-bit mode
  PUT32(AUX_MU_LCR_REGISTER, MASK_MINI_UART_8_BIT_MODE);

  // Set the UART RTS line to HIGH
  PUT32(AUX_MU_MCR_REGISTER, 0);

  // Set the baudrate
  PUT32(AUX_MU_BAUD_REGISTER, lookupBaudRegValue(baudrate));

  // Enable mini UART TX and RX
  PUT32(AUX_MU_CNTL_REGISTER, (MASK_MINI_UART_ENABLE_RX | MASK_MINI_UART_ENABLE_TX));
}

void uartWrite(char c)
{
  // ... wait until we are clear to send ...
  while((GET32(AUX_MU_LSR_REGISTER) & MASK_MINI_UART_TX_READY) == 0) {}

  // ... clear to send, therefore send the data
  PUT32(AUX_MU_IO_REGISTER, c);
}

void uartPrint(char* str)
{
  unsigned int char_index = 0;
  while (1) {
    char c = *(str + char_index);
    if (c == 0) {
      break;
    }
    uartWrite(c);
    char_index++;
  }
}

char uartRead()
{
  return GET32(AUX_MU_IO_REGISTER) & 0xFF;
}

unsigned char uartAvailable()
{
  if (GET32(AUX_MU_LSR_REGISTER) & MASK_MINI_UART_RX_READY) {
    return 1;
  }
  return 0;
}
