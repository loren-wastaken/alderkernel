#include "mbr.h"
#include "../drivers/ata.h"

#define MBR_PARTITION_TABLE_OFFSET 446
#define MBR_SIGNATURE_OFFSET       510
#define MBR_ENTRY_SIZE             16

int mbr_read_partitions(mbr_partition_t out[4])
{
    unsigned char sector[512];

    for (int i = 0; i < 4; i++) {
        out[i].bootable = 0;
        out[i].type = 0;
        out[i].lba_start = 0;
        out[i].sector_count = 0;
    }

    if (!ata_read_sector(0, sector)) {
        return 0;
    }

    unsigned int signature = sector[MBR_SIGNATURE_OFFSET] | (sector[MBR_SIGNATURE_OFFSET + 1] << 8);
    if (signature != 0xAA55) {
        return 0; // no valid MBR present yet
    }

    int used = 0;

    for (int i = 0; i < 4; i++) {
        unsigned char* entry = &sector[MBR_PARTITION_TABLE_OFFSET + i * MBR_ENTRY_SIZE];

        unsigned char status = entry[0];
        unsigned char type   = entry[4];
        unsigned int lba_start = (unsigned int)entry[8] | ((unsigned int)entry[9] << 8) |
                                  ((unsigned int)entry[10] << 16) | ((unsigned int)entry[11] << 24);
        unsigned int sector_count = (unsigned int)entry[12] | ((unsigned int)entry[13] << 8) |
                                     ((unsigned int)entry[14] << 16) | ((unsigned int)entry[15] << 24);

        out[i].bootable = (status == 0x80) ? 1 : 0;
        out[i].type = type;
        out[i].lba_start = lba_start;
        out[i].sector_count = sector_count;

        if (type != 0) {
            used++;
        }
    }

    return used;
}