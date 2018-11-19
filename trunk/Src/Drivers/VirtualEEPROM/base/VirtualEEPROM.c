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
#define READ_FROM_VALID_AREA  	((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE   	((uint8_t)0x01)

/* Page full define */
#define PAGE_FULL             	((uint8_t)0x80)

/* Number of pages */
#define ONE_PAGE  				((uint8_t)0x01)

#define FLASH_MOD_SIZE(size) 	((size%2) ? 1  : 0)

#define EEPROM_BYTES_TRANSFER_PAGE 20
#define EEPROM_BYTES_SWAP 8

#define INITIAL_DATA_AREA 8

/****************************************************************************
 *  EXTERN VARIABLES
 ****************************************************************************/
extern const tVirtualEEPROMDeviceMap EEPROMDeviceMap[NUM_OF_VIRTUAL_EEPROMS];
extern const tVirtualEEPROMInstanceMap VirtualEEPROMInstanceMap[NUM_OF_TABLE_EEPROM];
/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
tVirtualEEPROMRegister virtualEEPROMRegister;

tVirtualEEPROMContext virtualEEPROMhregContext[EEPROM_NUM_OF_HREG];
tVirtualEEPROMContext virtualEEPROMsregFactoryContext[EEPROM_NUM_OF_SREG_FACTORY];
tVirtualEEPROMContext virtualEEPROMsregCalibrationContext[EEPROM_NUM_OF_SREG_CALIBRATION];
tVirtualEEPROMContext virtualEEPROMsregUserContext[EEPROM_NUM_OF_SREG_USER];
tVirtualEEPROMContext virtualEEPROMdataContext[EEPROM_NUM_OF_DATA];

const tVirtualEEPROMareaContext virtualEEPROMareaContext [NUM_OF_TABLE_EEPROM] =
{
		{ EEPROM_NUM_OF_HREG,				(tVirtualEEPROMContext*)&virtualEEPROMhregContext			},
		{ EEPROM_NUM_OF_SREG_FACTORY,		(tVirtualEEPROMContext*)&virtualEEPROMsregFactoryContext	},
		{ EEPROM_NUM_OF_SREG_CALIBRATION,	(tVirtualEEPROMContext*)&virtualEEPROMsregCalibrationContext},
		{ EEPROM_NUM_OF_SREG_USER,			    (tVirtualEEPROMContext*)&virtualEEPROMsregUserContext		},
		{ EEPROM_NUM_OF_DATA,				(tVirtualEEPROMContext*)&virtualEEPROMdataContext			},
};


tVirtualEEPROMCache virtualEEPROMCache[SIZE_OF_VIRTUAL_EEPROM_CACHE];
uint8_t	VirtualEEPROMCacheUsed = 0;

/****************************************************************************
 *  PRIVATE FUNCTIONS
 ****************************************************************************/
eError VirtualEEPROMinitVirtual (tVirtualEEPROMDevice eepromDevice );
eError VirtualEEPROMSearchPointer(tVirtualEEPROMDevice eepromDevice);

uint16_t VirtualEEPROMWriteVariable(tVirtualEEPROMDevice eepromDevice , uint16_t VirtOffset, uint16_t Data);
uint16_t VirtualEEPROMReadVariable(tFlash areaNumber, uint16_t virtualOffset, uint16_t* Data, uint32_t offsetStartAddr );

eError VirtualEEPROMFormat(tVirtualEEPROMDevice eepromDevice );
tFlash VirtualEEPROMFindValidArea(tVirtualEEPROMDevice eepromDevice , uint8_t Operation);
eError VirtualEEPROMPageTransfer(tVirtualEEPROMDevice eepromDevice , uint16_t VirtOffset, uint16_t Data);
uint16_t VirtualEEPROMVerifyPageFullWriteVariable(tFlash areaNumber , uint16_t VirtOffset, uint16_t Data);

eError VirtualEEPROMTransferPage( tVirtualEEPROMDevice eepromDevice );
eError VirtualEEPROMEraseOldPage( tVirtualEEPROMDevice eepromDevice );
eError VirtualEEPROMTransferDataFrom(tFlash flashAreaNew, tFlash flashAreaOld);

eError findInCache( uint16_t virtualAdress, uint16_t *value );
eError updateCache( uint16_t virtualAdress, uint16_t value );
eError addToCache( uint16_t virtualAdress, uint16_t value );

static uint8_t EEPROMGetNumOfInstances(tVirtualEEPROM eeprom, uint16_t reg);
static uint16_t EEPROMGetAreaSize(tVirtualEEPROM eeprom, uint16_t reg);

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

	for (eeprom = 0; eeprom < NUM_OF_TABLE_EEPROM; eeprom++)
	{
		/* if offsetArea is 0, ignore this Area */
		if ( VirtualEEPROMInstanceMap[eeprom].offsetArea != 0 )
		{
			areaSize = virtualEEPROMareaContext[eeprom].areaSize;
			for (reg = 0; reg < areaSize; reg++ )
			{
				virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset = nextVirtOffset;
				virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size = EEPROMGetAreaSize(eeprom,reg);
				nextNumInst = EEPROMGetNumOfInstances(eeprom,reg);
				nextVirtOffset = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset + nextNumInst * virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size;
			}

			/* Check that offset is between the reserved offset area */
			if ( nextVirtOffset > (VirtualEEPROMInstanceMap[eeprom].offsetArea + lastOffsetArea))
			{
				return RET_NOT_INITIALIZED;
			}
			else
			{
				nextVirtOffset = VirtualEEPROMInstanceMap[eeprom].offsetArea + lastOffsetArea;
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

//	VirtualEEPROMinitVirtual(VIRTUAL_EEPROM);
//	VirtualEEPROMSearchPointer(VIRTUAL_EEPROM);

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
	uint16_t offset = 0;
	uint8_t size = 0;

	if (eeprom != EEPROM_DATA || reg < EEPROM_HEADER)
	{
		offset = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset +
				(instanceReg * virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size);
		size = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size;
	}
	else
	{
		if ( instanceReg < virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size )
		{
			offset = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset + instanceReg;
			size = 2;
		}
	}

	switch( size )
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
	uint16_t offset = 0;
	uint32_t doubleWord = 0;
	uint16_t word = 0;
    uint16_t varExist = 0;
    uint8_t size = 0;

	if (eeprom != EEPROM_DATA || reg < EEPROM_HEADER)
	{
		offset = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset +
				(instanceReg * virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size);
		size = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size;
	}
	else
	{
		if ( instanceReg < virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size )
		{
			offset = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset + instanceReg;
			size = 2;
		}
	}

	switch( size )
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

	eError 	success = RET_OK;

	/* Get Page0 status */
	FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea1, &FlashAreaStatus1, PAGE_STATUS2);
	if (FlashAreaStatus1 == STATUS2_OBSOLETE )
	{
		FlashAreaStatus1 = ERASED;
	}
	else
	{
		FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea1, &FlashAreaStatus1, PAGE_STATUS);
	}

	/* Get Page1 status */
	FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea2, &FlashAreaStatus2, PAGE_STATUS2);
	if (FlashAreaStatus2 == STATUS2_OBSOLETE )
	{
		FlashAreaStatus2 = ERASED;
	}
	else
	{
		FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea2, &FlashAreaStatus2, PAGE_STATUS);
	}

	/* Check for invalid header states and repair if necessary */
	switch (FlashAreaStatus1)
	{
	case ERASED:
		if (FlashAreaStatus2 == VALID_AREA) /* FlashAreaStatus1 erased, FlashAreaStatus2 valid */
		{
            virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea2;
			/* Erase Page0 */
			success = FlashErase( EEPROMDeviceMap[eepromDevice].flashArea1, FLASH_ALL_PAGES);
			if (success != RET_OK)
			{
				return success;
			}
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
            virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea2;
			success = VirtualEEPROMMarkPageAs(EEPROMDeviceMap[eepromDevice].flashArea2, VALID_AREA);
			if (success != RET_OK)
			{
				return success;
			}
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
			success = VirtualEEPROMTransferDataFrom(EEPROMDeviceMap[eepromDevice].flashArea2, EEPROMDeviceMap[eepromDevice].flashArea1);
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
			/* Transfer data from FlashAreaStatus1 to FlashAreaStatus2 */
			success = VirtualEEPROMTransferDataFrom(EEPROMDeviceMap[eepromDevice].flashArea1, EEPROMDeviceMap[eepromDevice].flashArea2);
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
 * @brief  Find valid Area for write or read operation
 * @param  Operation: operation to achieve on the valid page.
 *   This parameter can be one of the following values:
 *     @arg READ_FROM_VALID_PAGE: read operation from valid page
 *     @arg WRITE_IN_VALID_PAGE: write operation from valid page
 * @retval Valid page number (FlashAreaStatus1 or PAGE1) or NO_VALID_PAGE in case
 *   of no valid page was found
 ****************************************************************************/
tFlash VirtualEEPROMFindValidArea(tVirtualEEPROMDevice eepromDevice, uint8_t Operation)
{
	tFlash areaNumber 	= NO_VALID_AREA;

	uint16_t FlashAreaStatus1 	= 0;
	uint16_t FlashAreaStatus2 	= 0;

	/* Get Page0 status */
	FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea1, &FlashAreaStatus1, PAGE_STATUS2);
	if (FlashAreaStatus1 == STATUS2_OBSOLETE )
	{
		FlashAreaStatus1 = ERASED;
	}
	else
	{
		FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea1, &FlashAreaStatus1, PAGE_STATUS);
	}

	/* Get Page1 status */
	FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea2, &FlashAreaStatus2, PAGE_STATUS2);
	if (FlashAreaStatus2 == STATUS2_OBSOLETE )
	{
		FlashAreaStatus2 = ERASED;
	}
	else
	{
		FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea2, &FlashAreaStatus2, PAGE_STATUS);
	}

	/* Write or read operation */
	switch (Operation)
	{
	/* ---- Write operation ---- */
	case WRITE_IN_VALID_PAGE:
		if (FlashAreaStatus2 == VALID_AREA)
		{
			/* Page0 receiving data */
			if (FlashAreaStatus1 == RECEIVE_DATA)
			{
				areaNumber = EEPROMDeviceMap[eepromDevice].flashArea1;         /* Area1 valid */
			}
			else
			{
				areaNumber = EEPROMDeviceMap[eepromDevice].flashArea2;         /* Area2 valid */
			}
		}
		else if (FlashAreaStatus1 == VALID_AREA)
		{
			/* Page1 receiving data */
			if (FlashAreaStatus2 == RECEIVE_DATA)
			{
				areaNumber = EEPROMDeviceMap[eepromDevice].flashArea2;         /* Area2 valid */
			}
			else
			{
				areaNumber = EEPROMDeviceMap[eepromDevice].flashArea1;         /* Area1 valid */
			}
		}
		else
		{
			areaNumber = NO_VALID_AREA;   /* No valid Area */
		}
		break;

		/* ---- Read operation ---- */
	case READ_FROM_VALID_AREA:
		if (FlashAreaStatus1 == VALID_AREA)
		{
			areaNumber = EEPROMDeviceMap[eepromDevice].flashArea1;           /* Area1 valid */
		}
		else if (FlashAreaStatus2 == VALID_AREA)
		{
			areaNumber = EEPROMDeviceMap[eepromDevice].flashArea2;           /* Area2 valid */
		}
		else
		{
			areaNumber = NO_VALID_AREA ;  /* No valid Area */
		}
		break;

	default:
		areaNumber = EEPROMDeviceMap[eepromDevice].flashArea1;             /* Area1 valid */
		break;
	}

	return areaNumber;
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
	else if ( diffOffset < EEPROM_BYTES_TRANSFER_PAGE )
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
	uint16_t FlashAreaStatus = 0;
	tFlash OldArea;

	if (virtualEEPROMRegister.actualPage == EEPROMDeviceMap[eepromDevice].flashArea2)       /* Area 2 valid */
	{
		FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea1, &FlashAreaStatus, PAGE_STATUS2);
		if (FlashAreaStatus != STATUS2_OBSOLETE )
		{
			FlashAreaStatus = ERASED;
		}
		else
		{
			FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea1, &FlashAreaStatus, PAGE_STATUS);
		}
		OldArea = EEPROMDeviceMap[eepromDevice].flashArea1;
	}
	else	/*Area 1 valid */
	{
		FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea2, &FlashAreaStatus, PAGE_STATUS2);
		if (FlashAreaStatus != STATUS2_OBSOLETE )
		{
			FlashAreaStatus = ERASED;
		}
		else
		{
			FlashReadData(EEPROMDeviceMap[eepromDevice].flashArea2, &FlashAreaStatus, PAGE_STATUS);
		}
		OldArea = EEPROMDeviceMap[eepromDevice].flashArea2;
	}

	if ( FlashAreaStatus != ERASED )
	{
		success = FlashErase( OldArea, FLASH_ALL_PAGES);
		if(success != RET_OK)
		{
			return success;
		}
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

	uint16_t eepromTable, eepromVar, instance, offsetInstance;
	uint32_t MaxOffset;
	uint16_t offsetVar;
	uint16_t EepromStatus 	= 0;
	uint16_t ReadStatus 	= 0;
	uint16_t DataVar		= 0;

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

	/* Transfer process: transfer variables from old to the new active page */
	for (eepromTable = 0; eepromTable < NUM_OF_TABLE_EEPROM; eepromTable++ )
	{
		for (eepromVar = 0; eepromVar < virtualEEPROMareaContext[eepromTable].areaSize; eepromVar++)
		{
			for (instance=0; instance < EEPROMGetNumOfInstances(eepromTable,eepromVar); instance++)
			{
				for(offsetInstance=0; offsetInstance < EEPROMGetAreaSize(eepromTable, eepromVar); offsetInstance+=2)
				{
					/* Read the other last variable updates */
					offsetVar = virtualEEPROMareaContext[eepromTable].areaEEPROM[eepromVar].virtualOffset + (virtualEEPROMareaContext[eepromTable].areaEEPROM[eepromVar].size * instance);
					if ( findInCache(offsetVar+(offsetInstance/2), &DataVar) != RET_OK )
					{
						ReadStatus = VirtualEEPROMReadVariable(OldArea, offsetVar+(offsetInstance/2), &DataVar, virtualEEPROMRegister.maxOffset);
					}
					else
					{
						ReadStatus = 0;
					}

					/* In case variable corresponding to the virtual address was found */
					if (ReadStatus != 0x1)
					{
						/* Transfer the variable to the new active page */
						EepromStatus = VirtualEEPROMVerifyPageFullWriteVariable(NewArea, offsetVar+(offsetInstance/2), DataVar);
						/* If program operation was failed, a Flash error code is returned */
						if (EepromStatus != RET_OK)
						{
							return EepromStatus;
						}
					}
				}
			}
		}
	}

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
eError VirtualEEPROMMarkPageAs(tFlash flashArea, uint16_t Value)
{
	eError FlashMarkStatus = RET_OK;
	FlashMarkStatus = FlashProgramData( flashArea, Value, PAGE_STATUS);
	return FlashMarkStatus;
}

/**************************************************************************//**
 * @brief   Transfers data from one page to other page
 * @param  	OldArea, NewArea
 * @return  EepromStatus:	result of verifying data and read memory
 ****************************************************************************/
eError VirtualEEPROMTransferDataFrom(tFlash flashAreaOld, tFlash flashAreaNew)
{
	eError success = RET_OK;

	uint16_t eepromTable, eepromVar, instance;
	uint16_t DataVar 			= 0;
	uint16_t ReadStatus = 0;
	uint16_t EepromStatus 	= 0;
	uint16_t FlashOffset;
	uint16_t offsetVar;

	int16_t x = -1;

	/* Transfer process: transfer variables from old to the new active page */
	for (eepromTable = 0; eepromTable < NUM_OF_TABLE_EEPROM; eepromTable++ )
	{
		for (eepromVar = 0; eepromVar < virtualEEPROMareaContext[eepromTable].areaSize; eepromVar++)
		{
			for (instance=0; instance < EEPROMGetNumOfInstances(eepromTable,eepromVar); instance++)
			{
				FlashReadData(flashAreaOld, &FlashOffset, PAGE_STATUS + 6 );
				if ( FlashOffset == virtualEEPROMareaContext[eepromTable].areaEEPROM[eepromVar].virtualOffset )
				{
					x = eepromVar;
				}
				if (eepromVar != x)
				{
					/* Read the other last variable updates */
					offsetVar = virtualEEPROMareaContext[eepromTable].areaEEPROM[eepromVar].virtualOffset + (virtualEEPROMareaContext[eepromTable].areaEEPROM[eepromVar].size * instance);
					if ( findInCache(offsetVar, &DataVar) != RET_OK )
					{
						ReadStatus = VirtualEEPROMReadVariable(flashAreaOld, offsetVar, &DataVar, virtualEEPROMRegister.maxOffset);
					}
					else
					{
						ReadStatus = 0;
					}

					/* In case variable corresponding to the virtual address was found */
					if (ReadStatus != 0x1)
					{
						/* Transfer the variable to the new active page */
						EepromStatus = VirtualEEPROMVerifyPageFullWriteVariable(flashAreaNew, offsetVar, DataVar);
						/* If program operation was failed, a Flash error code is returned */
						if (EepromStatus != RET_OK)
						{
							return EepromStatus;
						}
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
eError VirtualEEPROMSearchPointer(tVirtualEEPROMDevice eepromDevice)
{
	eError success 	= RET_OK;
	tFlash areaNumber;
	uint16_t OffsetValue, OffsetValue2;

	uint32_t diffOffset = 0;
	uint32_t MaxOffset;

	areaNumber = VirtualEEPROMFindValidArea(eepromDevice, READ_FROM_VALID_AREA);

	virtualEEPROMRegister.actualPage = areaNumber;
	FlashGetSize( areaNumber, &MaxOffset);
	virtualEEPROMRegister.maxOffset = MaxOffset;

	/* HEADER virtualEEPROM offset */
	virtualEEPROMRegister.ptrAddrOffset = virtualEEPROMRegister.maxOffset;

	while ( virtualEEPROMRegister.ptrAddrOffset > INITIAL_DATA_AREA)
	{
		success = FlashReadData( areaNumber, &OffsetValue, virtualEEPROMRegister.ptrAddrOffset - 2);
		success = FlashReadData( areaNumber, &OffsetValue2, virtualEEPROMRegister.ptrAddrOffset - 4);
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



/*****************************************************************************
 * @brief  Get num of instances depending on the tEEPROM table
 * @param  EEPROM Table and register
 * @return num of instances
 ***************************************************************************/
static uint8_t EEPROMGetNumOfInstances(tVirtualEEPROM eeprom, uint16_t reg)
{
	uint8_t num;
	if (VirtualEEPROMInstanceMap[eeprom].typeArea == EEPROM_AREA_INST)
	{
		num = VirtualEEPROMInstanceMap[eeprom].regTable.withInst[reg].numOfInstances;
	}
	else
	{
		num = UNIQUE_EEPROM_INSTANCE;
	}
	return num;
}

/*****************************************************************************
 * @brief  Get size of register depending on the tEEPROM table
 * @param  EEPROM Table and register
 * @return size
 ***************************************************************************/
static uint16_t EEPROMGetAreaSize(tVirtualEEPROM eeprom, uint16_t reg)
{
	uint16_t size;
	if (VirtualEEPROMInstanceMap[eeprom].typeArea == EEPROM_AREA_INST)
	{
		size = VirtualEEPROMInstanceMap[eeprom].regTable.withInst[reg].sizeOfInstance;
	}
	else
	{
		size = VirtualEEPROMInstanceMap[eeprom].regTable.noInst[reg].sizeOfInstance;
	}
	return size;
}


#endif /* _EEPROM_C_ */

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ****************************************************************************/
