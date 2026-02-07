#include "mbr.h"

#define READ32UINT(ptr) ( ((unsigned int) *(ptr + 0)) << 0 | ((unsigned int) *(ptr + 1)) << 8 | ((unsigned int) *(ptr + 2)) << 16 | ((unsigned int) *(ptr + 3)) << 24 )

void fillMBRPartitionTableFromSectorData(MBRPartitionTable* tbl, unsigned char* mbr_data) {
  /*
   * Parse sector 0 (MBR Partition table)
   *   - Partition 1 information is at offset 0x01be
   *   - Partition 2 information is at offset 0x01ce
   *   - Partition 3 information is at offset 0x01de
   *   - Partition 4 information is at offset 0x01ee
   *   - Within each partition information struct:
   *     - Partition's starting LBA value is at offset 0x08
   *     - Total number of sectors in partition (ie. partition size) is at offset 0x0c
   */
   tbl->partitions[0].partitionStart = READ32UINT(mbr_data + 0x1be + 0x08);
   tbl->partitions[0].partitionLength = READ32UINT(mbr_data + 0x1be + 0x0c);
   tbl->partitions[1].partitionStart = READ32UINT(mbr_data + 0x1ce + 0x08);
   tbl->partitions[1].partitionLength = READ32UINT(mbr_data + 0x1ce + 0x0c);
   tbl->partitions[2].partitionStart = READ32UINT(mbr_data + 0x1de + 0x08);
   tbl->partitions[2].partitionLength = READ32UINT(mbr_data + 0x1de + 0x0c);
   tbl->partitions[3].partitionStart = READ32UINT(mbr_data + 0x1ee + 0x08);
   tbl->partitions[3].partitionLength = READ32UINT(mbr_data + 0x1ee + 0x0c);
}
