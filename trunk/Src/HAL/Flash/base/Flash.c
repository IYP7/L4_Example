/*****************************************************************************
 * @file 	Flash.c
 *
 * @brief 	Flash driver control.
 *
 * @author 	Albert Serrallo
 *
 * @date 	26/09/2016
 *
 ****************************************************************************/

#ifndef _FLASH_C_
#define _FLASH_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"
#include <string.h>

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#ifdef MON_FLASH_CRC32
#include "crc.h"
#endif //MON_FLASH_CRC32

#include "Flash.h"

/****************************************************************************
 *  EXTERN VARIABLES
 ****************************************************************************/
extern const tFlashInstanceMap FlashInstanceMap[NUM_OF_FLASH];
tFlashContext FlashContext[NUM_OF_FLASH];	/* Attributes for each flash area */
/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/

/****************************************************************************
 *  PRIVATE FUNCTIONS
 ****************************************************************************/
uint32_t FlashCalcOffset ( uint32_t initSector, uint32_t finalSector );
uint8_t FlashGetFirstPage( tFlash flashArea );

/****************************************************************************
 *    PRIVATE DEFINES
 ***************************************************************************/
#define FLASH_DEFAULT_ADDRESS 	0xFFFFFFFF

/****************************************************************************
 * @brief  	Init Flash driver.
 * @param   None.
 * @return  success.
 ****************************************************************************/
eError FlashInit( void )
{
	eError 	success = RET_OK;

	uint8_t 	i = 0;
	uint32_t 	sectorUsed = 0;
	/* Initializes first element */
	FlashContext[i].initAddress = FLASH_BASE;

	/* Set max offset */
	FlashContext[i].initSector = 0;
	FlashContext[i].maxOffset = FlashCalcOffset(sectorUsed, FlashInstanceMap[0].sectors);
	sectorUsed += FlashInstanceMap[0].sectors;

	for (i = 1; i < NUM_OF_FLASH; i++)
	{
		if ( sectorUsed < FLASH_MAX_SECTORS )
		{
			FlashContext[i].initSector = sectorUsed;
			FlashContext[i].initAddress = FlashContext[i-1].initAddress + FlashContext[i-1].maxOffset;
			FlashContext[i].maxOffset = FlashCalcOffset(sectorUsed, (sectorUsed + FlashInstanceMap[i].sectors));

			sectorUsed +=  FlashInstanceMap[i].sectors;
		}
		else
		{
			success = RET_FAIL;
		}
	}

	return success;
}


/****************************************************************************
 * @brief  		return the total offset within initial and final sector
 * @param[in]   initSector: inital sector to calculate offset
 * @param[in]   initSector: inital sector to calculate offset
 * @return  success.
 ****************************************************************************/
uint32_t FlashCalcOffset ( uint32_t initSector, uint32_t finalSector )
{
	uint32_t offset = 0;

	offset = (finalSector - initSector) * (FLASH_SECTOR_SIZE);

	return offset;
}

/*****************************************************************************
 * @brief  	Stops Flash driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError FlashStop( void )
{
	eError 	success = RET_OK;

	return success;
}


/*****************************************************************************
 * @brief  	Starts Flash driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError FlashStart( void )
{
	eError 	success = RET_OK;

	return success;
}


/*****************************************************************************
 * @brief  	Sleeps Flash driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError FlashSleep( void )
{
	eError 	success = RET_OK;

	return success;
}


/*****************************************************************************
 * @brief  	Wakes up Flash driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError FlashWake( void )
{
	eError 	success = RET_OK;

	return success;
}


/*****************************************************************************
 * @brief	FlashLock
 * 			Lock the FLASH control register access
 * @note  	The function should be called after to lock the FLASH interface
 * @param
 * @return 	Success or error status.
 *
 ****************************************************************************/
eError FlashLock( void)
{
	eError success = RET_FAIL;

	HAL_StatusTypeDef flashStatus = HAL_ERROR;

	flashStatus = HAL_FLASH_Lock();
	if(flashStatus == HAL_OK)
	{
		success = RET_OK;
	}

	return success;
}


/*****************************************************************************
 * @brief	FlashUnlock
 * 			Unlock the FLASH control register access
 * @note   	The function should be called before to unlock the FLASH interface
 * @param
 * @return 	Success or error status.
 *
 ****************************************************************************/
eError FlashUnlock( void)
{
	eError success = RET_FAIL;

	HAL_StatusTypeDef flashStatus = HAL_ERROR;

	flashStatus = HAL_FLASH_Unlock();
	if(flashStatus == HAL_OK)
	{
		success = RET_OK;
	}

	return success;
}



/*****************************************************************************
 * @brief	FlashReadData.
 * @param	data: value read
 * 			offset: from 0 - to total flash area size
 * 			flashArea: Specifies the flash area.
 * @return 	Success or error status.
 ****************************************************************************/
eError FlashReadData(tFlash flashArea, uint64_t *data, uint32_t offset)
{
	eError 	success = RET_OK;

	uint32_t address = FLASH_DEFAULT_ADDRESS;

	if(offset>FlashContext[flashArea].maxOffset)
	{
		success = RET_FAIL;
	}
	else
	{
		address = FlashContext[flashArea].initAddress + offset;
		*data = (*(__IO uint64_t*)(address));
	}

	return success;
}


/*****************************************************************************
 * @brief	FlashReadDataBuffer retrieves a buffer of data from flash
 *
 * @param[in,out] data: buffer to fill
 * @param[in] offset: from 0 - to total flash area size
 * @param[in] flashArea: Specifies the flash area.
 * @param[in] size: How many bytes to read.
 *
 * @return 	Success or error status.
 *
 ****************************************************************************/
eError FlashReadDataBuffer(tFlash flashArea, uint8_t *data, uint32_t offset, uint32_t size)
{
	eError	success = RET_OK;

	uint32_t address = FLASH_DEFAULT_ADDRESS;

	if(offset>FlashContext[flashArea].maxOffset || offset % 2)
	{
		success = RET_FAIL;
	}
	else
	{
		address = FlashContext[flashArea].initAddress + offset;
		memcpy((uint32_t*) data, (uint32_t*) address, size);
	}

	return success;
}


/*****************************************************************************
 * @brief	FlashProgramData
 * 			Program half-word (16-bit) at a specified address.
 *
 * @note   	FLASH should be previously erased before new
 * 			programmation.
 *
 * @param	flashArea: Specifies the flash area.
 *			data: Specifies the data to be programmed
 *			offset: from 0 - to total flash area size
 *
 * @return 	success or error status.
 *
 ****************************************************************************/
eError FlashProgramData( tFlash flashArea, uint64_t data, uint32_t offset)
{
	eError 	success = RET_OK;

	HAL_StatusTypeDef status = HAL_OK;

	/* Check the offset is valid */
	if(offset>FlashContext[flashArea].maxOffset)
	{
		success = RET_FAIL;
	}
	else
	{
		success = FlashUnlock();
		if(success == RET_OK)
		{
			status =  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (FlashContext[flashArea].initAddress + offset), data);
			if(status != HAL_OK)
			{
				success = FlashLock();
				return RET_FAIL;
			}
			success = FlashLock();
		}
	}

	return success;
}


/*****************************************************************************
 * @brief	Write a Buffer to Flash. Note that it must be multiple of 2
 * 			Program half-word (16-bit) aty a specified address.
 *
 * @note   	FLASH should be previously erased before new
 * 			programmation. Or behaviour on write is undefined
 *
 * @param[in] flashArea: Specifies the flash area.
 * @param[in] data: Specifies the data to be programmed
 * @param[in] bufferSize: Size of the buffer. Multiple of two
 * @param[in] offset: from 0 - to total flash area size. Divisible by two
 *
 * @return 	success or error.
 *
 ****************************************************************************/
eError FlashProgramDataBuffer( tFlash flashArea, uint8_t* data, uint32_t bufferSize, uint32_t offset)
{
	uint16_t* dataPtr;
	eError err;
	uint32_t loopOffset;

	if(offset % 2 || bufferSize % 2) {
		return RET_FAIL;
	}

	for(loopOffset = offset,
		dataPtr = (uint16_t*) data;
		loopOffset < offset + bufferSize; loopOffset+=2, dataPtr++){

		err = FlashProgramData(flashArea, *dataPtr, loopOffset);
		if(err != RET_OK){
			return err;
		}
	}

	return RET_OK;

}


/*****************************************************************************
 * @brief	FlashErase
 * 			Perform a total flash area erase or erase the specified FLASH
 * 			memory page.
 *
 * @param  page: FLASH_ALL_PAGES or a specific page.
 *
 * @return success or error status.
 *
 ****************************************************************************/
eError FlashErase( tFlash flashArea, uint8_t page)
{
	eError 	success = RET_OK;

	HAL_StatusTypeDef flashStatus 	= HAL_OK;
	uint32_t FlashEraseError = 0;
	uint32_t fPage = FlashGetFirstPage(flashArea);
	uint32_t totalPages = FlashGetPages(flashArea);

	FLASH_EraseInitTypeDef 	FlashErase;	 /*FLASH Erase structure */

	/* Initializes Flash erase structure */
	FlashErase.TypeErase = FLASH_TYPEERASE_PAGES;
	FlashErase.Banks     = FLASH_BANK_1;

	/* all pages to erase */
	if(page == FLASH_ALL_PAGES)
	{
		FlashErase.NbPages = FlashGetPages(flashArea);
		FlashErase.Page = FlashGetFirstPage(flashArea);
	}
	/* only a specific page */
	else
	{
		if(page < (fPage + totalPages))
		{
			FlashErase.NbPages = 1;
			FlashErase.Page = page;
		}
		else
		{
			success = RET_FAIL;
		}
	}

	if(success == RET_OK)
	{
		success = FlashUnlock();
		if(success == RET_OK)
		{
			flashStatus = HAL_FLASHEx_Erase(&FlashErase, &FlashEraseError);
			if(flashStatus != HAL_OK)
			{
				success = FlashLock();
				return RET_FAIL;
			}
			success = FlashLock();
		}
	}
	return success;
}


/*****************************************************************************
 * @brief	FlashGetSize
 * @param
 * @return Success or error status.
 ****************************************************************************/
void FlashGetSize( tFlash flashArea, uint32_t *size)
{
	*size =   FlashContext[flashArea].maxOffset;
}



/*****************************************************************************
 * @brief	FlashGetPages
 * @param
 * @return Success or error status.
 ****************************************************************************/
uint8_t FlashGetPages( tFlash flashArea)
{
	uint8_t pages = FlashInstanceMap[flashArea].sectors / (FLASH_PAGE_SIZE / FLASH_SECTOR_SIZE);

	return pages;
}

/*****************************************************************************
 * @brief	FlashGetPages
 * @param
 * @return Success or error status.
 ****************************************************************************/
uint8_t FlashGetFirstPage( tFlash flashArea )
{
	uint8_t fPage = 0;

	if (FlashContext[flashArea].initAddress < (FLASH_BASE + FLASH_BANK_SIZE))
	{
		/* Bank 1 */
		fPage = (FlashContext[flashArea].initAddress - FLASH_BASE) / FLASH_PAGE_SIZE;
	}
	else
	{
		/* Bank 2 */
		fPage = (FlashContext[flashArea].initAddress - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
	}

	return fPage;
}

/*****************************************************************************
 * @brief	Returns the start address of a flash Araea
 * @param   flashArea The flash area to return the start address
 * @return  uint32_t value of a flash start address area
 ****************************************************************************/
uint32_t FlashGetStartAddress( tFlash flashArea)
{
	return FlashContext[flashArea].initAddress;
}


/*****************************************************************************
 * @brief	Returns the actual Flash Instance
 * @param[out]	actual areaFlash
 * @return  initFlashArea
 ****************************************************************************/
eError FlashActualInstance( tFlash *areaFlash )
{
	eError success = RET_FAIL;
	uint32_t flashMapAddress, lastAreaAddress;
	tFlash flash;

	flashMapAddress = (uint32_t)&FlashInstanceMap;
	for (flash = 0; flash < NUM_OF_FLASH; flash++)
	{
		lastAreaAddress = FlashContext[flash].initAddress + FlashContext[flash].maxOffset;
		if ( flashMapAddress > FlashContext[flash].initAddress &&  flashMapAddress < lastAreaAddress )
		{
			*areaFlash = flash;
			success = RET_OK;
			break;
		}
	}
	return success;
}



#ifdef MON_FLASH_CRC32
/*****************************************************************************
 * @brief	Return the CRC32 of a Flash area, minus FLASH_CONFBYTES_SIZE bytes
 *			at the end. those bytes are not used for CRC as those are possible
 *			configuration bytes and may (or may not) contain a crc themselves.
 * @param  flashArea Area of the flash to CRC
 * @return Calculated CRC
 ****************************************************************************/
uint32_t FlashCRC(tFlash flashArea)
{
	uint8_t i;
	uint8_t data[FLASH_MEMORY_SIZEOF_PAGES]; //put this in stack
	uint32_t crc = 0x00000000; //note that we are using this value bitwise NOT as default
                               //seed. Following the CCITT format

	for(i=0; i<FlashInstanceMap[flashArea].pages; i++){
		FlashReadDataBuffer(flashArea, &data[0],
			i * FLASH_MEMORY_SIZEOF_PAGES, FLASH_MEMORY_SIZEOF_PAGES);

        //Last iteration does not have to include configuration BYTES
        if(i == FlashInstanceMap[flashArea].pages - 1){
		    crc = crc32a(&data[0],
                FLASH_MEMORY_SIZEOF_PAGES - FLASH_CONFBYTES_SIZE, ~crc);
        } else {
		    crc = crc32a(&data[0],
                FLASH_MEMORY_SIZEOF_PAGES, ~crc);
        }
	}

	return crc;
}
#endif //MON_FLASH_CRC32

/*****************************************************************************
 * @brief	Get the last FLASH_CONFBYTES_SIZE bytes of a Flash area in a buffer
 *			to be interpreted by higher levels.
 * @param[in]  		flashArea Area of the flash to retrieve the configuration bytes from
 * @param[in,out]  	buffer Memory buffer to fill with the configuration bytes
 * @return
 ****************************************************************************/
void inline FlashReadConfBytes(tFlash flashArea, uint8_t *buffer)
{
	uint32_t address;

	address = FlashContext[flashArea].initAddress + FlashContext[flashArea].maxOffset - FLASH_CONFBYTES_SIZE;
	memcpy((uint32_t*) buffer, (uint32_t*) address, FLASH_CONFBYTES_SIZE);
}

/*****************************************************************************
 * @brief	Write the buffer as configuration bytes. Note that Flash Writes
 * 			only can be performed in 0xFF areas or to 0x00, writing the same
 *			value will result in a HAL ERROR. For that, this function checks
 * 			difference in values and performs the write only in the values
 *			that are different
 * @param[in]  	flashArea Area of the flash to retrieve the configuration bytes from
 * @param[in]  	buffer Memory buffer write to Flash.
 * @return
 ****************************************************************************/
void inline FlashProgramConfBytes(tFlash flashArea, uint8_t *buffer)
{
	uint16_t* dataWrPtr;
	uint16_t* dataRdPtr;
	uint32_t loopOffset;

	//Read existing conf bytes
    uint8_t tempBuffer[FLASH_CONFBYTES_SIZE];
    FlashReadConfBytes(flashArea, tempBuffer);

	for(loopOffset = FlashContext[flashArea].maxOffset - FLASH_CONFBYTES_SIZE,
		dataWrPtr = (uint16_t*) buffer,
		dataRdPtr = (uint16_t*) tempBuffer;

		loopOffset < FlashContext[flashArea].maxOffset;

		loopOffset+=2, dataWrPtr++, dataRdPtr++){ //end for loop definition

		if(*dataWrPtr == *dataRdPtr){
			continue; //same information, don't try to write to Flash
		}

		if(*dataRdPtr == 0xFFFF || *dataWrPtr == 0){ //go Ahead, we can record with no problem
			FlashProgramData(flashArea, *dataWrPtr, loopOffset);
		}
	}
}




#endif // _FLASH_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
