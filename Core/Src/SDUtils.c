#include "SDUtils.h"

HAL_SD_CardInfoTypeDef cardInfo;

// Stolen from HAL_SD_Init
HAL_StatusTypeDef initSDCard() {
    HAL_SD_CardStatusTypeDef CardStatus;
    uint32_t speedgrade;
    uint32_t unitsize;
    uint32_t tickstart;

    /* Initialize the Card parameters */
    if(HAL_SD_InitCard(&hsd1) != HAL_OK) {
        return HAL_ERROR;
    }

    if(HAL_SD_GetCardStatus(&hsd1, &CardStatus) != HAL_OK) {
        return HAL_ERROR;
    }

    speedgrade = CardStatus.UhsSpeedGrade;
    unitsize = CardStatus.UhsAllocationUnitSize;
    if((hsd1.SdCard.CardType == CARD_SDHC_SDXC) && ((speedgrade != 0U) || (unitsize != 0U))) {
        hsd1.SdCard.CardSpeed = CARD_ULTRA_HIGH_SPEED;
    }
    else {
        if(hsd1.SdCard.CardType == CARD_SDHC_SDXC) {
            hsd1.SdCard.CardSpeed = CARD_HIGH_SPEED;
        }
        else {
            hsd1.SdCard.CardSpeed = CARD_NORMAL_SPEED;
        }
    }

    /* Configure the bus wide */
    if(HAL_SD_ConfigWideBusOperation(&hsd1, hsd1.Init.BusWide) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Verify that SD card is ready to use after Initialization */
    tickstart = HAL_GetTick();
    while((HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)) {
        if((HAL_GetTick() - tickstart) >= SDMMC_SWDATATIMEOUT) {
            hsd1.ErrorCode = HAL_SD_ERROR_TIMEOUT;
            hsd1.State = HAL_SD_STATE_READY;
            return HAL_TIMEOUT;
        }
    }

    /* Initialize the error code */
    hsd1.ErrorCode = HAL_SD_ERROR_NONE;

    /* Initialize the SD operation */
    hsd1.Context = SD_CONTEXT_NONE;

    /* Initialize the SD state */
    hsd1.State = HAL_SD_STATE_READY;

    return HAL_OK;
}

void cardDetTask() {
    static GPIO_PinState prevState = GPIO_PIN_RESET;
    GPIO_PinState curState = HAL_GPIO_ReadPin(SD_CARD_DET_GPIO_Port, SD_CARD_DET_Pin);

    // If the card detection pin value did not change, do nothing
    if(curState == prevState) return;
    prevState = curState;

    // If a card has been detected, initialize the card
    if(curState == GPIO_PIN_RESET) {
        if(hsd1.State != HAL_SD_STATE_RESET) return;
        if(initSDCard() == HAL_OK) {
            HAL_SD_GetCardInfo(&hsd1, &cardInfo);
        }
    }

    // Otherwise deinitialize
    else {
        if(hsd1.State == HAL_SD_STATE_RESET) return;
        hsd1.State = HAL_SD_STATE_RESET;
    }
}
