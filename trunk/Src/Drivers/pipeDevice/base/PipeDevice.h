/*************************************************************************//**
 * @file PipeDevice.h
 *
 * @brief Provides a reception buffer independently of protocol used.
 *		  Driver is also providing  functions to parse buffer searching
 *		  for a specific pattern.
 *
 * @author Rubén Rodríguez
 *
 * @date 23/01/2012
 *
 ****************************************************************************/

#ifndef _PIPEDEVICE_H_
#define _PIPEDEVICE_H_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"
#include "pipeDevice_map.h"

/****************************************************************************
 *  DEFINES
 ****************************************************************************/

/****************************************************************************
 *  TYPES DEFINITION
 ****************************************************************************/
typedef struct sPipeMap
{
    FPTR            cbFun;
    uint8_t*        bufferPtr;
    uint8_t         bufferSize;
} tPipeMap;

typedef struct
{
	uint8_t *buffer;
	uint8_t bufferStartingPosition;
	uint8_t bufferRxPtr;
	uint8_t bufferLength;
} tPipeElement;

/****************************************************************************
 *    VARIABLES
 ****************************************************************************/

 /****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/
eError pipeDeviceInit( void );
eError pipeDeviceStop( void );
eError pipeDeviceStart( void );
eError pipeDeviceSleep( void );
eError pipeDeviceWake( void );

void    pipeDeviceResetPipe(ePipes id);
void    pipeDeviceExtractData(ePipes id, uint8_t *command, uint8_t length, uint8_t *commandSize);

uint8_t pipeDeviceAdvancedParseCommand(ePipes id, uint8_t *command, uint8_t length,
                                   uint8_t *headerPattern, uint8_t headerPatternSize,
                                   uint8_t *tailPattern, uint8_t tailPatternSize,
                                   uint8_t *commandSize);

uint8_t pipeDeviceParseCommand(ePipes id, uint8_t *command, uint8_t length,
                               int8_t *pattern, uint8_t patternSize,
                               uint8_t *bufferUsed );

eError pipeDeviceByteReceived(ePipes id, uint8_t data);


/**Helper functions to access each pipe */
#ifdef IS_PIPE1
inline eError pipeDevice1ByteReceived(uint8_t data);
#endif

#ifdef IS_PIPE2
inline eError pipeDevice2ByteReceived(uint8_t data);
#endif

#ifdef IS_PIPE3
inline eError pipeDevice3ByteReceived(uint8_t data);
#endif

#ifdef IS_PIPE4
inline eError pipeDevice4ByteReceived(uint8_t data);
#endif

#ifdef IS_PIPE5
inline eError pipeDevice5ByteReceived(uint8_t data);
#endif

#ifdef IS_PIPE6
inline eError pipeDevice6ByteReceived(uint8_t data);
#endif


#endif // _PIPEDEVICE_H_

/****************************************************************************
 *    End of file
 ***************************************************************************/
