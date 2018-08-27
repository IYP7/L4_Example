/***********************************************************************
 * @file	sd_api.c
 *
 * @brief	SD Hardware registers
 *
 * @author	Alex Colomer
 *
 * @date	2018-02-26
 *
 **********************************************************************/
#ifndef	_SD_API_C_
#define	_SD_API_C_

/***********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "sd.h"


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
 * @brief	API for init Sd.
 *
 **********************************************************************/
__weak eError initHregSD( void )
{
	eError result = RET_OK;

	result = sdInit();

	return result;
}

/***********************************************************************
 * @brief	API for start Sd.
 *
 **********************************************************************/
__weak eError startHregSD( void )
{
	eError result = RET_OK;

	result = sdStart();

	return result;
}

/***********************************************************************
 * @brief	API for stop Sd.
 *
 **********************************************************************/
__weak eError stopHregSD( void )
{
	eError result = RET_OK;

	result = sdStop();

	return result;
}

/***********************************************************************
 * @brief	API for sleep Sd.
 *
 **********************************************************************/
__weak eError sleepHregSD( void )
{
	eError result = RET_OK;

	result = sdSleep();

	return result;
}

/***********************************************************************
 * @brief	API for wake Sd.
 *
 **********************************************************************/
__weak eError wakeHregSD( void )
{
	eError result = RET_OK;

	result = sdWake();

	return result;
}

/***********************************************************************
 * @brief	On Read function for SD bufferSize
 *
 **********************************************************************/
__weak eError onReadHregSDBufferSize( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

    *(uint32_t*)param1 = sdDriverGetBufferSize();

	return result;
}

/***********************************************************************
 * @brief	On Write function for SD bufferSize
 *
 **********************************************************************/
__weak eError onWriteHregSDBufferSize( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

    result = sdDriverSetBufferSize(param1);

	return result;
}

/***********************************************************************
 * @brief	On Read function for SD data.
 *
 **********************************************************************/
__weak eError onReadHregSDData( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;
	uint16_t numberOfBlocks;
	uint32_t blockSize, sector;

	READ_HREG(instance, HREG_SD_SECTOR, &sector);
	READ_HREG(instance, HREG_SD_BUFFER_SIZE, &blockSize);
	READ_HREG(instance, HREG_SD_NUMBER_OF_BLOCKS, &numberOfBlocks);
	result = sdDriverRead((tSd)instance, (uint32_t*) param1, sector*blockSize, blockSize, numberOfBlocks);

	return result;
}

/***********************************************************************
 * @brief	On Write function for SD data.
 *
 **********************************************************************/
__weak eError onWriteHregSDData( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result;
	uint16_t numberOfBlocks;
	uint32_t blockSize, sector;

	READ_HREG(instance, HREG_SD_SECTOR, &sector);
	READ_HREG(instance, HREG_SD_BUFFER_SIZE, &blockSize);
	READ_HREG(instance, HREG_SD_NUMBER_OF_BLOCKS, &numberOfBlocks);
    result = sdDriverWrite((tSd)instance, (uint32_t*) param1, sector*blockSize, blockSize, numberOfBlocks);

	return result;
}

/***********************************************************************
 * @brief	On Read function for SD status.
 *
 **********************************************************************/
__weak eError onReadHregSDStatus( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result;

    result = sdDriverGetStatus((tSd)instance);

	return result;
}


/***********************************************************************
 * @brief	On Read function for SD address sector
 *
 **********************************************************************/
__weak eError onReadHregSDSector( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

    *(uint32_t*)param1 = sdDriverGetSector();

	return result;
}


/***********************************************************************
 * @brief	On Write function for SD address sector.
 *
 **********************************************************************/
__weak eError onWriteHregSDSector( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = RET_OK;

    result = sdDriverSetSector(param1);

	return result;
}



/***********************************************************************
 * @brief	On Read function for SD number of blocks.
 *
 **********************************************************************/
__weak eError onReadHregSDNumberOfBlocks( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

    *(uint16_t*)param1 = sdDriverGetNumberOfBlocks();

	return result;
}


/***********************************************************************
 * @brief	On Write function for SD High Bits Address.
 *
 **********************************************************************/
__weak eError onWriteHregSDNumberOfBlocks( uint8_t instance, const HREG* hregPtr, uint32_t param1 )
{
	eError result = sdDriverSetNumberOfBlocks(param1);

	return result;
}




#endif	/*_SD_API_C_*/


