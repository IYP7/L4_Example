/***********************************************************************
 * @file	EEPROM_api.c
 *
 * @brief	EEPROM Hardware registers
 *
 * @author	Albert Serrallo
 *
 * @date	2016-03-11
 *
 **********************************************************************/
#ifndef	_EEPROM_API_C_
#define	_EEPROM_API_C_

/***********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "VirtualEEPROM.h"


/***********************************************************************
 * PUBLIC ARCHITECTURE INCLUDES
 **********************************************************************/
#include "Hreg.h"
#include "Hreg_api.h"

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
 * @brief	API for init EEPROM.
 *
 **********************************************************************/
__weak eError initHregVirtualEEPROM( void )
{
	eError result = RET_OK;

	VirtualEEPROMinit();

	return result;
}

/***********************************************************************
 * @brief	API for start EEPROM.
 *
 **********************************************************************/
__weak eError startHregVirtualEEPROM( void )
{
	eError result = RET_OK;

	VirtualEEPROMStart();

	return result;
}

/***********************************************************************
 * @brief	API for stop EEPROM.
 *
 **********************************************************************/
__weak eError stopHregVirtualEEPROM( void )
{
	eError result = RET_OK;

	VirtualEEPROMStop();

	return result;
}

/***********************************************************************
 * @brief	API for sleep EEPROM.
 *
 **********************************************************************/
__weak eError sleepHregVirtualEEPROM( void )
{
	eError result = RET_OK;

	VirtualEEPROMSleep();

	return result;
}

/***********************************************************************
 * @brief	API for wake EEPROM.
 *
 **********************************************************************/
__weak eError wakeHregVirtualEEPROM( void )
{
	eError result = RET_OK;

	VirtualEEPROMWake();

	return result;
}

/***********************************************************************
 * @brief	On Read function for EEPROM register.
 *
 **********************************************************************/
__weak eError onReadHregVirtualEEPROMData( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

//	tEEPROMMap VirtualEepromReg;
//	uint8_t instanceReg;
//
//	READ_HREG( instance, HREG_VIRTUALEEPROM_INSTANCE_REG, &instanceReg);
//	READ_HREG( instance, HREG_VIRTUALEEPROM_REGISTER, &VirtualEepromReg);
//	result = VirtualEEPROMReadRegister((eEEVDeviceMap)instance, (tEEPROMMap)VirtualEepromReg, (uint8_t)instanceReg, (uint32_t*) param1);


	*(uint32_t*)param1 = 0;
	result = RET_FAIL;

	return result;
}


/***********************************************************************
 * @brief	On Write function for EEPROM register.
 *
 **********************************************************************/
__weak eError onWriteHregVirtualEEPROMData( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

//	tEEPROMMap VirtualEepromReg;
//	uint8_t instanceReg;
//	READ_HREG( instance, HREG_VIRTUALEEPROM_INSTANCE_REG, &instanceReg);
//	READ_HREG( instance, HREG_VIRTUALEEPROM_REGISTER, &VirtualEepromReg);
//	result = VirtualEEPROMWriteRegister((tEEPROMInstance)instance, (tEEPROMMap)VirtualEepromReg, (uint8_t)instanceReg, (uint32_t) param1);

	result = RET_FAIL;

	return result;
}


#endif	/*_EEPROM_API_C_*/




