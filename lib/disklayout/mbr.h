#include "partition.h"

typedef struct {
  PartitionDescriptor partitions[4];
} MBRPartitionTable;

void fillMBRPartitionTableFromSectorData(MBRPartitionTable* tbl, unsigned char* mbr_data);
