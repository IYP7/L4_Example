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
#ifndef	_SWTIMER_API_C_
#define	_SWTIMER_API_C_

/***********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "SwTimer.h"


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
 * @brief	API for init Timer.
 *
 **********************************************************************/
__weak eError initHregSwTimer( void )
{
	eError result = RET_OK;

	result = SwTimerInit();

	return result;
}

/***********************************************************************
 * @brief	API for start Timer.
 *
 **********************************************************************/
__weak eError startHregSwTimer( void )
{
	eError result = RET_OK;

	result = SwTimerStart();

	return result;
}

/***********************************************************************
 * @brief	API for stop Timer.
 *
 **********************************************************************/
__weak eError stopHregSwTimer( void )
{
	eError result = RET_OK;

	result = SwTimerStop();

	return result;
}

/***********************************************************************
 * @brief	API for sleep Timer.
 *
 **********************************************************************/
__weak eError sleepHregSwTimer( void )
{
	eError result = RET_OK;

	result = SwTimerSleep();

	return result;
}

/***********************************************************************
 * @brief	API for wake Timer.
 *
 **********************************************************************/
__weak eError wakeHregSwTimer( void )
{
	eError result = RET_OK;

	result = SwTimerWake();

	return result;
}

/***********************************************************************
 * @brief	On Write function for Timer callbackFunction.
 *
 **********************************************************************/
__weak eError onWriteHregSwTimerCallbackFunction( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

	result = SwTimerSetCallbackFunction( instance, (void*)param1 );

	return result;
}

/***********************************************************************
 * @brief	On Read function for Timer getPending.
 *
 **********************************************************************/
__weak eError onReadHregSwTimerGetPending( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	*(uint32_t*)param1 = SwTimerGetPending( instance );

	return result;
}

/***********************************************************************
 * @brief	On Read function for Timer getState.
 *
 **********************************************************************/
__weak eError onReadHregSwTimerState( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	*(tSwTimerState*)param1 = SwTimerGetState( instance );

	return result;
}

/***********************************************************************
 * @brief	On Write function for Timer setState.
 *
 **********************************************************************/
__weak eError onWriteHregSwTimerState( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

	result = SwTimerSetState( instance, param1 );

	return result;
}

/***********************************************************************
 * @brief	On Read function for Timer getElapsed.
 *
 **********************************************************************/
__weak eError onReadHregSwTimerGetElapsed( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	*(uint32_t *)param1 = SwTimerGetElapsed( instance );

	return result;
}

/***********************************************************************
 * @brief	On Write function for Timer set.
 *
 **********************************************************************/
__weak eError onWriteHregSwTimerSet( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

	result = SwTimerSetTimeout( instance, param1 );

	return result;
}


#endif	/*_SWTIMER_API_C_*/




