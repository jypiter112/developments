#include "io.h"
#include <stdint.h>
#include "ata.h"

// TODO: implement check to ST_ERR bit in between functions, to avoid hang
/*
    Function: waits until ATA drive is finished
    Param: n_drq
    1 -> wait for data
    0 -> wait for drive to finish
    return: -1 on error, 0 on success
*/
static int ata_wait(int n_drq){
    // needed because drive needs time to send status signal
    for(int i = 0; i < 4; i++){
        inb(ATA_CTRL); // this loop does a 400ns delay
    }
    uint8_t s;
    while((s = inb(ATA_CMD)) & ST_BSY); // waits until drive clears its busy bit
    if(s & (ST_ERR | ST_DF)){
        return -1;
    }
    // checks if drive st request bit since if n_drq == 1, then we are requesting something
    if(n_drq && !(s & ST_DRQ)){
        return -1;
    }
    return 0;
}
/*
    Function: tells ATA drive what to do and where, then starts a command
    Params:
    drive: DRIVE_MASTER 0 = OS, DRIVE_SLAVE 1 = DRIVE1
    lba: 28-bit logical block sector address
    count: sector transfer count: 0 = 256
    cmd: command, read/write ..
*/
static void ata_setup(uint8_t drive, uint32_t lba, uint8_t count, uint8_t cmd){
    // selects which drive to use
    // sets up the drive for operation specified
    outb(ATA_DRIVE, 0xE0 | (drive << 4) | ((lba >> 24) & 0x0F));
    outb(ATA_COUNT, count);
    outb(ATA_LBA_LO, lba);
    outb(ATA_LBA_MID, lba >> 8);
    outb(ATA_LBA_HI, lba >> 16);
    // disable disk interrupts
    outb(ATA_CTRL, 0x02);
    // execute command
    outb(ATA_CMD, cmd);
}
// TO DO: implement kmalloc for this function to not fail in the future
int ata_read(uint8_t drive, uint32_t lba, uint8_t count, void *buf){
    ata_setup(drive, lba, count, ATA_CMD_READ);
    uint16_t *p = (uint16_t*)buf;
    for(int i = 0; i < count; i++){
        if(ata_wait(1) < 0) return -1;
        insw(ATA_DATA, p, 256); // 256 bytes = 512 bits
        p += 256;
    }
    return 0;
}
int ata_write(uint8_t drive, uint32_t lba, uint8_t count, const void*buf){
    ata_setup(drive, lba, count, ATA_CMD_WRITE);
    const uint16_t *p = (const uint16_t*)buf;
    for(int i = 0; i < count; i++){
        if(ata_wait(1) < 0) return -1;
        outsw(ATA_DATA, p, 256);
        p += 256;
    }
    outb(ATA_CMD, ATA_CACHE_FLUSH);
    return ata_wait(0);
}
