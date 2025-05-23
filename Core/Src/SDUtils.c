#include "SDUtils.h"

HAL_SD_CardInfoTypeDef cardInfo;
int cardDet;

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
