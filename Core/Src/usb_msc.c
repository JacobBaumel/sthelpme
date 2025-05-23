#include <stm32h5xx_hal.h>
#include <stm32h5xx_hal_gpio.h>

#include "main.h"
#include "SDUtils.h"

#include "tusb.h"
#include "class/msc/msc_device.h"

#include "disk.h"

void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
    static const char VID[] = "LRI Elec";
    static const char PID[] = "Crystalline Echo";
    static const char PREV[] = "v1.0";

    (void) lun;

    memcpy(vendor_id, VID, sizeof(VID));
    memcpy(product_id, PID, sizeof(PID));
    memcpy(product_rev, PREV, sizeof(PREV));
}

bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    (void) lun;
    // return cardDet;
    return false;
}

void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size) {
    (void) lun;

    *block_count = cardInfo.LogBlockNbr;
    *block_size = cardInfo.LogBlockSize;
}

int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
    (void) lun;

    // out of ramdisk
    if (lba >= cardInfo.LogBlockNbr) {
        return -1;
    }

    // Check for overflow of offset + bufsize
    if (offset + bufsize > cardInfo.LogBlockSize) {
        return -1;
    }

    uint8_t data[cardInfo.LogBlockSize];
    HAL_SD_ReadBlocks(&hsd1, data, lba, 1, 10);
    memcpy(buffer, data + offset, bufsize);
    return (int32_t) bufsize;
}

bool tud_msc_is_writable_cb(uint8_t lun) {
    (void) lun;
    return true;
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
    (void) lun;

    // out of ramdisk
    if (lba >= DISK_BLOCKS) return -1;

    uint8_t *addr = DISK[lba] + offset;
    memcpy(addr, buffer, bufsize);

    return (int32_t) bufsize;
}

int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer, uint16_t bufsize) {
    (void) buffer;
    (void) bufsize;

    switch (scsi_cmd[0]) {
    default:
        // Set Sense = Invalid Command Operation
            tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);

        // negative means error -> tinyusb could stall and/or response with failed status
        return -1;
    }
}