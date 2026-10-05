#ifndef ATA_H
#define ATA_H

// for types, move to types.h in the future to omit stdint.h
#include <stdint.h>

#define ATA_DATA 0x1F0
#define ATA_COUNT 0x1F2
#define ATA_LBA_LO 0x1F3
#define ATA_LBA_MID 0x1F4
#define ATA_LBA_HI 0x1F5
#define ATA_DRIVE 0x1F6
#define ATA_CMD 0x1F7
#define ATA_CTRL 0x3F6
#define ATA_CACHE_FLUSH 0xE7

// drive control bits (set by the drive)
#define ST_ERR 0x01 // error
#define ST_DRQ 0x08 // data request, (set by drive)
#define ST_DF 0x20 // drive fault
#define ST_BSY 0x80 // drive BUSY bit, 1 busy, 0 not busy

// ATA commands
#define ATA_CMD_READ 0x20
#define ATA_CMD_WRITE 0x30

// defined drives here
#define DRIVE_MASTER 0 // where OS lives, dont modify
#define DRIVE_SLAVE 1

int ata_read(uint8_t, uint32_t, uint8_t, void*);
int ata_write(uint8_t, uint32_t, uint8_t, const void*);

#endif