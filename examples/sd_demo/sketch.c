#include "../../src/sd.h"
#include "../../src/uart.h"
#include "../../lib/disklayout/mbr.h"

#define READ32UINT(ptr) ( ((unsigned int) *(ptr + 0)) << 0 | ((unsigned int) *(ptr + 1)) << 8 | ((unsigned int) *(ptr + 2)) << 16 | ((unsigned int) *(ptr + 3)) << 24 )
#define WRITE32UINT(dst, data) *(dst + 0) = ((data >> 0) & 0xff); *(dst + 1) = ((data >> 8) & 0xff); *(dst + 2) = ((data >> 16) & 0xff); *(dst + 3) = ((data >> 24) & 0xff)

const char* greetingMessage = "RPi-BareMetal-Core SD DEMO (examples/sd_demo/sketch.c)\r\n\n";
const char* hexChars = "0123456789ABCDEF";

void hexPrint8(unsigned char val) {
  uartWrite('0');
  uartWrite('x');
  uartWrite((char)hexChars[(val >> 4) & 0x0f]);
  uartWrite((char)hexChars[(val >> 0) & 0x0f]);
}

void decPrint32(unsigned int val) {
  uartWrite((char)hexChars[(val / 1000000000) % 10]);
  uartWrite((char)hexChars[(val / 100000000) % 10]);
  uartWrite((char)hexChars[(val / 10000000) % 10]);
  uartWrite((char)hexChars[(val / 1000000) % 10]);
  uartWrite((char)hexChars[(val / 100000) % 10]);
  uartWrite((char)hexChars[(val / 10000) % 10]);
  uartWrite((char)hexChars[(val / 1000) % 10]);
  uartWrite((char)hexChars[(val / 100) % 10]);
  uartWrite((char)hexChars[(val / 10) % 10]);
  uartWrite((char)hexChars[(val / 1) % 10]);
}

void uartPrintMBRPartitionTable(MBRPartitionTable* tbl) {
  uartPrint("=== BEGIN MBR Partition Table ===\r\n");
  for (unsigned int i = 0; i < 4; i++) {
    uartPrint("Number: ");
    decPrint32(i + 1);
    uartPrint("\t");
    uartPrint("start: ");
    decPrint32(tbl->partitions[i].partitionStart);
    uartPrint("\t");
    uartPrint("length: ");
    decPrint32(tbl->partitions[i].partitionLength);
    uartPrint("\r\n");
  }
  uartPrint("=== END MBR Partition Table ===\r\n");
}

void setup()
{
  SDCard sdCard;
  unsigned char mbr_block[512];
  unsigned char sd_p2_blk0[512];
  unsigned char sd_p2_blk1[512];
  unsigned int boot_count;

  uartInit(115200);

  // Print the greeting message
  uartPrint((char*)greetingMessage);

  // Initialize the EMMC interface and SD card
  if (sdInit(&sdCard)) {
    uartPrint("FATAL setup(): Failed to initialize SD card!\r\n");
    return;
  }
  uartPrint("OK setup(): Initialized SD card.\r\n");

  // Read the first block of the SD card
  if (sdReadBlockSync(&sdCard, mbr_block, 0)) {
    uartPrint("FATAL setup(): Failed to read SD card block 0!\r\n");
    return;
  }

  // Display as hex symbols the bytes of the first block on the SD card
  uartPrint("=== BEGIN SD BLOCK 0 ===\r\n");
  for (unsigned int i = 0; i < 512; i++) {
    hexPrint8(mbr_block[i]);
    uartPrint(" ");
  }
  uartPrint("\r\n=== END SD BLOCK 0 ===\r\n");

  // Display the MBR partition table on this SD card
  MBRPartitionTable sdPartitionTable;
  fillMBRPartitionTableFromSectorData(&sdPartitionTable, mbr_block);
  uartPrintMBRPartitionTable(&sdPartitionTable);

  // If partition 2 exists, display the contents of the first block of it as ASCII text
  if (sdPartitionTable.partitions[1].partitionLength > 0) {
    if (sdReadBlockSync(&sdCard, sd_p2_blk0, sdPartitionTable.partitions[1].partitionStart)) {
      uartPrint("FATAL setup(): Failed to read SD card partition 2, block 0!\r\n");
      return;
    }

    uartPrint("=== BEGIN SD Partition 2, Block 0 as ASCII text ===\r\n");
    for (unsigned int i = 0; i < 512; i++) {
      if ((sd_p2_blk0[i] >= 32) && (sd_p2_blk0[i] <= 126)) {
        // ASCII printable character
        uartWrite(sd_p2_blk0[i]);
      } else {
        // ASCII non-printable
        uartWrite('.');
      }
    }
    uartPrint("\r\n");
    uartPrint("=== END SD Partition 2, Block 0 as ASCII text ===\r\n");
  }

  // Write to the second block of partition 2 if it exists
  if (sdPartitionTable.partitions[1].partitionLength > 1) {
    if (sdReadBlockSync(&sdCard, sd_p2_blk1, sdPartitionTable.partitions[1].partitionStart + 1)) {
      uartPrint("FATAL setup(): Failed to read SD card partition 2, block 1!\r\n");
      return;
    }

    // Increment and display the boot count
    boot_count = READ32UINT(sd_p2_blk1);
    boot_count += 1;
    uartPrint("Boot count: ");
    decPrint32(boot_count);
    uartPrint("\r\n");

    // Write the incremented boot counter back to the SD card
    WRITE32UINT(sd_p2_blk1, boot_count);
    if (sdWriteBlockSync(&sdCard, sd_p2_blk1, sdPartitionTable.partitions[1].partitionStart + 1)) {
      uartPrint("FATAL setup(): Failed to write SD card partition 2, block 1!\r\n");
      return;
    }
    uartPrint("SD Write success!\r\n");
  }

  uartPrint("Done: Have a nice day :)\r\n");
}

void loop()
{
}
