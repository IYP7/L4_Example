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
/* No valid page define */
#define NO_VALID_AREA         	((uint16_t)0x00AB)

/* Valid pages in read and write defines */
#define READ_FROM_VALID_AREA  	((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE   	((uint8_t)0x01)

/* Page full define */
#define PAGE_FULL             	((uint8_t)0x80)

/* Number of pages */
#define ONE_PAGE  				((uint8_t)0x01)

#define FLASH_MOD_SIZE(size) 	((size%2) ? 1  : 0)

#define EEPROM_BYTES_SWAP 32

#define INITIAL_DATA_AREA 32

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

uint16_t VirtualEEPROMWriteVariable(tVirtualEEPROMDevice eepromDevice , uint16_t VirtOffset, uint32_t Data);
uint16_t VirtualEEPROMReadVariable(tFlash areaNumber, uint16_t virtualOffset, uint32_t* Data, uint32_t offsetStartAddr );

eError VirtualEEPROMFormat(tVirtualEEPROMDevice eepromDevice );
tFlash VirtualEEPROMFindValidArea(tVirtualEEPROMDevice eepromDevice , uint8_t Operation);
eError VirtualEEPROMPageTransfer(tVirtualEEPROMDevice eepromDevice , uint16_t VirtOffset, uint16_t Data);
uint16_t VirtualEEPROMVerifyPageFullWriteVariable(tFlash areaNumber , uint16_t VirtOffset, uint32_t Data);

eError VirtualEEPROMTransferPage( tVirtualEEPROMDevice eepromDevice );
eError VirtualEEPROMEraseOldPage( tVirtualEEPROMDevice eepromDevice );
eError VirtualEEPROMTransferDataFrom(tFlash flashAreaNew, tFlash flashAreaOld);

eError findInCache( uint16_t virtualAdress, uint32_t *value );
eError updateCache( uint16_t virtualAdress, uint32_t value );
eError addToCache( uint16_t virtualAdress, uint32_t value );

static uint8_t EEPROMGetNumOfInstances(tVirtualEEPROM eeprom, uint16_t reg);
static uint16_t EEPROMGetAreaSize(tVirtualEEPROM eeprom, uint16_t reg);

static EE_State_type GetPageState(tFlash flashArea);
static eError SetPageState(tFlash flashArea, EE_State_type State);

uint16_t CalculateCrc(EE_DATA_TYPE Data, uint16_t VirtAddress);
void ConfigureCrc(void);
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
	uint16_t nextVirtOffset = 0x1;
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
				nextVirtOffset = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset + nextNumInst;
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

	VirtualEEPROMinitVirtual(VIRTUAL_EEPROM);
	VirtualEEPROMSearchPointer(VIRTUAL_EEPROM);

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
	eError result = RET_FAIL;
	uint16_t offset = 0;

	if ( instanceReg < virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size )
	{
		offset = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset + instanceReg;
		result = VirtualEEPROMWriteVariable(VIRTUAL_EEPROM, offset , value);
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
	uint32_t varValue = 0;
    uint16_t varExist = 0;

	if ( instanceReg < virtualEEPROMareaContext[eeprom].areaEEPROM[reg].size )
	{
		offset = virtualEEPROMareaContext[eeprom].areaEEPROM[reg].virtualOffset + instanceReg;
		varExist = VirtualEEPROMReadVariable(virtualEEPROMRegister.actualPage, offset, &varValue, virtualEEPROMRegister.ptrAddrOffset);
		*value = varValue;
	}

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
	EE_State_type FlashAreaStatus1, FlashAreaStatus2;

	eError 	success = RET_OK;

	/* Get Page0 status */
	FlashAreaStatus1 = GetPageState(EEPROMDeviceMap[eepromDevice].flashArea1);

	/* Get Page1 status */
	FlashAreaStatus2 = GetPageState(EEPROMDeviceMap[eepromDevice].flashArea2);

	switch(FlashAreaStatus1)
	{
		case STATE_PAGE_ERASING:
		case STATE_PAGE_ERASED:
			virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea2;
			if(FlashAreaStatus2 == STATE_PAGE_ACTIVE)
			{
			}
			else if(FlashAreaStatus2 == STATE_PAGE_RECEIVE)
			{
				/* Erase FlashAreaStatus1 */
				if(FlashAreaStatus1 == STATE_PAGE_ERASING)
				{
					success = VirtualEEPROMEraseOldPage(eepromDevice);
					if (success != RET_OK)
					{
						return success;
					}
				}
				/* Mark FlashAreaStatus2 as valid */
			    success = SetPageState(EEPROMDeviceMap[eepromDevice].flashArea2, STATE_PAGE_ACTIVE);
				if (success != RET_OK)
				{
					return success;
				}
			}
			else if(FlashAreaStatus2 == STATE_PAGE_VALID)
			{
				success = VirtualEEPROMTransferPage(eepromDevice);
				if(success != RET_OK)
				{
					return success;
				}
			}
			else /* Invalid state -> format eeprom */
			{
	  			/* Erase both FlashAreas set FlashArea1 as valid page */
	  			success = VirtualEEPROMFormat(eepromDevice);
	  			/* If erase/program operation was failed, a Flash error code is returned */
	  			if (success != RET_OK)
	  			{
	  				return success;
	  			}
			}
			break;

		case STATE_PAGE_ACTIVE:
			virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea1;
			if(FlashAreaStatus2 == STATE_PAGE_ACTIVE)
			{
		  		success = VirtualEEPROMFormat(eepromDevice);
		  		/* If erase/program operation was failed, a Flash error code is returned */
		  		if (success != RET_OK)
		  		{
		  			return success;
		  		}
			}
			else if(FlashAreaStatus2 != STATE_PAGE_ERASED)
			{
				/* Erase FlashAreaStatus2 */
				success = VirtualEEPROMEraseOldPage(eepromDevice);
				if(success != RET_OK)
				{
					return success;
				}
			}
			break;

		case STATE_PAGE_RECEIVE:
			if(FlashAreaStatus2 == STATE_PAGE_VALID)
			{
				virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea2;
				success = VirtualEEPROMTransferPage(eepromDevice);
				if(success != RET_OK)
				{
					return success;
				}
			}
			else
			{
				virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea1;
				success = VirtualEEPROMEraseOldPage(eepromDevice);
				if(success != RET_OK)
				{
					return success;
				}
				/* Mark FlashAreaStatus2 as valid */
			    success = SetPageState(EEPROMDeviceMap[eepromDevice].flashArea1, STATE_PAGE_ACTIVE);
				if (success != RET_OK)
				{
					return success;
				}
			}
			break;

			case STATE_PAGE_VALID:
				virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea1;
				if(FlashAreaStatus2 == STATE_PAGE_RECEIVE)
				{
					success = VirtualEEPROMTransferPage(eepromDevice);
					if(success != RET_OK)
					{
						return success;
					}
				}
				else if(FlashAreaStatus2 == STATE_PAGE_ACTIVE)
				{
					virtualEEPROMRegister.actualPage = EEPROMDeviceMap[eepromDevice].flashArea2;
					success = VirtualEEPROMEraseOldPage(eepromDevice);
					if(success != RET_OK)
					{
						return success;
					}
				}
				else if(FlashAreaStatus2 == STATE_PAGE_ERASED)
				{
					success = VirtualEEPROMTransferPage(eepromDevice);
					if(success != RET_OK)
					{
						return success;
					}
				}
				else
				{

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
uint16_t VirtualEEPROMReadVariable(tFlash areaNumber, uint16_t virtualOffset, uint32_t* Data, uint32_t offsetStartAddr )
{
	//tFlash areaNumber;
	uint64_t OffsetValue = 0;
	uint16_t ReadStatus 	= 1;

	uint32_t Offset	= 0;
	uint32_t crc = 0;
	uint16_t startOffset	= 0;

	/* Check first is value is stored in Cache */
	if ( findInCache(virtualOffset, Data) != RET_OK )
	{
		/* Start to search variable from last offset written on EEPROM */
		Offset = offsetStartAddr - EE_ELEMENT_SIZE;

		while (Offset >= INITIAL_DATA_AREA)
		{
			/* Get the current offset content to be compared with virtual offset */
			FlashReadData( areaNumber, &OffsetValue, Offset);

			/* Compare the read offset with the virtual offset */
			if (EE_VIRTUALADDRESS_VALUE(OffsetValue) == virtualOffset)
			{
				/* Calculate crc of variable data and virtual address */
				crc = CalculateCrc(EE_DATA_VALUE(OffsetValue), EE_VIRTUALADDRESS_VALUE(OffsetValue));

				/* if crc verification pass, data is correct and is returned.
				 if crc verification fails, data is corrupted and has to be skip */
				if (crc == EE_CRC_VALUE(OffsetValue))
				{
					/* Get content of variable value */
					*Data = EE_DATA_VALUE(OffsetValue);
					/* In case variable value is read, reset ReadStatus flag */
					ReadStatus = 0;
					/* Add to cache this pair address-value */
					addToCache(virtualOffset, *Data);
				}
				break;
			}
			else
			{
				/* Next address location */
				Offset -= EE_ELEMENT_SIZE;
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
	SetPageState(EEPROMDeviceMap[eepromDevice].flashArea1, STATE_PAGE_ERASING);
	FlashStatus = FlashErase( EEPROMDeviceMap[eepromDevice].flashArea1, FLASH_ALL_PAGES);
	if(FlashStatus == RET_OK)
	{
		FlashStatus = SetPageState(EEPROMDeviceMap[eepromDevice].flashArea1, STATE_PAGE_ACTIVE);
		virtualEEPROMRegister.ptrAddrOffset = INITIAL_DATA_AREA;
		virtualEEPROMRegister.actualPage =  EEPROMDeviceMap[eepromDevice].flashArea1;
	}

	/* Erase FlashAreaStatus2 */
	if(FlashStatus == RET_OK)
	{
		if(GetPageState(EEPROMDeviceMap[eepromDevice].flashArea2) != STATE_PAGE_ERASED)
		{
			SetPageState(EEPROMDeviceMap[eepromDevice].flashArea2, STATE_PAGE_ERASING);
			FlashStatus = FlashErase( EEPROMDeviceMap[eepromDevice].flashArea2, FLASH_ALL_PAGES);
		}
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
uint16_t VirtualEEPROMVerifyPageFullWriteVariable(tFlash areaNumber, uint16_t VirtOffset, uint32_t Data)
{
	eError success 	= RET_OK;
	uint64_t dataWrite = 0;
	uint32_t crc = 0U;

	/* Check if there is no valid page */
	if (areaNumber == NO_VALID_AREA)
	{
		return  NO_VALID_AREA;
	}

	/* Force crc to 0 in case of Data/VirtAddress are 0*/
	if ((Data == 0U) && (VirtOffset == 0U))
	{
		crc = 0U;
	}
	else
	{
		/* Calculate crc of variable data and virtual address */
		crc = CalculateCrc(Data, VirtOffset);
	}
	/* Set data to write from virtual offset variable value and crc*/
	dataWrite = EE_ELEMENT_VALUE(VirtOffset, Data, crc);
	success = FlashProgramData(areaNumber, dataWrite, virtualEEPROMRegister.ptrAddrOffset);
	if (success == RET_OK)
	{
		if ( (virtualEEPROMRegister.ptrAddrOffset + EE_ELEMENT_SIZE) <= virtualEEPROMRegister.maxOffset )
		{
			virtualEEPROMRegister.ptrAddrOffset += EE_ELEMENT_SIZE;
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
	EE_State_type FlashAreaStatus1, FlashAreaStatus2;

	FlashAreaStatus1 = GetPageState(EEPROMDeviceMap[eepromDevice].flashArea1);
	FlashAreaStatus2 = GetPageState(EEPROMDeviceMap[eepromDevice].flashArea2);

	if (FlashAreaStatus1 == STATE_PAGE_ACTIVE)
	{
		areaNumber = EEPROMDeviceMap[eepromDevice].flashArea1;           /* Area1 valid */
	}
	else if (FlashAreaStatus2 == STATE_PAGE_ACTIVE)
	{
		areaNumber = EEPROMDeviceMap[eepromDevice].flashArea2;           /* Area2 valid */
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
uint16_t VirtualEEPROMWriteVariable(tVirtualEEPROMDevice eepromDevice, uint16_t VirtOffset, uint32_t Data)
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
	EE_State_type FlashAreaStatus = 0;
	tFlash OldArea;

	if (virtualEEPROMRegister.actualPage == EEPROMDeviceMap[eepromDevice].flashArea2)       /* Area 2 valid */
	{
		FlashAreaStatus = GetPageState(EEPROMDeviceMap[eepromDevice].flashArea1);
		OldArea = EEPROMDeviceMap[eepromDevice].flashArea1;
	}
	else	/*Area 1 valid */
	{
		FlashAreaStatus = GetPageState(EEPROMDeviceMap[eepromDevice].flashArea2);
		OldArea = EEPROMDeviceMap[eepromDevice].flashArea2;
	}

	if ( FlashAreaStatus != STATE_PAGE_ERASED )
	{
		SetPageState(OldArea, STATE_PAGE_ERASING);
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

	uint16_t eepromTable, eepromVar, instance;
	uint32_t MaxOffset;
	uint16_t offsetVar;
	uint32_t EepromStatus 	= 0;
	uint32_t ReadStatus 	= 0;
	uint32_t DataVar		= 0;

	/* Get active Page for read operation */
	if (virtualEEPROMRegister.actualPage == EEPROMDeviceMap[eepromDevice].flashArea2)       /* Area2 valid */
	{
		NewArea = EEPROMDeviceMap[eepromDevice].flashArea1;
		OldArea = EEPROMDeviceMap[eepromDevice].flashArea2;
	}
	else if (virtualEEPROMRegister.actualPage == EEPROMDeviceMap[eepromDevice].flashArea1)  /* Area1 valid */
	{
		NewArea = EEPROMDeviceMap[eepromDevice].flashArea2;
		OldArea = EEPROMDeviceMap[eepromDevice].flashArea1;
	}
	else
	{
		return NO_VALID_AREA;       /* No valid Page */
	}

	/* Set pages new states */
	SetPageState(OldArea, STATE_PAGE_VALID);
	if(GetPageState(NewArea) != STATE_PAGE_ERASED)
	{
		SetPageState(NewArea, STATE_PAGE_ERASING);
		if(FlashErase(NewArea, FLASH_ALL_PAGES) != RET_OK)
		{
			return RET_FAIL;
		}
	}
	SetPageState(NewArea, STATE_PAGE_RECEIVE);

	/* Point to new Page */
	virtualEEPROMRegister.ptrAddrOffset = INITIAL_DATA_AREA;

	/* Transfer process: transfer variables from old to the new active page */
	for (eepromTable = 0; eepromTable < NUM_OF_TABLE_EEPROM; eepromTable++ )
	{
		for (eepromVar = 0; eepromVar < virtualEEPROMareaContext[eepromTable].areaSize; eepromVar++)
		{
			for (instance=0; instance < EEPROMGetNumOfInstances(eepromTable,eepromVar); instance++)
			{
				/* Read the other last variable updates */
				offsetVar = virtualEEPROMareaContext[eepromTable].areaEEPROM[eepromVar].virtualOffset + instance;
				if ( findInCache(offsetVar, &DataVar) != RET_OK )
				{
					ReadStatus = VirtualEEPROMReadVariable(OldArea, offsetVar, &DataVar, virtualEEPROMRegister.maxOffset);
				}
				else
				{
					ReadStatus = 0;
				}

				/* In case variable corresponding to the virtual address was found */
				if (ReadStatus != 0x1)
				{
					/* Transfer the variable to the new active page */
					EepromStatus = VirtualEEPROMVerifyPageFullWriteVariable(NewArea, offsetVar, DataVar);
					/* If program operation was failed, a Flash error code is returned */
					if (EepromStatus != RET_OK)
					{
						return EepromStatus;
					}
				}
			}
		}
	}

	/* Set pages new states */
	SetPageState(NewArea, STATE_PAGE_ACTIVE);
	SetPageState(OldArea, STATE_PAGE_ERASING);
	if(FlashErase(OldArea, FLASH_ALL_PAGES) != RET_OK)
	{
		return RET_FAIL;
	}

	/* Update ptrOffset HEADER */
	virtualEEPROMRegister.actualPage = NewArea;
	FlashGetSize( NewArea, &MaxOffset);
	virtualEEPROMRegister.maxOffset = MaxOffset;

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
	uint64_t OffsetValue;

	uint32_t diffOffset = 0;
	uint32_t MaxOffset;

	areaNumber = VirtualEEPROMFindValidArea(eepromDevice, READ_FROM_VALID_AREA);
	if(areaNumber == NO_VALID_AREA)
	{
		success = RET_FAIL;
		return success;
	}

	virtualEEPROMRegister.actualPage = areaNumber;
	FlashGetSize( areaNumber, &MaxOffset);
	virtualEEPROMRegister.maxOffset = MaxOffset;

	/* HEADER virtualEEPROM offset */
	virtualEEPROMRegister.ptrAddrOffset = virtualEEPROMRegister.maxOffset;

	while ( virtualEEPROMRegister.ptrAddrOffset > INITIAL_DATA_AREA)
	{
		success = FlashReadData( areaNumber, &OffsetValue, virtualEEPROMRegister.ptrAddrOffset - EE_ELEMENT_SIZE);
		if ( OffsetValue == EE_PAGESTAT_ERASED)
		{
			virtualEEPROMRegister.ptrAddrOffset -= EE_ELEMENT_SIZE;
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
eError findInCache( uint16_t virtualAddress, uint32_t *value )
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
eError updateCache( uint16_t virtualAddress, uint32_t value )
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
eError addToCache( uint16_t virtualAddress, uint32_t value )
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

/**************************************************************************//**
 * @brief  Set page state in page header
 * @param  flashArea to be set
 * @param  New state
 * @retval RET_OK or RET_FAIL
 ****************************************************************************/
static eError SetPageState(tFlash flashArea, EE_State_type State)
{
	switch(State)
	{
		case STATE_PAGE_RECEIVE:
			/* Set new Page status to STATE_PAGE_RECEIVE status */
			if (FlashProgramData(flashArea, EE_PAGESTAT_RECEIVE, (EE_ELEMENT_SIZE*0U)) != RET_OK)
			{
				return RET_FAIL;
			}
			break;
		case STATE_PAGE_ACTIVE:
			/* Set new Page status to STATE_PAGE_ACTIVE status */
			if (FlashProgramData(flashArea, EE_PAGESTAT_ACTIVE, (EE_ELEMENT_SIZE*1U)) != RET_OK)
			{
				return RET_FAIL;
			}
			break;
		case STATE_PAGE_VALID:
			/* Set new Page status to STATE_PAGE_VALID status */
			if (FlashProgramData(flashArea, EE_PAGESTAT_VALID, (EE_ELEMENT_SIZE*2U)) != RET_OK)
			{
				return RET_FAIL;
			}
			break;
		case STATE_PAGE_ERASING:
			/* Set new Page status to STATE_PAGE_ERASING status */
			if (FlashProgramData(flashArea, EE_PAGESTAT_ERASING, (EE_ELEMENT_SIZE*4U)) != RET_OK)
			{
				return RET_FAIL;
			}
			break;
		default:
			break;
	}

	/* Return last operation flash status */
	return RET_OK;
}


/**************************************************************************//**
 * @brief  Get page state in page header
 * @param  flashArea to be read
 * @retval Page status
 ****************************************************************************/
static EE_State_type GetPageState(tFlash flashArea)
{
	EE_ELEMENT_TYPE status1 = 0U, status2 = 0U, status3 = 0U, status4 = 0U;

	/* Get page state information from page header (3 first elements) */
	FlashReadData(flashArea, &status1, (uint32_t) (EE_ELEMENT_SIZE*0U));
	FlashReadData(flashArea, &status2, (uint32_t) (EE_ELEMENT_SIZE*1U));
	FlashReadData(flashArea, &status3, (uint32_t) (EE_ELEMENT_SIZE*2U));
	FlashReadData(flashArea, &status4, (uint32_t) (EE_ELEMENT_SIZE*3U));

	/* Return erasing status, if element4 is not EE_PAGESTAT_ERASED value */
	if (status4 != EE_PAGESTAT_ERASED)
	{
		return STATE_PAGE_ERASING;
	}

	/* Return valid status, if element3 is not EE_PAGESTAT_ERASED value */
	if (status3 != EE_PAGESTAT_ERASED)
	{
		return STATE_PAGE_VALID;
	}

	/* Return active status, if element2 is not EE_PAGESTAT_ERASED value */
	if (status2 != EE_PAGESTAT_ERASED)
	{
		return STATE_PAGE_ACTIVE;
	}

	/* Return receive status, if element1 is not EE_PAGESTAT_ERASED value */
	if (status1 != EE_PAGESTAT_ERASED)
	{
		return STATE_PAGE_RECEIVE;
	}

	/* Return erased status, if 4 first elements are EE_PAGESTAT_ERASED value */
	return STATE_PAGE_ERASED;
}


/**************************************************************************//**
 * @brief  Performs CRC calculation on Data and Virtual Address
 * @param  Data value of  the eeprom variable.
 * @param  VirtAddress address of the eeprom variable.
 * @retval 16-bit CRC value computed on Data and Virtual Address.
 ****************************************************************************/
uint16_t CalculateCrc(EE_DATA_TYPE Data, uint16_t VirtAddress)
{
  /* Reset CRC calculation unit */
  LL_CRC_ResetCRCCalculationUnit(CRC);

  /* Feed Data and Virtual Address */
  LL_CRC_FeedData32(CRC, Data);
  LL_CRC_FeedData16(CRC, VirtAddress);

  /* Return computed CRC value */
  return(LL_CRC_ReadData16(CRC));
}


/**************************************************************************//**
 * @brief  Configures CRC Instance
 * @note   This function is used to :
 *         -1- Enable peripheral clock for CRC.
 *         -2- Configure CRC functional parameters.
 ****************************************************************************/
void ConfigureCrc(void)
{
  /* (1) Enable peripheral clock for CRC */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);

  /* (2) Configure CRC functional parameters */

  /* Configure CRC calculation unit with user defined polynomial */
  LL_CRC_SetPolynomialCoef(CRC, CRC_POLYNOMIAL_VALUE);
  LL_CRC_SetPolynomialSize(CRC, CRC_POLYNOMIAL_LENGTH);

  /* Initialize default CRC initial value */
  /* Reset value is LL_CRC_DEFAULT_CRC_INITVALUE */
  /* LL_CRC_SetInitialData(CRC, LL_CRC_DEFAULT_CRC_INITVALUE);*/

  /* Set input data inversion mode : No inversion*/
  /* Reset value is LL_CRC_INDATA_REVERSE_NONE */
  /* LL_CRC_SetInputDataReverseMode(CRC, LL_CRC_INDATA_REVERSE_NONE); */

  /* Set output data inversion mode : No inversion */
  /* Reset value is LL_CRC_OUTDATA_REVERSE_NONE */
  /* LL_CRC_SetOutputDataReverseMode(CRC, LL_CRC_OUTDATA_REVERSE_NONE); */
}
#endif /* _VIRTUALEEPROM_C_ */

/**@}*/
/****************************************************************************
 *    End of file
 ****************************************************************************/
