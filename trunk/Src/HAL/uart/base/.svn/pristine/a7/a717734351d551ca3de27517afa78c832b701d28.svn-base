/***********************************************************************
 * @file	uart_api.c
 *
 * @brief	UART Hardware registers
 *
 * @author	Albert Serrallo
 *
 * @date	2016-03-11
 *
 **********************************************************************/
#ifndef	_UART_API_C_
#define	_UART_API_C_

/***********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "uart.h"


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
 * @brief	API for init Uart.
 *
 **********************************************************************/
__weak eError initHregUart( void )
{
	eError result = RET_OK;

	uartInit();

	return result;
}

/***********************************************************************
 * @brief	API for start Uart.
 *
 **********************************************************************/
__weak eError startHregUart( void )
{
	eError result = RET_OK;

	uartStart();

	return result;
}

/***********************************************************************
 * @brief	API for stop Uart.
 *
 **********************************************************************/
__weak eError stopHregUart( void )
{
	eError result = RET_OK;

	uartStop();

	return result;
}

/***********************************************************************
 * @brief	API for sleep Uart.
 *
 **********************************************************************/
__weak eError sleepHregUart( void )
{
	eError result = RET_OK;

	uartSleep();

	return result;
}

/***********************************************************************
 * @brief	API for wake Uart.
 *
 **********************************************************************/
__weak eError wakeHregUart( void )
{
	eError result = RET_OK;

	//Add your code here.

	return result;
}

/***********************************************************************
 * @brief	On Read function for Uart bufferSize.
 *
 **********************************************************************/
__weak eError onReadHregUartBufferSize( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

    *(uint32_t*)param1 = uartDriverGetBufferSize((tUart)instance);

	return result;
}

/***********************************************************************
 * @brief	On Write function for Uart bufferSize.
 *
 **********************************************************************/
__weak eError onWriteHregUartBufferSize( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

    result = uartDriverSetBufferSize((tUart)instance, param1);

	return result;
}

/***********************************************************************
 * @brief	On Read function for Uart data.
 *
 **********************************************************************/
__weak eError onReadHregUartData( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

    result = uartDriverRead((tUart)instance, (uint8_t*) param1);

	return result;
}

/***********************************************************************
 * @brief	On Write function for Uart data.
 *
 **********************************************************************/
__weak eError onWriteHregUartData( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

    result = uartDriverWrite((tUart)instance, (uint8_t*) param1);

	return result;
}

/***********************************************************************
 * @brief	On Read function for Uart status.
 *
 **********************************************************************/
__weak eError onReadHregUartStatus( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

    *(uint8_t*)param1 = uartDriverStatus((tUart)instance);

	return result;
}


#endif	/*_UART_API_C_*/





