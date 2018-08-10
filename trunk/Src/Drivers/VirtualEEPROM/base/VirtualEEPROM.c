/**************************************************************************//**
 * @file
 * @addtogroup Drivers
 * @{
 * @addtogroup VirtualEEPROM
 * @{
 * 
 * @brief 	VirtualEEPROM control driver.
 *
 * @author 	Albert Serrallo
 *
 * @date 	20/04/2016
 *
 ****************************************************************************/

#ifndef _VIRTUALEEPROM_C_
#define _VIRTUALEEPROM_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"
#include "Hreg_dat.h"
#include "sreg_dat.h"

/****************************************************************************
 *  INCLUDES
 ****************************************************************************/
#include "VirtualEEPROM.h"
#include "VirtualEEPROM_map.h"

/****************************************************************************
 *  DEFINES
 ****************************************************************************/

/* Used Flash pages for EEPROM emulation */
#define PAGE0                	((uint16_t)0x0000)
#define PAGE1                	((uint16_t)0x0001)

/* No valid page define */
#define NO_VALID_AREA         	((uint16_t)0x00AB)

/* Page status definitions */
#define ERASED                	((uint16_t)0xFFFF)     /* Page is empty */
#define RECEIVE_DATA          	((uint16_t)0xEEEE)     /* Page is marked to receive data */
#define VALID_AREA            	((uint16_t)0x0000)     /* Page containing valid data */

#define STATUS2_OBSOLETE		((uint16_t)0x0000)
#define STATUS2_NORMAL			((uint16_t)0xFFFF)

/* Valid pages in read and write defines */
#define TRANSFER_STATE_RESTART  	((uint8_t)0x00)
#define TRANSFER_STATE_NO   	   ((uint8_t)0x01)

/* Page full define */
#define PAGE_FULL             	((uint8_t)0x80)

/* Number of pages */
#define ONE_PAGE  				((uint8_t)0x01)

#define FLASH_MOD_SIZE(size) 	((size%2) ? 1  : 0)

#define EEPROM_BYTES_TRANSFER_PAGE 20
#define EEPROM_BYTES_SWAP 8

#define INITIAL_DATA_AREA 4

/****************************************************************************
 *  EXTERN VARIABLES
 ****************************************************************************/
extern const tVirtualEEPROMDeviceMap	EEPROMDeviceMap[NUM_OF_VIRTUAL_EEPROMS];

extern const tVirtualEEPROMInstanceMap virtualEEPROMInstanceMap[NUM_OF_TABLE_EEPROMS];
extern const tVirtualEEPROMAreaMap virtualEEPROMhregMap[EEPROM_NUM_OF_HREG];
extern const tVirtualEEPROMAreaMap virtualEEPROMsregFactoryMap[EEPROM_NUM_OF_FACTORY_SREG];
extern const tVirtualEEPROMAreaMap virtualEEPROMsregCalibrationMap[EEPROM_NUM_OF_CALIBRATION_SREG];
extern const tVirtualEEPROMAreaMap virtualEEPROMsregUserMap[EEPROM_NUM_OF_USER_SREG];
extern const tVirtualEEPROMAreaMap virtualEEPROMdataTableMap[EEPROM_NUM_OF_DATA];

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
tVirtualEEPROMRegister virtualEEPROMRegister;

tVirtualEEPROMContext virtualEEPROMhregContext[EEPROM_NUM_OF_HREG];
tVirtualEEPROMContext virtualEEPROMsregFactoryContext[EEPROM_NUM_OF_FACTORY_SREG];
tVirtualEEPROMContext virtualEEPROMsregCalibrationContext[EEPROM_NUM_OF_CALIBRATION_SREG];
tVirtualEEPROMContext virtualEEPROMsregUserContext[EEPROM_NUM_OF_USER_SREG];
tVirtualEEPROMContext virtualEEPROMdataContext[EEPROM_NUM_OF_DATA];

const tVirtualEEPROMareaContext virtualEEPROMareaContext [NUM_OF_TABLE_EEPROMS] =
{
		{ EEPROM_NUM_OF_HREG,				(tVirtualEEPROMContext*)&virtualEEPROMhregContext			},
		{ EEPROM_NUM_OF_FACTORY_SREG,		(tVirtualEEPROMContext*)&virtualEEPROMsregFactoryContext	},
		{ EEPROM_NUM_OF_CALIBRATION_SREG,	(tVirtualEEPROMContext*)&virtualEEPROMsregCalibrationContext},
		{ EEPROM_NUM_OF_USER_SREG,			(tVirtualEEPROMContext*)&virtualEEPROMsregUserContext		},
		{ EEPROM_NUM_OF_DATA,				(tVirtualEEPROMContext*)&virtualEEPROMdataContext			},
};


tVirtualEEPROMCache virtualEEPROMCache[SIZE_OF_VIRTUAL_EEPROM_CACHE];
uint8_t	VirtualEEPROMCacheUsed = 0;

/****************************************************************************
 *  PRIVATE FUNCTIONS
 ****************************************************************************/
eError VirtualEEPROMinitVirtual (tVirtualEEPROMDevice eepromDevice );
eError VirtualEEPROMSearchPointer(tVirtualEEPROMDevice eepromDevice, tFlash flashArea);

uint16_t VirtualEEPROMWriteVariable(tVirtualEEPROMDevice eepromDevice , uint16_t VirtOffset, uint16_t Data);
uint16_t VirtualEEPROMReadVariable(tFlash areaNumber, uint16_t virtualOffset, uint16_t* Data, uint32_t offsetStartAddr );

eError VirtualEEPROMFormat(tVirtualEEPROMDevice eepromDevice );
tFlash VirtualEEPROMFindValidArea(tVirtualEEPROMDevice eepromDevice , uint8_t Operation);
eError VirtualEEPROMPageTransfer(tVirtualEEPROMDevice eepromDevice , uint16_t VirtOffset, uint16_t Data);
uint16_t VirtualEEPROMVerifyPageFullWriteVariable(tFlash areaNumber , uint16_t VirtOffset, uint16_t Data);

static eError VirtualEEPROMMarkPageAs(tFlash flashArea, uint16_t Value);
static uint16_t VirtualEEPROMReadStatus( tFlash flashArea );
static eError VirtualEEPROMAdressExist( tFlash areaNumber, uint16_t VirtOffset ,uint8_t transferState);

eError VirtualEEPROMTransferPage( tVirtualEEPROMDevice eepromDevice );
eError VirtualEEPROMEraseOldPage( tVirtualEEPROMDevice eepromDevice );
eError VirtualEEPROMTransferDataFrom(tFlash flashAreaOld, tFlash flashAreaNew, uint8_t transferState);

eError findInCache( uint16_t virtualAdress, uint16_t *value );
eError updateCache( uint16_t virtualAdress, uint16_t value );
eError addToCache( uint16_t virtualAdress, uint16_t value );

/**************************************************************************//**
 * @brief  	Initializes Virtual Address for VirtualEEPROM table
 * @param	None.
 * @return  success: RET_OK or RET_FAIL
 ****************************************************************************/
eError VirtualEEPROMinit(void)
{
	eError 	success = RET_OK;

	uint8_t reg = 0, eeprom = 0;
	uint16_t areaSize;
	uint8_t nextNumInst = 0;
	uint16_t nextVirtOffset = 0;
	uint16_t lastOffsetArea = 0;

	for (eeprom = 0; eeprom < NUM_OF_TABLE_EEPROMS; eeprom++)
	{
		/* if offsetArea is 0, ignore this Area */
		if ( virtualEEPROMInstanceMap[eeprom].offsetArea != 0 )
		{
			areaSize = virtualEEPROMareaContext[eeprom].areaSize;
			for (reg = 0; reg < areaSize; reg++ )
			{
				virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset = nextVirtOffset;
				virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size = virtualEEPROMInstanceMap[eeprom].regTable[reg].sizeOfInstance;
				nextNumInst = virtualEEPROMInstanceMap[eeprom].regTable[reg].numOfInstances;
				nextVirtOffset = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset + nextNumInst * virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size;
			}

			/* Check that offset is between the reserved offset area */
			if ( nextVirtOffset > (virtualEEPROMInstanceMap[eeprom].offsetArea + lastOffsetArea))
			{
				return RET_NOT_INITIALIZED;
			}
			else
			{
				nextVirtOffset = virtualEEPROMInstanceMap[eeprom].offsetArea + lastOffsetArea;
				lastOffsetArea = nextVirtOffset;
			}
		}

		/* offset Area is 0: Map of Area have a Not Valid VirtualOffset = 0xFFFF */
		else
		{
			virtualEEPROMareaContext[eeprom].areaEEPROM[0].virtualOffset = 0xFFFF;
		}
	}
	return success;
}


/**************************************************************************//**
 * @brief  	Starts EEPROM driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError VirtualEEPROMStart(void)
{
	eError 	success = RET_OK;

	VirtualEEPROMinitVirtual(VIRTUAL_EEPROM);
	VirtualEEPROMSearchPointer(VIRTUAL_EEPROM, virtualEEPROMRegister.actualPage);

	return success;
}

/**************************************************************************//**
 * @brief  	Stops EEPROM driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError VirtualEEPROMStop( void )
{
	eError 	success = RET_OK;
	return success;
}

/**************************************************************************//**
 * @brief  	Sleeps EEPROM driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError VirtualEEPROMSleep( void )
{
	eError 	success = RET_OK;
	return success;
}

/**************************************************************************//**
 * @brief  	Wakes up EEPROM driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError VirtualEEPROMWake( void )
{
	eError 	success = RET_OK;
	return success;
}


/**************************************************************************//**
 * @brief  	Stores one value on EEPROM.
 * @param	reg:	register to write.p
 * @param	value : 	value to write in EEPROM
 * @return  eError.
 ****************************************************************************/
eError VirtualEEPROMWriteRegister(tVirtualEEPROM eeprom, uint16_t reg, uint16_t instanceReg, uint32_t value)
{
	eError result = RET_OK;
	uint16_t offset;

	offset = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset + (instanceReg * virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size);

	switch( virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size )
	{
		case 1:
		case 2:
			VirtualEEPROMWriteVariable(VIRTUAL_EEPROM, offset , word_0(value));
			break;

		case 3:
		case 4:
			VirtualEEPROMWriteVariable(VIRTUAL_EEPROM, offset , word_0(value));
			VirtualEEPROMWriteVariable(VIRTUAL_EEPROM, offset+1 , word_1(value));
			break;

		default:
			break;
	}

	return result;
}



/**************************************************************************//**
 * @brief  	Reads one register on EEPROM.
 * @param	reg: 	register to Read.
 * @param   value: 	read value
 * @return  eError.
 ****************************************************************************/
eError VirtualEEPROMReadRegister(tVirtualEEPROM eeprom, uint16_t reg,  uint16_t instanceReg, uint32_t *value)
{
	eError 	result = RET_OK;
	uint16_t offset;
	uint32_t doubleWord = 0;
	uint16_t word = 0;
    uint16_t varExist = 0;

	offset = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset + (instanceReg*virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size);

	switch( virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size  )
	{
		case 1:
		case 2:
			varExist = VirtualEEPROMReadVariable(virtualEEPROMRegister.actualPage, offset, &word, virtualEEPROMRegister.ptrAddrOffset);
			doubleWord = (uint32_t)word;
			break;
		case 3:
		case 4:
			varExist = VirtualEEPROMReadVariable(virtualEEPROMRegister.actualPage, offset+1, &word, virtualEEPROMRegister.ptrAddrOffset);
			doubleWord = ((uint32_t)word)<<16;
			varExist = VirtualEEPROMReadVariable(virtualEEPROMRegister.actualPage, offset, &word, virtualEEPROMRegister.ptrAddrOffset);
			doubleWord = doubleWord | word;
			break;
		default:
			break;
	}

	*value = doubleWord;

    if ( varExist != 0 )
    {
        result = RET_FAIL;
    }

	return result;
}



/**************************************************************************//**
 * @brief  	Initializes the EEPROM driver:
 * 			Restore the pages to a known good state in case of page's status
 *   		corruption after a power loss.
 * @param	eeVMap
 * @return  RET_OK or RET_FAIL
 ****************************************************************************/
eError VirtualEEPROMinitVirtual (tVirtualEEPROMDevice eepromDevice )
{
	uint16_t FlashAreaStatus1 	= 0;
	uint16_t FlashAreaStatus2 	= 0;
	uint32_t MaxOffset;
	eError 	success = RET_OK;

	/* Get Page0 status */
	FlashAreaStatus1 = VirtualEEPROMReadStatus( EEPROMDeviceMap[eepromDevice].flashArea1);
	/* Get Page1 status */
	FlashAreaStatus2 = VirtualEEPROMReadStatus( EEPROMDeviceMap[eepromDevice].flashArea2);

	/* Check for invalid header states and repair if necessary */
	switch (FlashAreaStatus1)
	{
	case ERASED:
		if (FlashAreaStatus2 == VALID_AREA) /* FlashAreaStatus1 erased, FlashAreaStatus2 valid */
		{
			/* Erase FlashAreaStatus1 */
			success = FlashErase( EEPROMDeviceMap[eepromDevice].flashArea1, FLASH_ALL_PAGES);
			if (success != RET_OK)
			{
				return success;
			}
            virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea2;
		}
		else if (FlashAreaStatus2 == RECEIVE_DATA) /* FlashAreaStatus1 erased, FlashAreaStatus2 receive */
		{
			/* Erase FlashAreaStatus1 */
			success = FlashErase( EEPROMDeviceMap[eepromDevice].flashArea1, FLASH_ALL_PAGES);
			if (success != RET_OK)
			{
				return success;
			}
			/* Mark FlashAreaStatus2 as valid */
			success = VirtualEEPROMMarkPageAs(EEPROMDeviceMap[eepromDevice].flashArea2, VALID_AREA);
			if (success != RET_OK)
			{
				return success;
			}

            virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea2;
		}
		else /* First EEPROM access (FlashAreaStatus1&FlashAreaStatus2 are erased) or invalid state -> format EEPROM */
		{
			/* Erase both FlashAreaStatus1 and FlashAreaStatus2 and set FlashAreaStatus1 as valid page */
			success = VirtualEEPROMFormat(eepromDevice);
			/* If erase/program operation was failed, a Flash error code is returned */
			if (success != RET_OK)
			{
				return success;
			}
		}
		/* todo: set crc page 0 and page 1 */
		break;

	case RECEIVE_DATA:
		if (FlashAreaStatus2 == VALID_AREA) /* FlashAreaStatus1 receive, FlashAreaStatus2 valid */
		{
			/* Transfer data from FlashAreaStatus2 to FlashAreaStatus1 */
			VirtualEEPROMSearchPointer(eepromDevice, EEPROMDeviceMap[eepromDevice].flashArea1);
			success = VirtualEEPROMTransferDataFrom(EEPROMDeviceMap[eepromDevice].flashArea2, EEPROMDeviceMap[eepromDevice].flashArea1, TRANSFER_STATE_RESTART);
			if (success != RET_OK)
			{
				return success;
			}
			/* Mark FlashAreaStatus1 as valid */
            virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea1;
			success = VirtualEEPROMMarkPageAs(EEPROMDeviceMap[eepromDevice].flashArea1, VALID_AREA);
			/* If program operation was failed, a Flash error code is returned */
			if (success != RET_OK)
			{
				return success;
			}
			/* Erase FlashAreaStatus2 */
			success = FlashErase( EEPROMDeviceMap[eepromDevice].flashArea2, FLASH_ALL_PAGES);
			if (success != RET_OK)
			{
				return success;
			}
		}
		else if (FlashAreaStatus2 == ERASED) /* FlashAreaStatus1 receive, FlashAreaStatus2 erased */
		{
			/* Erase FlashAreaStatus2 */
			success = FlashErase( EEPROMDeviceMap[eepromDevice].flashArea2, FLASH_ALL_PAGES);
			if (success != RET_OK)
			{
				return success;
			}

			/* Mark FlashAreaStatus1 as valid */
            virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea1;
			success = VirtualEEPROMMarkPageAs(EEPROMDeviceMap[eepromDevice].flashArea1, VALID_AREA);
			/* If program operation was failed, a Flash error code is returned */
			if (success != RET_OK)
			{
				return success;
			}
		}
		else /* Invalid state -> format eeprom */
		{
			/* Erase both FlashAreaStatus1 and Page1 and set FlashAreaStatus1 as valid page */
			success = VirtualEEPROMFormat(eepromDevice);
			/* If erase/program operation was failed, a Flash error code is returned */
			if (success != RET_OK)
			{
				return success;
			}
		}
		/* todo: set crc page 0 and page 1 */
		break;

	case VALID_AREA:
		if (FlashAreaStatus2 == VALID_AREA) /* Invalid state -> format eeprom */
		{
			/* Erase both FlashAreaStatus1 and FlashAreaStatus2 and set FlashAreaStatus1 as valid page */
			success = VirtualEEPROMFormat(eepromDevice);
			/* If erase/program operation was failed, a Flash error code is returned */
			if (success != RET_OK)
			{
				return success;
			}
		}
		else if (FlashAreaStatus2 == ERASED) /* FlashAreaStatus1 valid, FlashAreaStatus2 erased */
		{
			/* Erase FlashAreaStatus2 */
            virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea1;
			success = FlashErase( EEPROMDeviceMap[eepromDevice].flashArea2, FLASH_ALL_PAGES);
			if (success != RET_OK)
			{
				return success;
			}
		}
		else /* FlashAreaStatus1 valid, FlashAreaStatus2 receive */
		{
			VirtualEEPROMSearchPointer(eepromDevice, EEPROMDeviceMap[eepromDevice].flashArea2);
			/* Transfer data from FlashAreaStatus1 to FlashAreaStatus2 */
			success = VirtualEEPROMTransferDataFrom(EEPROMDeviceMap[eepromDevice].flashArea1, EEPROMDeviceMap[eepromDevice].flashArea2, TRANSFER_STATE_RESTART);
			if (success != RET_OK)
			{
				return success;
			}
			/* Mark FlashAreaStatus2 as valid */
            virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea2;
			success = VirtualEEPROMMarkPageAs(EEPROMDeviceMap[eepromDevice].flashArea2, VALID_AREA);
			/* If program operation was failed, a Flash error code is returned */
			if (success != RET_OK)
			{
				return success;
			}
			/* Erase FlashAreaStatus1 */
			success = FlashErase( EEPROMDeviceMap[eepromDevice].flashArea1, FLASH_ALL_PAGES);
			if (success != RET_OK)
			{
				return success;
			}

		}
		/* todo: set crc page 0 and page 1 */
		break;

	default:  /* Any other state -> format EEPROM */
		/* Erase both FlashAreaStatus1 and FlashAreaStatus2 and set FlashAreaStatus1 as valid page */
		success = VirtualEEPROMFormat(eepromDevice);
		/* If erase/program operation was failed, a Flash error code is returned */
		if (success != RET_OK)
		{
			return success;
		}

		break;
	}


	FlashGetSize( virtualEEPROMRegister.actualPage, &MaxOffset);
	virtualEEPROMRegister.maxOffset = MaxOffset;

	return success;
}



/**************************************************************************//**
 * @brief  Returns the last stored variable data, if found, which correspond to
 *   the passed virtual offset
 * @param  virtualOffset: Variable virtual offset
 * @param  Data: Global variable contains the read variable value
 * @retval Success or error status:
 *           - 0: if variable was found
 *           - 1: if the variable was not found
 *           - NO_VALID_AREA: if no valid area was found.
  ****************************************************************************/
uint16_t VirtualEEPROMReadVariable(tFlash areaNumber, uint16_t virtualOffset, uint16_t* Data, uint32_t offsetStartAddr )
{
	//tFlash areaNumber;
	uint16_t OffsetValue 	= 0;
	uint16_t ReadStatus 	= 1;

	uint32_t Offset 		= 0;
	uint16_t startOffset 	= 0;

	/* Check first is value is stored in Cache */
	if ( findInCache(virtualOffset, Data) != RET_OK )
	{

		/* Start to searhc variable from last offset writed on EEPROM */
		Offset = offsetStartAddr -2;

		while (Offset > (startOffset + 2))
		{
			/* Get the current offset content to be compared with virtual offset */
			FlashReadData( areaNumber, &OffsetValue, Offset);

			/* Compare the read offset with the virtual offset */
			if (OffsetValue == virtualOffset)
			{
				/* Get content of offset-2 which is variable value */
				FlashReadData( areaNumber, Data, (Offset - 2) );

				/* In case variable value is read, reset ReadStatus flag */
				ReadStatus = 0;

				/* Add to cache this pair address-value */
				addToCache(virtualOffset, *Data);

				break;
			}
			else
			{
				/* Next address location */
				Offset = Offset - 4;
			}
		}
	}
	else
	{
		ReadStatus = 0;
	}

	/* Return ReadStatus value: (0: variable exist, 1: variable doesn't exist) */
	return ReadStatus;
}


/**************************************************************************//**
 * @brief   Erases FlashAreaStatus1 and FlashAreaStatus2 and writes VALID_AREA header to
 * 			FlashAreaStatus1
 * @param  	None
 * @return  Status of the last operation (Flash write or erase) done during
 *         	EEPROM formating
 ****************************************************************************/
eError VirtualEEPROMFormat(tVirtualEEPROMDevice eepromDevice)
{
	eError FlashStatus = RET_OK;

	/* Erase FlashAreaStatus1 */
	FlashStatus = FlashErase( EEPROMDeviceMap[eepromDevice].flashArea1, FLASH_ALL_PAGES);
	if(FlashStatus == RET_OK)
	{
		FlashStatus = FlashProgramData( EEPROMDeviceMap[eepromDevice].flashArea1, VALID_AREA, PAGE_STATUS);
		virtualEEPROMRegister.ptrAddrOffset = INITIAL_DATA_AREA;
		virtualEEPROMRegister.actualPage =  EEPROMDeviceMap[eepromDevice].flashArea1;
	}

	/* Erase FlashAreaStatus2 */
	if(FlashStatus == RET_OK)
	{
		FlashStatus = FlashErase( EEPROMDeviceMap[eepromDevice].flashArea2, FLASH_ALL_PAGES);
	}
	return FlashStatus;
}


/**************************************************************************//**
 * @brief  Verify if active page is full and Writes variable in EEPROM.
 * @param  VirtAddress: 16 bit virtual address of the variable
 * @param  Data: 16 bit data to be written as variable value
 * @retval Success or error status:
 *           - FLASH_COMPLETE: on success
 *           - AREA_FULL: if valid page is full
 *           - NO_VALID_AREA: if no valid page was found
 *           - Flash error code: on write Flash error
 ****************************************************************************/
uint16_t VirtualEEPROMVerifyPageFullWriteVariable(tFlash areaNumber, uint16_t VirtOffset, uint16_t Data)
{
	eError success 	= RET_OK;

	/* Check if there is no valid page */
	if (areaNumber == NO_VALID_AREA)
	{
		return  NO_VALID_AREA;
	}

	/* Set variable data */
	success = FlashProgramData(areaNumber, Data, virtualEEPROMRegister.ptrAddrOffset);
	if (success != RET_OK)
	{
		return success;
	}

	/* Set variable virtual offset */
	success = FlashProgramData(areaNumber, VirtOffset, virtualEEPROMRegister.ptrAddrOffset+2);
	if (success == RET_OK)
	{
		if ( (virtualEEPROMRegister.ptrAddrOffset + 4) <= virtualEEPROMRegister.maxOffset )
		{
			virtualEEPROMRegister.ptrAddrOffset += 4;
		}
		return success;
	}

	/* Return PAGE_FULL in case the valid page is full */
	return PAGE_FULL;
}


/**************************************************************************//**
 * @brief  Writes/upadtes variable data in EEPROM.
 * @param  VirtAddress: Variable virtual address
 * @param  Data: 16 bit data to be written
 * @retval Success or error status:
 *           - FLASH_COMPLETE: on success
 *           - PAGE_FULL: if valid page is full
 *           - NO_VALID_PAGE: if no valid page was found
 *           - Flash error code: on write Flash error
 ****************************************************************************/
uint16_t VirtualEEPROMWriteVariable(tVirtualEEPROMDevice eepromDevice, uint16_t VirtOffset, uint16_t Data)
{
	uint16_t Status = 0;
	uint16_t diffOffset;

	/* Write the variable virtual address and value in the EEPROM */
	Status = VirtualEEPROMVerifyPageFullWriteVariable(virtualEEPROMRegister.actualPage, VirtOffset, Data);

	/* Update or add to cache */
	updateCache(VirtOffset, Data);

	diffOffset = (virtualEEPROMRegister.maxOffset - virtualEEPROMRegister.ptrAddrOffset);

	if ( diffOffset < EEPROM_BYTES_SWAP )
	{
		VirtualEEPROMTransferPage(eepromDevice);
	}
	else if ( diffOffset < EEPROM_BYTES_TRANSFER_PAGE && diffOffset + 4 >= EEPROM_BYTES_TRANSFER_PAGE )
	{
		VirtualEEPROMEraseOldPage(eepromDevice);
	}

	/* Return last operation status */
	return Status;
}



/**************************************************************************//**
 * @brief   Erase the Old Page if Page is not erased
 * @param  	virtualEEPROM device
 * @return  RET_OK and RET_FAIL
 ****************************************************************************/
eError VirtualEEPROMEraseOldPage( tVirtualEEPROMDevice eepromDevice )
{
	eError success = RET_OK;
	tFlash OldArea;

	if (virtualEEPROMRegister.actualPage == EEPROMDeviceMap[eepromDevice].flashArea2)       /* Area 2 valid */
	{
		OldArea = EEPROMDeviceMap[eepromDevice].flashArea1;
	}
	else	/*Area 1 valid */
	{
		OldArea = EEPROMDeviceMap[eepromDevice].flashArea2;
	}

	success = FlashErase( OldArea, FLASH_ALL_PAGES);
	if(success != RET_OK)
	{
		return success;
	}

	return success;
}


/**************************************************************************//**
 * @brief   Transfer variable From one Page to another
 * @param  	virtualEEPROM device
 * @return  RET_OK and RET_FAIL
 ****************************************************************************/
eError VirtualEEPROMTransferPage( tVirtualEEPROMDevice eepromDevice )
{
	eError success = RET_OK;
	tFlash NewArea;
	tFlash OldArea;
	uint32_t MaxOffset;


	/* Get active Page for read operation */

	if (virtualEEPROMRegister.actualPage == EEPROMDeviceMap[eepromDevice].flashArea2)       /* Area2 valid */
	{
		FlashProgramData(EEPROMDeviceMap[eepromDevice].flashArea1, RECEIVE_DATA, PAGE_STATUS);
		NewArea = EEPROMDeviceMap[eepromDevice].flashArea1;
		OldArea = EEPROMDeviceMap[eepromDevice].flashArea2;
	}
	else if (virtualEEPROMRegister.actualPage == EEPROMDeviceMap[eepromDevice].flashArea1)  /* Area1 valid */
	{
		FlashProgramData(EEPROMDeviceMap[eepromDevice].flashArea2, RECEIVE_DATA, PAGE_STATUS);
		NewArea = EEPROMDeviceMap[eepromDevice].flashArea2;
		OldArea = EEPROMDeviceMap[eepromDevice].flashArea1;
	}
	else
	{
		return NO_VALID_AREA;       /* No valid Page */
	}

	/* Point to new Page */
	virtualEEPROMRegister.ptrAddrOffset = INITIAL_DATA_AREA;
	VirtualEEPROMTransferDataFrom(OldArea, NewArea, TRANSFER_STATE_NO);

	/* Set new Page status to VALID_PAGE status */
	success = FlashProgramData(NewArea, VALID_AREA, PAGE_STATUS);
	if(success != RET_OK)
	{
		return success;
	}

	/* Set old Page status to ERASED status */
	success = FlashProgramData(OldArea, STATUS2_OBSOLETE, PAGE_STATUS2);
	if(success != RET_OK)
	{
		return success;
	}

	/* Update ptrOffset HEADER */
	virtualEEPROMRegister.actualPage = NewArea;
	FlashGetSize( NewArea, &MaxOffset);
	virtualEEPROMRegister.maxOffset = MaxOffset;

	return success;
}



/**************************************************************************//**
 * @brief   Marks a page with the indicated value
 * @param  	Value:        Specifies the data to be programmed
 * @return  HAL_StatusTypeDef FlashMarkStatus
 ****************************************************************************/
static eError VirtualEEPROMMarkPageAs(tFlash flashArea, uint16_t Value)
{
	eError FlashMarkStatus = RET_OK;
	FlashMarkStatus = FlashProgramData( flashArea, Value, PAGE_STATUS);
	return FlashMarkStatus;
}


/**************************************************************************//**
 * @brief   Marks a page with the indicated value
 * @param  	Value:        Specifies the data to be programmed
 * @return  HAL_StatusTypeDef FlashMarkStatus
 ****************************************************************************/
static uint16_t VirtualEEPROMReadStatus( tFlash flashArea )
{
	uint16_t FlashAreaStatus = 0;

	FlashReadData(flashArea, &FlashAreaStatus, PAGE_STATUS2);
	if (FlashAreaStatus == STATUS2_OBSOLETE )
	{
		FlashAreaStatus = ERASED;
	}
	else
	{
		FlashReadData(flashArea, &FlashAreaStatus, PAGE_STATUS);
	}

	return FlashAreaStatus;
}



/**************************************************************************//**
 * @brief   Search if virtualOffset is in the areaNumber
 * @param  	VirtualOffset
 * @return  RET_OK (exist) or RET_FAIL
 ****************************************************************************/
static eError VirtualEEPROMAdressExist( tFlash areaNumber, uint16_t VirtOffset ,uint8_t transferState)
{
	uint16_t Virtaddress, data;
	uint32_t address;
	if (transferState == TRANSFER_STATE_RESTART)
	{
		for ( address=INITIAL_DATA_AREA; address < virtualEEPROMRegister.maxOffset; address=address+4 )
		{
			FlashReadData(areaNumber, &data, address);
			FlashReadData(areaNumber, &Virtaddress, address+2);
			if ( Virtaddress == VirtOffset )
			{
				return RET_OK;
			}
			else if (data==0xFFFF && Virtaddress==0xFFFF)
			{
				break;
			}
		}
	}
	return RET_FAIL;
}


/**************************************************************************//**
 * @brief   Transfers data from one page to other page
 * @param  	OldArea, NewArea
 * @return  EepromStatus:	result of verifying data and read memory
 ****************************************************************************/
eError VirtualEEPROMTransferDataFrom(tFlash flashAreaOld, tFlash flashAreaNew, uint8_t transferState)
{
	eError success = RET_OK;

	uint16_t eepromTable, eepromVar, instance;
	uint32_t DataVar = 0;
	uint16_t word;
	uint16_t ReadStatus = 0;
	uint16_t EepromStatus 	= 0;
	uint16_t offsetVar;

	/* Transfer process: transfer variables from old to the new active page */
	for (eepromTable = 0; eepromTable < NUM_OF_TABLE_EEPROMS; eepromTable++ )
	{
		for (eepromVar = 0; eepromVar < virtualEEPROMareaContext[eepromTable].areaSize; eepromVar++)
		{
			for (instance=0; instance < virtualEEPROMInstanceMap[eepromTable].regTable[eepromVar].numOfInstances; instance++)
			{
				/* Read the other last variable updates */
				offsetVar = virtualEEPROMareaContext[eepromTable].areaEEPROM[eepromVar].virtualOffset + (virtualEEPROMareaContext[eepromTable].areaEEPROM[eepromVar].size * instance);
				if ( virtualEEPROMareaContext[eepromTable].areaEEPROM[eepromVar].size > 2 )
				{
					if ( findInCache(offsetVar, &word) != RET_OK )
					{
						ReadStatus = VirtualEEPROMReadVariable(flashAreaOld, offsetVar+1, &word, virtualEEPROMRegister.maxOffset);
					}
					DataVar = ((uint32_t)word)<<16;
					if ( findInCache(offsetVar+1, &word) != RET_OK )
					{
						ReadStatus = VirtualEEPROMReadVariable(flashAreaOld, offsetVar, &word, virtualEEPROMRegister.maxOffset);
					}
					else
					{
						ReadStatus = 0;
					}
					DataVar = DataVar | word;
				}
				else
				{
					if ( findInCache(offsetVar, &word) != RET_OK )
					{
						ReadStatus = VirtualEEPROMReadVariable(flashAreaOld, offsetVar, &word, virtualEEPROMRegister.maxOffset);
					}
					else
					{
						ReadStatus = 0;
					}
					DataVar = (uint32_t)word;
				}

				/* In case variable corresponding to the virtual address was found */
				if ( ReadStatus != 0x1 && VirtualEEPROMAdressExist(flashAreaNew, offsetVar, transferState)==RET_FAIL)
				{
					/* Transfer the variable to the new active page */
					EepromStatus = VirtualEEPROMVerifyPageFullWriteVariable(flashAreaNew, offsetVar, word_0(DataVar));
					updateCache(offsetVar, word_0(DataVar));
					if ( virtualEEPROMareaContext[eepromTable].areaEEPROM[eepromVar].size > 2 )
					{
						EepromStatus = VirtualEEPROMVerifyPageFullWriteVariable(flashAreaNew, offsetVar+1, word_1(DataVar));
						updateCache(offsetVar+1, word_1(DataVar));
					}

					/* If program operation was failed, a Flash error code is returned */
					if (EepromStatus != RET_OK)
					{
						return EepromStatus;
					}
				}
			}
		}
	}

	return success;
}




/**************************************************************************//**
 * @brief  Search the ptrOffset of the VirtualEEPROM
 * @param  eepromDevice
 * @retval RET_OK or RET_FAIL
 ****************************************************************************/
eError VirtualEEPROMSearchPointer(tVirtualEEPROMDevice eepromDevice, tFlash flashArea)
{
	eError success 	= RET_OK;
	uint16_t OffsetValue, OffsetValue2;
	uint32_t diffOffset = 0, MaxOffset;

	/* HEADER virtualEEPROM offset */
	FlashGetSize( flashArea, &MaxOffset);
	virtualEEPROMRegister.maxOffset = MaxOffset;
	virtualEEPROMRegister.ptrAddrOffset = MaxOffset;

	while ( virtualEEPROMRegister.ptrAddrOffset > INITIAL_DATA_AREA)
	{
		success = FlashReadData( flashArea, &OffsetValue, virtualEEPROMRegister.ptrAddrOffset - 2);
		success = FlashReadData( flashArea, &OffsetValue2, virtualEEPROMRegister.ptrAddrOffset - 4);
		if ( OffsetValue == 0xFFFF && OffsetValue2 == 0xFFFF )
		{
			virtualEEPROMRegister.ptrAddrOffset -= 4;
		}
		else
		{
			break;
		}
	}

	diffOffset = (virtualEEPROMRegister.maxOffset - virtualEEPROMRegister.ptrAddrOffset);
	if ( diffOffset < EEPROM_BYTES_SWAP )
	{
		VirtualEEPROMTransferPage(eepromDevice);
	}

	return success;
}


/**************************************************************************//**
 * @brief  Search the for an address-Value on Cache
 * @param  virtualAddress to search
 * @param  returned Value stored in cache in case of hit
 * @retval RET_OK or RET_FAIL
 ****************************************************************************/
eError findInCache( uint16_t virtualAddress, uint16_t *value )
{
	uint8_t cacheIndex = 0;
	eError result = RET_FAIL;
#ifdef EEPROM_REVERSE_CACHE
	for ( cacheIndex = VirtualEEPROMCacheUsed; cacheIndex > 0; --cacheIndex)
#else
	for ( cacheIndex = 0; cacheIndex < VirtualEEPROMCacheUsed; cacheIndex++)
#endif
	{
		if ( virtualEEPROMCache[cacheIndex].virtualAddress == virtualAddress)
		{
			*value = virtualEEPROMCache[cacheIndex].value;
			result = RET_OK;
			break;
		}
	}

	return result;
}

/**************************************************************************//**
 * @brief  Update or Add new pair Address-value to cache
 * @param  virtualAddress to search
 * @param  new value for the virtual Address
 * @retval RET_OK or RET_FAIL
 ****************************************************************************/
eError updateCache( uint16_t virtualAddress, uint16_t value )
{
	uint8_t cacheIndex = 0;
	eError result = RET_FAIL;

#ifdef EEPROM_REVERSE_CACHE
	for ( cacheIndex = VirtualEEPROMCacheUsed; cacheIndex > 0; --cacheIndex)
#else
	for ( cacheIndex = 0; cacheIndex < VirtualEEPROMCacheUsed; cacheIndex++)
#endif
	{
		if ( virtualEEPROMCache[cacheIndex].virtualAddress == virtualAddress)
		{
			virtualEEPROMCache[cacheIndex].value = value;
			result = RET_OK;
			break;
		}
	}

	/* If virtual Address is not inside cache, try to add in case of space available */
	if ( result != RET_OK )
	{
		result = addToCache( virtualAddress, value);
	}

	return result;
}

/**************************************************************************//**
 * @brief  Add new pair Address-value to cache
 * @param  virtualAddress to add
 * @param  Value for the new virtual Address
 * @retval RET_OK or RET_FAIL
 ****************************************************************************/
eError addToCache( uint16_t virtualAddress, uint16_t value )
{
	eError result = RET_FAIL;

	if ( (VirtualEEPROMCacheUsed < SIZE_OF_VIRTUAL_EEPROM_CACHE) )
	{
		virtualEEPROMCache[VirtualEEPROMCacheUsed].virtualAddress = virtualAddress;
		virtualEEPROMCache[VirtualEEPROMCacheUsed].value = value;
		VirtualEEPROMCacheUsed++;

		result = RET_OK;
	}

	return result;
}


#endif /* _EEPROM_C_ */

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ****************************************************************************/
