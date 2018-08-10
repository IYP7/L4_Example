/***********************************************************************
 * @file	interrupts_api.c
 *
 * @brief	Interrupts Hardware registers
 *
 * @author	Albert Serrallo
 *
 * @date	2016-03-11
 *
 **********************************************************************/
#ifndef	_INTERRUPTS_API_C_
#define	_INTERRUPTS_API_C_

/***********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "interrupts.h"


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
 * @brief	API for init Interruptions.
 *
 **********************************************************************/
__weak eError initHregInterrupts( void )
{
	eError result = RET_OK;

	interruptsInit();

	return result;
}

/***********************************************************************
 * @brief	API for start Interruptions.
 *
 **********************************************************************/
__weak eError startHregInterrupts( void )
{
	eError result = RET_OK;

	interruptsStart();

	return result;
}

/***********************************************************************
 * @brief	API for stop Interruptions.
 *
 **********************************************************************/
__weak eError stopHregInterrupts( void )
{
	eError result = RET_OK;

	interruptsStop();

	return result;
}

/***********************************************************************
 * @brief	API for sleep Interruptions.
 *
 **********************************************************************/
__weak eError sleepHregInterrupts( void )
{
	eError result = RET_OK;

	interruptsSleep();

	return result;
}

/***********************************************************************
 * @brief	API for wake Interruptions.
 *
 **********************************************************************/
__weak eError wakeHregInterrupts( void )
{
	eError result = RET_OK;

	interruptsWake();

	return result;
}

/***********************************************************************
 * @brief	On Read function for Interruptions enable.
 *
 **********************************************************************/
__weak eError onReadHregInterruptsState( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	*(uint8_t*)param1 = interruptsGetState(instance);

	return result;
}

/***********************************************************************
 * @brief	On Write function for Interruptions enable.
 *
 **********************************************************************/
__weak eError onWriteHregInterruptsState( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

	result = interruptsSetState(instance,param1);

	return result;
}





#endif	/*_NTERRUPTS_API_C_*/
