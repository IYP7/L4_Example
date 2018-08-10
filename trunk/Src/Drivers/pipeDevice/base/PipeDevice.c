/*************************************************************************//**
 * @file PipeDevice.c
 * @date 19/02/2016
 * @brief Provides a reception buffer independently of protocol used.
 *		  Driver is also providing  functions to parse buffer searching
 *		  for a specific pattern.
 * @author Jordi Castells
 *
 * @addtogroup Drivers
 * @{
 * @addtogroup PipeDevice
 * @{
 *
 *
 ****************************************************************************/

#ifndef _PIPEDEVICE_C_
#define _PIPEDEVICE_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "pipeDevice.h"

/****************************************************************************
 *  DEFINES
 ****************************************************************************/

/****************************************************************************
 *  EXTERN VARIABLES
 ****************************************************************************/
extern const tPipeMap pipeMap[NUM_OF_PIPES];

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
tPipeElement pipes[NUM_OF_PIPES];

/****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/

/*************************************************************************//**
 * @brief   Starts all defined pipes
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError pipeDeviceInit( void )
{
	uint8_t i;

	for(i = 0; i < NUM_OF_PIPES; i++)
	{
		pipes[i].bufferRxPtr = 0;
		pipes[i].bufferStartingPosition = 0;
		pipes[i].bufferLength = pipeMap[i].bufferSize;
		pipes[i].buffer = pipeMap[i].bufferPtr;
	}

	return RET_OK;
}

/*************************************************************************//**
 * @brief  Stops pipeDevice.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
eError pipeDeviceStop( void )
{
	return RET_OK;
}

/*************************************************************************//**
 * @brief  Starts pipeDevice.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
eError pipeDeviceStart( void )
{
	return RET_OK;
}

/*************************************************************************//**
 * @brief  Sleeps pipeDevice.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
eError pipeDeviceSleep( void )
{
	return RET_OK;
}

/*************************************************************************//**
 * @brief  Wakes pipeDevice.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
eError pipeDeviceWake( void )
{
	return RET_OK;
}

/*************************************************************************//**
 * @brief  Recives one character to be saved on specific pipe.
 * @param	data to be saved.
 * @return  None.
 ****************************************************************************/
eError pipeDeviceByteReceived(ePipes id, uint8_t data)
{
    pipes[id].buffer[pipes[id].bufferRxPtr] = data;
    pipes[id].bufferRxPtr = (pipes[id].bufferRxPtr + 1) % (pipes[id].bufferLength);
    if ( pipes[id].bufferRxPtr == pipes[id].bufferStartingPosition )
    {
        return RET_BUFFER_FULL;
    }

    return RET_OK;
}

/*************************************************************************//**
 * @brief  Empties buffer of selected pipe.
 * @param	pipe to be emptied.
 * @return  None.
 ****************************************************************************/
void pipeDeviceResetPipe(ePipes id)
{
	pipes[id].bufferRxPtr = 0;
	pipes[id].bufferStartingPosition = 0;
}

/*************************************************************************//**
 * @brief  Parse one pipe buffer searching for a eol pattern sent as parameter.
 *
 * @param	pipe to be parsed.
 * @param	return buffer with result found.
 * @param   max length of string to be returned.
 * @param   pattern to usa as a end of line.
 * @param   size of the pattern.
 * @param   number of bytes returned on command buffer.
 *
 * @return  Success.
 *
 ****************************************************************************/
uint8_t pipeDeviceParseCommand(ePipes id, uint8_t *command, uint8_t length, int8_t *pattern, uint8_t patternSize,  uint8_t *commandSize)
{
    uint8_t success 		= FALSE;
    uint8_t bFound 			= FALSE;
    uint8_t patternIndex 	= 0;
    uint8_t bufferUsed 		= 0;
    uint8_t bufferPos 		= /*DmaChnGetDstPnt(DMA_CHANNEL1);*/pipes[id].bufferRxPtr;
    uint8_t bufferStart 	= pipes[id].bufferStartingPosition;

    *commandSize = 0;
    bufferUsed 	= (uint8_t)((pipes[id].bufferLength - bufferStart + bufferPos) % pipes[id].bufferLength);

    if ( patternSize > 0 && bufferUsed >= patternSize  )
    {
        while ( bufferUsed > 0 && bFound == FALSE )
        {
            if ( *commandSize < length )
            {
                command[(*commandSize)] = pipes[id].buffer[bufferStart];
                (*commandSize)++;
            }

            if ( pipes[id].buffer[bufferStart] == pattern[patternIndex] )
            {
                patternIndex++;
                bFound 			= (uint8_t)(patternIndex >= patternSize);
            }
            else
            {
                bufferStart 	= (uint8_t)((bufferStart + pipes[id].bufferLength - patternIndex) % pipes[id].bufferLength);
                bufferUsed		+= patternIndex;
                (*commandSize)	-= patternIndex;
                patternIndex 	= 0;
            }

            bufferStart		= (uint8_t)((bufferStart + 1) % pipes[id].bufferLength);
            bufferUsed--;
        }

        if ( bFound == TRUE )
        {
            pipes[id].bufferStartingPosition = bufferStart;
            success = TRUE;
        }
    }

    return success;
}

/*****************************************************************************
 * Description:
 * Params:
 * Return: Nothing
 * Note :
 *****************************************************************************/
void pipeDeviceExtractData(ePipes id, uint8_t *command, uint8_t length, uint8_t *commandSize)
{
	uint8_t bufferUsed 		= 0;
	uint8_t bufferPos 		= pipes[id].bufferRxPtr;
	uint8_t bufferStart 	= pipes[id].bufferStartingPosition;

	*commandSize 			= 0;
	bufferUsed 				= (uint8_t)((pipes[id].bufferLength - bufferStart + bufferPos) % pipes[id].bufferLength);

	while ( bufferUsed > 0)
	{
		if ( *commandSize < length )
		{
			command[(*commandSize)] = pipes[id].buffer[bufferStart];
			(*commandSize)++;
		}

		bufferStart		= (uint8_t)((bufferStart + 1) % pipes[id].bufferLength);
		bufferUsed--;
	}

	pipes[id].bufferStartingPosition = bufferStart;
}

/*****************************************************************************
 * Description:
 * Params:
 * Return: Nothing
 * Note :
 *****************************************************************************/
uint8_t pipeDeviceAdvancedParseCommand(ePipes id, uint8_t *command, uint8_t length, uint8_t *headerPattern, uint8_t headerPatternSize, uint8_t *tailPattern, uint8_t tailPatternSize, uint8_t *commandSize)
{
	uint8_t success 		= FALSE;
	uint8_t bFound 			= TRUE;
	uint8_t patternIndex 	= 0;
	uint8_t bufferUsed 		= 0;
	uint8_t bufferPos 		= pipes[id].bufferRxPtr;
	uint8_t bufferStart 	= pipes[id].bufferStartingPosition;
	uint8_t headerIndex		= 0;
	uint8_t tailIndex		= 0;

	*commandSize 			= 0;
	bufferUsed 				= (uint8_t)((pipes[id].bufferLength - bufferStart + bufferPos) % pipes[id].bufferLength);

	if( bFound == TRUE)
	{
		bFound 				= FALSE;
		headerIndex			= bufferStart;

		if( headerPatternSize == 0)
		{
			bFound			= TRUE;
		}
		else if( bufferUsed >= headerPatternSize )
		{
			patternIndex	= 0;

			while( (bufferUsed > 0) && (bFound == FALSE) )
			{
				if( pipes[id].buffer[bufferStart] == headerPattern[patternIndex] )
				{
					patternIndex++;
					bFound 			= (uint8_t)(patternIndex >= headerPatternSize);
				}
				else
				{
					headerIndex		= (uint8_t)((bufferStart + 1) % pipes[id].bufferLength);
					bufferStart 	= (uint8_t)((bufferStart + pipes[id].bufferLength - patternIndex) % pipes[id].bufferLength);
					bufferUsed		+= patternIndex;
					patternIndex 	= 0;
				}

				bufferStart		= (uint8_t)((bufferStart + 1) % pipes[id].bufferLength);
				bufferUsed--;
			}
		}
	}

	if( bFound == TRUE)
	{
		bFound 				= FALSE;
		tailIndex			= bufferStart;

		if( tailPatternSize == 0)
		{
			bFound 			= TRUE;
		}
		else if( bufferUsed >= tailPatternSize )
		{
			patternIndex	= 0;

			while ( (bufferUsed > 0) && (bFound == FALSE) )
			{
				if( pipes[id].buffer[bufferStart] == tailPattern[patternIndex] )
				{
					tailIndex		= (uint8_t)((bufferStart + 1) % pipes[id].bufferLength);
					patternIndex++;
					bFound 			= (uint8_t)(patternIndex >= tailPatternSize);
				}
				else
				{
					bufferStart 	= (uint8_t)((bufferStart + pipes[id].bufferLength - patternIndex) % pipes[id].bufferLength);
					bufferUsed		+= patternIndex;
					patternIndex 	= 0;
				}

				bufferStart		= (uint8_t)((bufferStart + 1) % pipes[id].bufferLength);
				bufferUsed--;
			}
		}
	}

	if(bFound == TRUE)
	{
		bufferStart = headerIndex;

		while( bufferStart != tailIndex )
		{
			if ( *commandSize < length )
			{
				command[(*commandSize)] = pipes[id].buffer[bufferStart];
				(*commandSize)++;
			}
			bufferStart		= (uint8_t)((bufferStart + 1) % pipes[id].bufferLength);
		}
		pipes[id].bufferStartingPosition = bufferStart;

		success	= TRUE;
	}

	return success;
}


/**Helper functions to access each pipe */
#ifdef IS_PIPE1
inline eError pipeDevice1ByteReceived(uint8_t data)
{
	return pipeDeviceByteReceived(PIPE_1, data);
}
#endif

#ifdef IS_PIPE2
inline eError pipeDevice2ByteReceived(uint8_t data)
{
	return pipeDeviceByteReceived(PIPE_2, data);
}
#endif

#ifdef IS_PIPE3
inline eError pipeDevice3ByteReceived(uint8_t data)
{
	return pipeDeviceByteReceived(PIPE_3, data);
}
#endif

#ifdef IS_PIPE4
inline eError pipeDevice4ByteReceived(uint8_t data)
{
	return pipeDeviceByteReceived(PIPE_4, data);
}
#endif

#ifdef IS_PIPE5
inline eError pipeDevice5ByteReceived(uint8_t data)
{
	return pipeDeviceByteReceived(PIPE_5, data);
}
#endif

#ifdef IS_PIPE6
inline eError pipeDevice6ByteReceived(uint8_t data)
{
	return pipeDeviceByteReceived(PIPE_6, data);
}
#endif

#endif // _PIPEDEVICE_C_

/****************************************************************************
 *    End of file
 *    }@}@
 ***************************************************************************/
