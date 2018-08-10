/***********************************************************************
 * @file	spi_api.c
 *
 * @brief	SPI Hardware registers
 *
 * @author	Albert Serrallo
 *
 * @date	2016-03-11
 *
 **********************************************************************/
#ifndef	_SPI_API_C_
#define	_SPI_API_C_

/***********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "spi.h"


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
 * @brief	API for init Spi.
 *
 **********************************************************************/
__weak eError initHregSpi( void )
{
	eError result = RET_OK;

    spiInit();

	return result;
}

/***********************************************************************
 * @brief	API for start Spi.
 *
 **********************************************************************/
__weak eError startHregSpi( void )
{
	eError result = RET_OK;

    spiStart();

	return result;
}

/***********************************************************************
 * @brief	API for stop Spi.
 *
 **********************************************************************/
__weak eError stopHregSpi( void )
{
	eError result = RET_OK;

    spiStop();

	return result;
}

/***********************************************************************
 * @brief	API for sleep Spi.
 *
 **********************************************************************/
__weak eError sleepHregSpi( void )
{
	eError result = RET_OK;

    spiSleep();

	return result;
}

/***********************************************************************
 * @brief	API for wake Spi.
 *
 **********************************************************************/
__weak eError wakeHregSpi( void )
{
	eError result = RET_OK;

    spiWake();

	return result;
}

/***********************************************************************
 * @brief	On Read function for Spi bufferSize.
 *
 **********************************************************************/
__weak eError onReadHregSpiBufferSize( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

    *(uint32_t*)param1 = spiDriverGetBufferSize((tSpi)instance);

	return result;
}

/***********************************************************************
 * @brief	On Write function for Spi bufferSize.
 *
 **********************************************************************/
__weak eError onWriteHregSpiBufferSize( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

    result = spiDriverSetBufferSize((tSpi)instance, param1);

	return result;
}

/***********************************************************************
 * @brief	On Read function for Spi data.
 *
 **********************************************************************/
__weak eError onReadHregSpiData( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

    result = spiDriverRead((tSpi)instance, (uint8_t*) param1);

	return result;
}

/***********************************************************************
 * @brief	On Write function for Spi data.
 *
 **********************************************************************/
__weak eError onWriteHregSpiData( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

    result = spiDriverWrite((tSpi)instance, (uint8_t*) param1);

	return result;
}

/***********************************************************************
 * @brief	On Read function for Spi status.
 *
 **********************************************************************/
__weak eError onReadHregSpiStatus( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

    *(uint8_t*)param1 = spiDriverStatus((tSpi)instance);

	return result;
}


#endif	/*_SPI_API_C_*/


