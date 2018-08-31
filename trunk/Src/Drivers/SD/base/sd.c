/*************************************************************************//**
 * @file sdio.c
 *
 * @brief sdio driver control.
 *
 * @author Alex Colomer
 *
 * @date 26/03/2018
 *
 ****************************************************************************/

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"
#include <string.h>

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/
#include "hreg_dat.h"
#include "sd.h"
#include "DMA.h"
/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
//#include "integer.h"
/****************************************************************************
 *  EXTERNAL VARIABLES
 ****************************************************************************/
extern const tSdioInit sdioInitMap[NUM_OF_SD];
extern const tSdioInit sdioTransferMap[NUM_OF_BIT_TRANSFER_TYPES];
extern const tDmaStreamMap dmaStreamMap[NUM_OF_DMA_STREAMS];
extern DMA_HandleTypeDef DMA_Handler[NUM_OF_DMA_STREAMS];
/****************************************************************************
 *  PRIVATE VARIABLES
 ***************************************************************************/
SD_HandleTypeDef uSdHandle[NUM_OF_SD];

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
uint16_t sdNumberOfBlocks = 0;
uint32_t addressSector = 0;
uint32_t blockSize = 0;
/****************************************************************************
 *    PRIVATE FUNCTIONS DECLARATIONS
 ****************************************************************************/


/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/

/*************************************************************************//**
 * @brief  Init SDIO driver.
 * @return  eError.
 *
 ****************************************************************************/
eError sdInit(void)
{
	uint8_t result, i;

	result = RET_OK;

	const tSdioInit* msd;
	SD_HandleTypeDef* hsd;

	for(i=0; i<NUM_OF_SD; i++)
	{
		hsd = &uSdHandle[i];
		msd = &sdioInitMap[i];

		/* uSD device interface configuration */
		hsd->Instance = SDMMC1;
		hsd->Init.ClockEdge = msd->ClockEdge;
		hsd->Init.ClockBypass = msd->ClockBypass;
		hsd->Init.ClockPowerSave = msd->ClockPowerSave;
		hsd->Init.ClockDiv = msd->ClockDiv;
		hsd->Init.BusWide = msd->BusWide;
		hsd->Init.HardwareFlowControl = msd->HardwareFlowControl;


		if(HAL_SD_Init(hsd) != HAL_OK)
		{
			result = RET_NOT_INITIALIZED;
		}

	}
	return  result;

}


/*************************************************************************//**
 * @brief  Stops SDIO driver.
 *
 * @return  None.
 *
 ****************************************************************************/
eError sdStop( void )
{
	uint8_t i;
	eError result = RET_OK;

	for(i=0; i<NUM_OF_SD; i++)
	{
		SDMMC_PowerState_OFF(uSdHandle[i].Instance);
		__SDMMC_DISABLE(uSdHandle[i].Instance);
	}

	return result;
}

/*************************************************************************//**
 * @brief  Starts SDIO driver.
 *
 * @return  None.
 *
 ****************************************************************************/
eError sdStart( void )
{
	uint8_t i;
	eError result = RET_OK;
	SD_HandleTypeDef* hsd;

	__HAL_RCC_SDIO_CLK_ENABLE();

	for(i=0; i<NUM_OF_SD; i++)
	{
		hsd = &uSdHandle[i];

		/* Enable wide operation */
		if(HAL_SD_ConfigWideBusOperation(hsd, SDMMC_BUS_WIDE_4B) != HAL_OK)
		{
			result = RET_FAIL;
		}
		else
		{
			result = RET_OK;
		}
		if(SDMMC_PowerState_ON(hsd->Instance) != HAL_OK)
		{
			result = RET_FAIL;
		}
		else
		{
			result = RET_OK;
		}

		__SDMMC_ENABLE(hsd->Instance);

#ifdef DMA_AVAILABLE
		/* Associate the DMA handle */
		__HAL_LINKDMA(hsd, hdmarx, DMA_Handler[SD_RX_CHANNEL]);
		__HAL_LINKDMA(hsd, hdmatx, DMA_Handler[SD_TX_CHANNEL]);

#endif

	}

	return result;
}


/*************************************************************************//**
 * @brief  Sleeps SDIO driver.
 *
 * @return  None.
 *
 ****************************************************************************/
eError sdSleep( void )
{
	eError result = RET_OK;

	result = sdStop();

	return result;

}

/*************************************************************************//**
 * @brief  Wakes up sdio driver.
 *
 * @return  None.
 *
 ****************************************************************************/
eError sdWake( void )
{
	eError result = RET_OK;
	result = sdStart();

	return result;
}


/***************************************************************************
 * @brief  SDIO read.
 *
 * @param	rxBuffer.
 * @param	dataConfig.
 *
 * @return  eError.
 *
 ****************************************************************************/
eError sdDriverRead(tSd sdCard, uint32_t *buff, uint64_t readAddress, uint32_t NumberOfBlocks)
{
	eError result;
	HAL_StatusTypeDef SDresult;
	SD_HandleTypeDef *hsd;

	result = RET_OK;
	hsd = &uSdHandle[sdCard];

#if defined MON_SDIO_RX_POLL
	SDresult = HAL_SD_ReadBlocks(hsd, (uint8_t *)buff, readAddress, NumberOfBlocks);
#elif defined MON_SDIO_RX_DMA
	SDresult = HAL_SD_ReadBlocks_DMA(hsd, (uint8_t *)buff, readAddress, NumberOfBlocks);
#endif

	if(SDresult != HAL_OK)
	{
		result = RET_FAIL;
	}

	return result;
}



/***************************************************************************
 * @brief  SDIO write.
 *
 * @param	rxBuffer.
 * @param
 *
 * @return  eError.
 *
 ****************************************************************************/
eError sdDriverWrite(tSd sdCard, uint32_t *buff, uint64_t writeAddress, uint32_t NumberOfBlocks)
{
	eError result;
	HAL_StatusTypeDef SDresult;
	SD_HandleTypeDef *hsd;
	uint32_t auxBuffer[blockSize];

	result = RET_OK;
	hsd = &uSdHandle[sdCard];

	memcpy(&auxBuffer, buff, blockSize);

#if defined MON_SDIO_TX_POLL
	SDresult = HAL_SD_WriteBlocks(hsd, (uint8_t *)auxBuffer, writeAddress, NumberOfBlocks);
#elif defined MON_SDIO_TX_DMA
	SDresult = HAL_SD_WriteBlocks_DMA(hsd, (uint8_t *)auxBuffer, writeAddress, NumberOfBlocks);
#endif

	if(SDresult != HAL_OK)
	{
		result = RET_FAIL;
	}

	return result;
}


/***************************************************************************
 * @brief  SD get bufferSize.
 *
 * @param	rxBuffer.
 * @param
 *
 * @return  eError.
 *
 ****************************************************************************/
uint32_t sdDriverGetBufferSize(void)
{
	return blockSize;
}


/*************************************************************************//**
 * @brief  Set SD buffer size to write/read
 *
 * @param	sdCard Device to write/read to/from
 * @param	bufferSize buffer size.
 * @return  error state.
 ****************************************************************************/
eError sdDriverSetBufferSize(uint32_t bufferSize)
{
    blockSize = bufferSize;

    return RET_OK;
}



/*************************************************************************//**
 * @brief Returns current status of the port related to an SD card.
 *
 * @param	sdCard Sd card to get the status from it
 * @return  Status of the driver, either busy or allright.
 ****************************************************************************/
eError sdDriverGetStatus(tSd sdCard)
{
	SD_HandleTypeDef *hSd;

	hSd = &uSdHandle[sdCard];

    switch (HAL_SD_GetState(hSd))
    {
        case HAL_SD_STATE_TRANSFER:
            return RET_OK;
        case HAL_SD_STATE_BUSY:
            return RET_BUSY;
        case HAL_SD_STATE_ERROR:
        default:
        	return RET_FAIL;
    }
}



/***************************************************************************
 * @brief  SD get address sector to write/read into
 *
 * @param	none
 *
 * @return  addressSector.
 *
 ****************************************************************************/
uint32_t sdDriverGetSector(void)
{
	return addressSector;
}


/*************************************************************************//**
 * @brief  Set SD address sector to write/read into
 *
 * @param	address sector.
 * @return  error state.
 ****************************************************************************/
eError sdDriverSetSector(uint32_t sector)
{
    addressSector = sector;

    return RET_OK;;
}


/***************************************************************************
 * @brief  SD get number of blocks
 *
 * @param	none
 *
 * @return  number of blocks.
 *
 ****************************************************************************/
uint32_t sdDriverGetNumberOfBlocks(void)
{
	return sdNumberOfBlocks;
}


/*************************************************************************//**
 * @brief  Set SD number of blocks
 *
 * @param	number of blocks.
 * @return  error state.
 ****************************************************************************/
eError sdDriverSetNumberOfBlocks(uint32_t numberOfBlocks)
{
    sdNumberOfBlocks = numberOfBlocks;

    return RET_OK;
}




/****************************************************************************
 *    End of file
 ***************************************************************************/
