#ifndef SDUTILS_H
#define SDUTILS_H

#include <stm32h5xx_hal.h>
#include <stm32h5xx_hal_sd.h>
#include <stm32h5xx_hal_gpio.h>

#include "main.h"

extern SD_HandleTypeDef hsd1;
extern HAL_SD_CardInfoTypeDef cardInfo;

HAL_StatusTypeDef initSDCard();
void cardDetTask();
#endif //SDUTILS_H
