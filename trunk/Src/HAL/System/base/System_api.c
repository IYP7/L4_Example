/***********************************************************************
 * @file	System_api.c
 *
 * @brief	System Hardware registers
 *
 * @author	Albert Serrallo
 *
 * @date	2016-03-11
 *
 **********************************************************************/
#ifndef	_SYSTEM_API_C_
#define	_SYSTEM_API_C_

/***********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "System.h"


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
 * @brief	API for init System.
 *
 **********************************************************************/
__weak eError initHregSystem( void )
{
	eError result = RET_OK;

	result = stm32F4Init();

	return result;
}



/***********************************************************************
 * @brief	API for start System.
 *
 **********************************************************************/
__weak eError startHregSystem( void )
{
	eError result = RET_OK;

	result = stm32F4Start();

	return result;
}

/***********************************************************************
 * @brief	API for stop System.
 *
 **********************************************************************/
__weak eError stopHregSystem( void )
{
	eError result = RET_OK;

	result = stm32F4Stop();

	return result;
}

/***********************************************************************
 * @brief	API for sleep System.
 *
 **********************************************************************/
__weak eError sleepHregSystem( void )
{
	eError result = RET_OK;

	result = stm32F4Sleep();

	return result;
}

/***********************************************************************
 * @brief	API for wake System.
 *
 **********************************************************************/
__weak eError wakeHregSystem( void )
{
	eError result = RET_OK;

	result =  stm32F4Wake();

	return result;
}

/***********************************************************************
 * @brief	On Write function for System powerMode.
 *
 **********************************************************************/
__weak eError onWriteHregSystemPowerMode( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

	result = stm32F4SetPowerMode(param1);

	return result;
}

/***********************************************************************
 * @brief	On Read function for System date.
 *
 **********************************************************************/
__weak eError onReadHregSystemDate( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;
#ifdef MON_RTC_ENABLED
	result = RTCGetDate(param1);
#endif
	return result;
}

/***********************************************************************
 * @brief	On Write function for System date.
 *
 **********************************************************************/
__weak eError onWriteHregSystemDate( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;
#ifdef MON_RTC_ENABLED
	result = RTCSetDate(param1);
#endif
	return result;
}

/***********************************************************************
 * @brief	On Read function for System time.
 *
 **********************************************************************/
__weak eError onReadHregSystemTime( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;
#ifdef MON_RTC_ENABLED
	result = RTCGetTime(param1);
#endif
	return result;
}

/***********************************************************************
 * @brief	On Write function for System time.
 *
 **********************************************************************/
__weak eError onWriteHregSystemTime( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;
#ifdef MON_RTC_ENABLED
	result = RTCSetTime(param1);
#endif
	return result;
}


#endif	/*_SYSTEM_API_C_*/


