#ifndef TUSB_CONFIG_H
#define TUSB_CONFIG_H

#define BOARD_TUD_RHPORT 0
#define BOARD_TUD_MAX_SPEED   OPT_MODE_DEFAULT_SPEED
#define CFG_TUSB_MCU          OPT_MCU_STM32F4
#define CFG_TUSB_OS           OPT_OS_NONE
#define CFG_TUSB_DEBUG        0
#define CFG_TUD_ENABLED 1
#define CFG_TUD_MAX_SPEED     BOARD_TUD_MAX_SPEED
#define CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_ALIGN          __attribute__ ((aligned(4)))
#define CFG_TUD_ENDPOINT0_SIZE    64
#define CFG_TUD_CDC               1
#define CFG_TUD_MSC               0
#define CFG_TUD_HID               0
#define CFG_TUD_MIDI              0
#define CFG_TUD_VENDOR            0
#define CFG_TUD_CDC_RX_BUFSIZE   (TUD_OPT_HIGH_SPEED ? 512 : 64)
#define CFG_TUD_CDC_TX_BUFSIZE   (TUD_OPT_HIGH_SPEED ? 512 : 64)
#define CFG_TUD_CDC_EP_BUFSIZE   (TUD_OPT_HIGH_SPEED ? 512 : 64)

#endif //TUSB_CONFIG_H
