/***********************************************************************
 * @file	Leds_api.c
 *
 * @brief	Leds Hardware registers
 *
 * @author	Albert Serrallo
 *
 * @date	17/05/2016
 *
 **********************************************************************/
#ifndef	_LEDS_API_C_
#define	_LEDS_API_C_

/***********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "Leds.h"


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
 * @brief	API for init Leds.
 *
 **********************************************************************/
__weak eError initHregLeds( void )
{
	eError result = RET_OK;

	result = LedsInit();

	return result;
}

/***********************************************************************
 * @brief	API for start Leds
 *
 **********************************************************************/
__weak eError startHregLeds( void )
{
	eError result = RET_OK;

	result = LedsStart();

	return result;
}

/***********************************************************************
 * @brief	API for stop Leds.
 *
 **********************************************************************/
__weak eError stopHregLeds( void )
{
	eError result = RET_OK;

	result = LedsStop();

	return result;
}

/***********************************************************************
 * @brief	API for sleep Leds
 *
 **********************************************************************/
__weak eError sleepHregLeds( void )
{
	eError result = RET_OK;

	result = LedsSleep();

	return result;
}

/***********************************************************************
 * @brief	API for wake Leds
 *
 **********************************************************************/
__weak eError wakeHregLeds( void )
{
	eError result = RET_OK;

	result = LedsWake();

	return result;
}



/***********************************************************************
 * @brief	On Write function Leds Effect
 *
 **********************************************************************/
__weak eError onWriteHregLedsEffect( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

	result = LedsSetModeEffect((tLeds)instance, (tLedsEffect)param1);

	return result;
}

/***********************************************************************
 * @brief	On read function Leds Effect
 *
 **********************************************************************/
__weak eError onReadHregLedsEffect( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	result = LedsGetModeEffect((tLeds)instance, (tLedsEffect* )param1);

	return result;
}

/***********************************************************************
 * @brief	On Write function Led State
 *
 **********************************************************************/
__weak eError onWriteHregLedsState( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

	result = LedsSetModeState((tLeds)instance, (tLedState)param1);

	return result;
}

/***********************************************************************
 * @brief	On Read function for Leds ON OFF
 *
 **********************************************************************/
__weak eError onReadHregLedsState( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	result = LedsGetModeState((tLeds)instance, (tLedState* )param1);

	return result;
}


#endif	/*_LEDS_API_C_*/


