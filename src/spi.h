#define AUX_SPI0_CS_REGISTER 0x20204000
#define AUX_SPI0_FIFO_REGISTER 0x20204004
#define AUX_SPI0_CLK_REGISTER 0x20204008

#define MASK_SPI_ENABLE 0x02
#define MASK_SPI_CLEAR_TX_FIFO 0x10
#define MASK_SPI_CLEAR_RX_FIFO 0x20
#define MASK_SPI_TRANSFER_ACTIVE 0x80
#define MASK_SPI_DONE 0x10000
#define MASK_SPI_RXD 0x20000
#define MASK_SPI_TXD 0x40000

void spiInit();
void spiSetClockDivisor(unsigned int);
void spiBeginTransaction();
unsigned char spiTransfer(unsigned char);
void spiEndTransaction();
