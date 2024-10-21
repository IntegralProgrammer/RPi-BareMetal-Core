#include "common.h"
#include "gpio.h"
#include "spi.h"

extern unsigned int GET32(unsigned int);
extern void PUT32(unsigned int, unsigned int);

void spiInit()
{
  // Enable the SPI interface
  PUT32(AUX_ENABLES_REGISTER, GET32(AUX_ENABLES_REGISTER) | MASK_SPI_ENABLE);

  // Set GPIOs 7,8,9,10,11 into SPI alt function mode
  pinMode(7, GPIO_ALT_FUNCTION_0); // CHIP SELECT 1
  pinMode(8, GPIO_ALT_FUNCTION_0); // CHIP SELECT 0
  pinMode(9, GPIO_ALT_FUNCTION_0); // MISO
  pinMode(10, GPIO_ALT_FUNCTION_0); // MOSI
  pinMode(11, GPIO_ALT_FUNCTION_0); // SCLK

  // Clear the TX and RX SPI FIFO buffers
  PUT32(AUX_SPI0_CS_REGISTER, MASK_SPI_CLEAR_TX_FIFO | MASK_SPI_CLEAR_RX_FIFO);

  // Configure Clock - use slowest clock speed (for now)
  PUT32(AUX_SPI0_CLK_REGISTER, 0x00);
}

void spiBeginTransaction()
{
  // Clear RX/TX buffers and set TRANSFER_ACTIVE
  PUT32(AUX_SPI0_CS_REGISTER, GET32(AUX_SPI0_CS_REGISTER) | MASK_SPI_CLEAR_TX_FIFO | MASK_SPI_CLEAR_RX_FIFO | MASK_SPI_TRANSFER_ACTIVE);
}

unsigned char spiTransfer(unsigned char txData)
{
  // Ensure that the TX FIFO has space for this byte
  while ((GET32(AUX_SPI0_CS_REGISTER) & MASK_SPI_TXD) == 0x00) {}

  // Send the byte
  PUT32(AUX_SPI0_FIFO_REGISTER, txData);

  // Wait for one byte in the RX FIFO
  while ((GET32(AUX_SPI0_CS_REGISTER) & MASK_SPI_RXD) == 0x00) {}

  // Return the received byte
  return GET32(AUX_SPI0_FIFO_REGISTER) & 0xFF;
}

void spiEndTransaction()
{
  // Transfer is finished, therefore clear TRANSFER_ACTIVE bit
  PUT32(AUX_SPI0_CS_REGISTER, GET32(AUX_SPI0_CS_REGISTER) & (~MASK_SPI_TRANSFER_ACTIVE));
}
