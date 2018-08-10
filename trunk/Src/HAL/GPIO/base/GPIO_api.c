/***********************************************************************
 * @file	GPIO_api.c
 *
 * @brief	GPIO Hardware registers
 *
 * @author	Albert Serrallo
 *
 * @date	2016-03-11
 *
 **********************************************************************/
#ifndef	_GPIO_API_C_
#define	_GPIO_API_C_

/***********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "GPIO.h"


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
 * @brief	API for init Gpio.
 *
 **********************************************************************/
__weak eError initHregGPIO( void )
{
	eError result = RET_OK;

	result = GPIOInit();

	return result;
}

/***********************************************************************
 * @brief	API for start Gpio.
 *
 **********************************************************************/
__weak eError startHregGPIO( void )
{
	eError result = RET_OK;

	result = GPIOStart();

	return result;
}

/***********************************************************************
 * @brief	API for stop Gpio.
 *
 **********************************************************************/
__weak eError stopHregGPIO( void )
{
	eError result = RET_OK;

	result = GPIOStop();

	return result;
}

/***********************************************************************
 * @brief	API for sleep Gpio.
 *
 **********************************************************************/
__weak eError sleepHregGPIO( void )
{
	eError result = RET_OK;

	result = GPIOSleep();

	return result;
}

/***********************************************************************
 * @brief	API for wake Gpio.
 *
 **********************************************************************/
__weak eError wakeHregGPIO( void )
{
	eError result = RET_OK;

	result = GPIOWake();

	return result;
}

/***********************************************************************
 * @brief	On Read function for Gpio value.
 *
 **********************************************************************/
__weak eError onReadHregGPIOValue( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	result = GPIOReadPort((tGPIO)instance, (uint8_t*) param1);

	return result;
}

/***********************************************************************
 * @brief	On Write function for Gpio value.
 *
 **********************************************************************/
__weak eError onWriteHregGPIOValue( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

	result = GPIOWritePort( instance ,param1);

	return result;
}



#endif	/*_GPIO_API_C_*/


