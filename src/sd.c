#include "sd.h"
#include "gpio.h"
#include "timer.h"

// Default timeout of 5 seconds
#define SD_DEFAULT_TIMEOUT_US 5000000

// SD GPIO pins
#define SD_CD_GPIO 47
#define SD_CLK_GPIO 48
#define SD_CMD_GPIO 49
#define SD_DAT0_GPIO 50
#define SD_DAT1_GPIO 51
#define SD_DAT2_GPIO 52
#define SD_DAT3_GPIO 53

// EMMC interface register addresses
#define EMMC_ARG2 0x20300000
#define EMMC_BLKSIZECNT 0x20300004
#define EMMC_ARG1 0x20300008
#define EMMC_CMDTM 0x2030000c
#define EMMC_RESP0 0x20300010
#define EMMC_RESP1 0x20300014
#define EMMC_RESP2 0x20300018
#define EMMC_RESP3 0x2030001c
#define EMMC_DATA 0x20300020
#define EMMC_STATUS 0x20300024
#define EMMC_CONTROL0 0x20300028
#define EMMC_CONTROL1 0x2030002c
#define EMMC_INTERRUPT 0x20300030
#define EMMC_IRPT_MASK 0x20300034
#define EMMC_IRPT_EN 0x20300038
#define EMMC_CONTROL2 0x2030003c

// Important bits in EMMC registers
#define MASK_EMMC_SRST_HC 0x1000000
#define MASK_EMMC_CMD_DONE 0x1
#define MASK_EMMC_DATA_DONE 0x2
#define MASK_EMMC_READ_READY 0x20
#define MASK_EMMC_WRITE_READY 0x10
#define MASK_EMMC_ERR 0x8000
#define MASK_EMMC_CLK_EN 0x4
#define MASK_EMMC_CLK_STABLE 0x2
#define MASK_EMMC_CLK_INTLEN 0x1
#define MASK_EMMC_HCTL_DWIDTH 0x2

// SD card commands
#define SD_CMD_NEED_APP 0x80000000

#define SD_CMD_GO_IDLE 0x00000000
#define SD_CMD_CMD1 0x01020000
#define SD_CMD_ALL_SEND_CID 0x02010000
#define SD_CMD_SEND_REL_ADDR 0x03020000
#define SD_CMD_CARD_SELECT 0x07030000
#define SD_CMD_SEND_IF_COND 0x08020000
#define SD_CMD_STOP_TRANS 0x0c030000
#define SD_CMD_READ_SINGLE 0x11220010
#define SD_CMD_READ_MULTI 0x12220032
#define SD_CMD_WRITE_SINGLE 0x18220000
#define SD_CMD_WRITE_MULTI 0x19220022
#define SD_CMD_SET_BLOCKCNT 0x17020000
#define SD_CMD_APP_CMD 0x37000000
#define SD_CMD_SET_BUS_WIDTH 0x06020000
#define SD_CMD_SEND_OP_COND 0x29020000
#define SD_CMD_SEND_SCR 0x33220010

#define SD_ARG_ACMD41 0x51ff8000
#define SD_COMPLETE_ACMD41 0x80000000
#define SD_CCS_ACMD41 0x40000000
#define SD_CMD_MASK_RCA 0xffff0000
#define SD_CMD_MASK_RSPNS_48 0x20000
#define SD_SCR_BUS_WIDTH_4 0x400

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

unsigned char resetEmmcInterface(unsigned int timeout_us) {
  unsigned int start_time;
  PUT32(EMMC_CONTROL0, 0x00);
  PUT32(EMMC_CONTROL1, GET32(EMMC_CONTROL1) | MASK_EMMC_SRST_HC);
  start_time = micros();
  while ((timeout_us == 0) || ((micros() - start_time) < timeout_us)) {
    if ((GET32(EMMC_CONTROL1) & MASK_EMMC_SRST_HC) == 0) {
      return SD_OK;
    }
  }
  return SD_TIMEOUT;
}

unsigned char setEmmcClockDivisor(unsigned int divisor, unsigned int timeout_us) {
  // Base frequency is 41.6 MHz
  // divisor == 104 gives 400 KHz clock
  // divisor == 4 gives 10.4 MHz clock
  unsigned int divisor_msb = (divisor >> 8) & 0x3;
  unsigned int divisor_lsb = divisor & 0xFF;
  unsigned int start_time;

  // Disable the clock
  PUT32(EMMC_CONTROL1, GET32(EMMC_CONTROL1) & ~MASK_EMMC_CLK_EN);
  // Wait 10ms
  delayUS(10000);

  // Set the clock divisor bits
  PUT32(EMMC_CONTROL1, GET32(EMMC_CONTROL1) | (divisor_msb << 6) | (divisor_lsb << 8));
  // Wait 10ms
  delayUS(10000);

  // Re-enable the clock
  PUT32(EMMC_CONTROL1, GET32(EMMC_CONTROL1) | MASK_EMMC_CLK_EN);

  // Wait for the clock to become stable
  start_time = micros();
  while ((timeout_us == 0) || ((micros() - start_time) < timeout_us)) {
    if ((GET32(EMMC_CONTROL1) & MASK_EMMC_CLK_STABLE)) {
      return SD_OK;
    }
  }
  return SD_TIMEOUT;
}

void sendSDCommand(unsigned int code, unsigned int arg) {
  // Clear the EMMC_INTERRUPT register as it is not self-resetting
  PUT32(EMMC_INTERRUPT, GET32(EMMC_INTERRUPT));

  // Send run the command(code) with its arguments(arg)
  PUT32(EMMC_ARG1, arg);
  PUT32(EMMC_CMDTM, code);
}

unsigned char sdCommandSuccess() {
  // Return true if EMMC_INTERRUPT shows MASK_EMMC_CMD_DONE
  if (GET32(EMMC_INTERRUPT) & MASK_EMMC_CMD_DONE) {
    return 1;
  }
  return 0;
}

unsigned char sdCommandFailed() {
  // Return true if EMMC_INTERRUPT shows MASK_EMMC_ERR
  if (GET32(EMMC_INTERRUPT) & MASK_EMMC_ERR) {
    return 1;
  }
  return 0;
}

unsigned char sdCommandDone() {
  // Return true if the SD command execution is finished - either in success or in failure
  if (GET32(EMMC_INTERRUPT) & (MASK_EMMC_CMD_DONE | MASK_EMMC_ERR)) {
    return 1;
  }
  return 0;
}

unsigned int getSDCommandResponse32Bit() {
  return GET32(EMMC_RESP0);
}

unsigned char runBlockingSdCommand(unsigned int code, unsigned int arg, unsigned int timeout_us) {
  unsigned int start_time;

  // Send the command to the card
  sendSDCommand(code, arg);

  // Wait for the card to execute the command
  start_time = micros();
  while ((timeout_us == 0) || ((micros() - start_time) < timeout_us)) {
    if (sdCommandDone()) {
      if (sdCommandSuccess()) {
        return SD_OK;
      } else {
        return SD_ERROR;
      }
    }
  }
  return SD_TIMEOUT;
}

unsigned char waitForSdReadReady(unsigned int timeout_us) {
  unsigned int start_time = micros();
  while ((timeout_us == 0) || ((micros() - start_time) < timeout_us)) {
    if (GET32(EMMC_INTERRUPT) & MASK_EMMC_READ_READY) {
      return SD_OK;
    }
  }
  return SD_TIMEOUT;
}

unsigned char waitForSdWriteReady(unsigned int timeout_us) {
  unsigned int start_time = micros();
  while ((timeout_us == 0) || ((micros() - start_time) < timeout_us)) {
    if (GET32(EMMC_INTERRUPT) & MASK_EMMC_WRITE_READY) {
      return SD_OK;
    }
  }
  return SD_TIMEOUT;
}

unsigned char waitSdForDataDone(unsigned int timeout_us) {
  unsigned int start_time = micros();
  while ((timeout_us == 0) || ((micros() - start_time) < timeout_us)) {
    if (GET32(EMMC_INTERRUPT) & MASK_EMMC_DATA_DONE) {
      return SD_OK;
    }
  }
  return SD_TIMEOUT;
}

unsigned char sdInit(SDCard* sdCard) {
  unsigned int cmd_resp32;
  unsigned int sd_rca;
  unsigned int sd_scr[2];

  sd_rca = 0;
  sd_scr[0] = 0;
  sd_scr[1] = 0;

  // Configure SD_CD_GPIO
  pinMode(SD_CD_GPIO, GPIO_ALT_FUNCTION_3);
  setPullUpDown(SD_CD_GPIO, PULL_UP_ENABLED);
  detectGpioHigh(SD_CD_GPIO);

  // Configure SD_CLK_GPIO
  pinMode(SD_CD_GPIO, GPIO_ALT_FUNCTION_3);
  setPullUpDown(SD_CD_GPIO, PULL_UP_ENABLED);

  // Configure SD_CMD_GPIO
  pinMode(SD_CMD_GPIO, GPIO_ALT_FUNCTION_3);
  setPullUpDown(SD_CMD_GPIO, PULL_UP_ENABLED);

  // Configure SD_DAT0_GPIO
  pinMode(SD_DAT0_GPIO, GPIO_ALT_FUNCTION_3);
  setPullUpDown(SD_DAT0_GPIO, PULL_UP_ENABLED);

  // Configure SD_DAT1_GPIO
  pinMode(SD_DAT1_GPIO, GPIO_ALT_FUNCTION_3);
  setPullUpDown(SD_DAT1_GPIO, PULL_UP_ENABLED);

  // Configure SD_DAT2_GPIO
  pinMode(SD_DAT2_GPIO, GPIO_ALT_FUNCTION_3);
  setPullUpDown(SD_DAT2_GPIO, PULL_UP_ENABLED);

  // Configure SD_DAT3_GPIO
  pinMode(SD_DAT3_GPIO, GPIO_ALT_FUNCTION_3);
  setPullUpDown(SD_DAT3_GPIO, PULL_UP_ENABLED);

  // Reset the EMMC module
  if (resetEmmcInterface(SD_DEFAULT_TIMEOUT_US)) {
    return SD_TIMEOUT;
  }

  // Configure the SD clock ~400 KHz clock for card initialization
  PUT32(EMMC_CONTROL1, GET32(EMMC_CONTROL1) | MASK_EMMC_CLK_INTLEN | (0x7 << 17));
  if (setEmmcClockDivisor(104, SD_DEFAULT_TIMEOUT_US)) {
    return SD_TIMEOUT;
  }

  // Don't generate IRQs since we're using polling, but do enable the setting of the bits in the EMMC_INTERRUPT register
  PUT32(EMMC_IRPT_EN, 0x00000000);
  PUT32(EMMC_IRPT_MASK, 0xffffffff);

  // Initialize the SD card ...

  // SD_CMD_GO_IDLE (CMD0)
  if (runBlockingSdCommand(SD_CMD_GO_IDLE, 0x00000000, SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  // SD_CMD_SEND_IF_COND (CMD8)
  if (runBlockingSdCommand(SD_CMD_SEND_IF_COND, 0x000001aa, SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  // SD_CMD_SEND_OP_COND (ACMD41) until complete
  while (1) {
    // SD_CMD_SEND_OP needs to be prefixed with the SD_CMD_APP_CMD (CMD55) command
    if (runBlockingSdCommand(SD_CMD_APP_CMD, 0, SD_DEFAULT_TIMEOUT_US)) {
      return SD_ERROR;
    }

    // A 10ms delay seems to be needed following SD_CMD_APP_CMD
    delayUS(10000);

    if (runBlockingSdCommand(SD_CMD_SEND_OP_COND, SD_ARG_ACMD41, SD_DEFAULT_TIMEOUT_US)) {
      return SD_ERROR;
    }

    cmd_resp32 = getSDCommandResponse32Bit();
    if (cmd_resp32 & SD_COMPLETE_ACMD41) {
      if (cmd_resp32 & SD_CCS_ACMD41) {
        sdCard->ccs = 1;
      } else {
        sdCard->ccs = 0;
      }
      break;
    }

    // Try again 250ms later
    delayUS(250000);
  }

  // CMD2: CMD_ALL_SEND_CID
  if (runBlockingSdCommand(SD_CMD_ALL_SEND_CID, 0, SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  // CMD3: CMD_SEND_REL_ADDR
  if (runBlockingSdCommand(SD_CMD_SEND_REL_ADDR, 0, SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }
  // Read the response from the SD card containing the relative address
  sd_rca = getSDCommandResponse32Bit() & SD_CMD_MASK_RCA;

  // TODO: Should we increase the SD Clock frequency here? 25 MHz, 10 MHz
  // We'll try 8, for 5.2 MHz for now
  if (setEmmcClockDivisor(8, SD_DEFAULT_TIMEOUT_US)) {
    return SD_TIMEOUT;
  }

  // CMD7: CMD_CARD_SELECT
  if (runBlockingSdCommand(SD_CMD_CARD_SELECT, sd_rca, SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  // Run CMD_SEND_SCR - this will send data to the EMMC_DATA_REGISTER
  // We'll be receiving 1 block of 8 bytes
  PUT32(EMMC_BLKSIZECNT, (1 << 16) | 8);

  if (runBlockingSdCommand(SD_CMD_APP_CMD | (sd_rca ? SD_CMD_MASK_RSPNS_48 : 0), sd_rca, SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  // A 10ms delay seems to be needed following SD_CMD_APP_CMD
  delayUS(10000);

  if (runBlockingSdCommand(SD_CMD_SEND_SCR, 0, SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  // Wait for READ_READY
  if (waitForSdReadReady(SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  // Read the data from the DATA register
  sd_scr[0] = GET32(EMMC_DATA);
  sd_scr[1] = GET32(EMMC_DATA);

  // Set the bus width
  if (sd_scr[0] & SD_SCR_BUS_WIDTH_4) {

    if (runBlockingSdCommand(SD_CMD_APP_CMD | (sd_rca ? SD_CMD_MASK_RSPNS_48 : 0), sd_rca, SD_DEFAULT_TIMEOUT_US)) {
      return SD_ERROR;
    }

    // A 10ms delay seems to be needed following SD_CMD_APP_CMD
    delayUS(10000);

    if (runBlockingSdCommand(SD_CMD_SET_BUS_WIDTH, (sd_rca | 2), SD_DEFAULT_TIMEOUT_US)) {
      return SD_ERROR;
    }

    PUT32(EMMC_CONTROL0, GET32(EMMC_CONTROL0) | MASK_EMMC_HCTL_DWIDTH);
  }

  return SD_OK;
}

/*
 * Reads a block (specified by address) from the SD card (*sdCard) into the buffer (*buffer)
 * *buffer MUST be 4-byte aligned
 * Function blocks until the operation has completed
 */
unsigned char sdReadBlockSync(SDCard* sdCard, unsigned char* buffer, unsigned int address) {
  unsigned int* buffer32 = (unsigned int*)buffer;

  if (((unsigned int)buffer) % 4) {
    return SD_INCORRECT_ALIGNMENT;
  }

  // We'll be receiving 1 block of 512 bytes
  PUT32(EMMC_BLKSIZECNT, (1 << 16) | 512);

  // Send the command to read from the given address
  if (runBlockingSdCommand(SD_CMD_READ_SINGLE, (sdCard->ccs ? address : (512 * address)), SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  // Wait for READ_READY
  if (waitForSdReadReady(SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  // DATA is ready
  for (unsigned int i = 0; i < 128; i++) {
    buffer32[i] = GET32(EMMC_DATA);
  }

  return SD_OK;
}

/*
 * Writes a block (specified by address) on the SD card (*sdCard) with data in the buffer (*buffer)
 * *buffer MUST be 4-byte aligned
 * Function blocks until the operation has completed
 */
unsigned char sdWriteBlockSync(SDCard* sdCard, unsigned char* buffer, unsigned int address) {
  unsigned int* buffer32 = (unsigned int*)buffer;

  if (((unsigned int)buffer) % 4) {
    return SD_INCORRECT_ALIGNMENT;
  }

  // We'll be sending 1 block of 512 bytes
  PUT32(EMMC_BLKSIZECNT, (1 << 16) | 512);

  // Send the command to write to the given address
  if (runBlockingSdCommand(SD_CMD_WRITE_SINGLE, (sdCard->ccs ? address : (512 * address)), SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  // Wait for WRITE_READY
  if (waitForSdWriteReady(SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  // DATA is ready
  for (unsigned int i = 0; i < 128; i++) {
    PUT32(EMMC_DATA, buffer32[i]);
  }

  // Wait for the transfer to finish
  if (waitSdForDataDone(SD_DEFAULT_TIMEOUT_US)) {
    return SD_ERROR;
  }

  return SD_OK;
}
