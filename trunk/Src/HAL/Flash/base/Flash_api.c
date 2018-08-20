/***********************************************************************
 * @file	FLASH_api.c
 *
 * @brief	FLASH Hardware registers
 *
 * @author	FV4MMA
 *
 * @date	2016-03-29
 *
 **********************************************************************/
#ifndef	_FLASH_API_C_
#define	_FLASH_API_C_

/***********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "Flash.h"


/***********************************************************************
 * PUBLIC ARCHITECTURE INCLUDES
 **********************************************************************/
#include "Hreg.h"
#include "Hreg_api.h"
#include "Hreg_dat.h"
/***********************************************************************
 * PRIVATE DEFINES
 **********************************************************************/

/***********************************************************************
 * PRIVATE TYPES
 **********************************************************************/

/***********************************************************************
 * PRIVATE VARIABLES DEFINITIONS
 **********************************************************************/

/***********************************************************************
 * PRIVATE FUNCTIONS DECLARATIONS
 **********************************************************************/

/***********************************************************************
 * PRIVATE FUNCTIONS DEFINITIONS
 **********************************************************************/

/***********************************************************************
 * PUBLIC VARIABLES DEFINITIONS
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUNCTIONS DEFINITIONS
 **********************************************************************/


/***********************************************************************
 * @brief	API for init Flash.
 *
 **********************************************************************/
__weak eError initHregFlash( void )
{
	eError result = RET_OK;

	result = FlashInit();

	return result;
}

/***********************************************************************
 * @brief	API for start Flash.
 *
 **********************************************************************/
__weak eError startHregFlash( void )
{
	eError result = RET_OK;

	result = FlashStart();

	return result;
}

/***********************************************************************
 * @brief	API for stop Flash.
 *
 **********************************************************************/
__weak eError stopHregFlash( void )
{
	eError result = RET_OK;

	result = FlashStop();

	return result;
}

/***********************************************************************
 * @brief	API for sleep Flash.
 *
 **********************************************************************/
__weak eError sleepHregFlash( void )
{
	eError result = RET_OK;

	result = FlashSleep();

	return result;
}

/***********************************************************************
 * @brief	API for wake Flash.
 *
 **********************************************************************/
__weak eError wakeHregFlash( void )
{
	eError result = RET_OK;

	result = FlashWake();

	return result;
}


/***********************************************************************
 * @brief	On Read function for Flash size.
 *
 **********************************************************************/
__weak eError onReadHregFlashSize( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	FlashGetSize( (tFlash)instance, (uint32_t*) param1);

	return result;
}

/***********************************************************************
 * @brief	On Read function for Flash data.
 *
 **********************************************************************/
__weak eError onReadHregFlashData( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	uint32_t offset;
	READ_HREG( instance, HREG_FLASH_OFFSET, &offset);
	result = FlashReadData((tFlash)instance, (uint32_t*) param1, offset);

	return result;
}

/***********************************************************************
 * @brief	On Write function for Flash data.
 *
 **********************************************************************/
__weak eError onWriteHregFlashData( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

	uint32_t offset;
	READ_HREG( instance, HREG_FLASH_OFFSET, &offset);
	result = FlashProgramData((tFlash)instance, (uint64_t) param1, offset);

	return result;
}

/***********************************************************************
 * @brief	On Write function for Flash offset.
 *
 **********************************************************************/
__weak eError onWriteHregFlashOffset( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

	return result;
}

#endif	/*_EEPROM_API_C_*/




