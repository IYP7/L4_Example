/*************************************************************************//**
 * @file PipeDevice_Map.c
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

#ifndef _PIPEDEVICE_MAP_C_
#define _PIPEDEVICE_MAP_C_

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
#include "pipeDevice_map.h"

/****************************************************************************
 *  DEFINES
 ****************************************************************************/

/****************************************************************************
 *  EXTERN VARIABLES
 ****************************************************************************/

/****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
#ifdef IS_PIPE1
uint8_t pipe1Buffer[PIPE1_BUFFER_LENGTH];
#endif

#ifdef IS_PIPE2
uint8_t pipe2Buffer[PIPE2_BUFFER_LENGTH];
#endif

#ifdef IS_PIPE3
uint8_t pipe3Buffer[PIPE3_BUFFER_LENGTH];
#endif

#ifdef IS_PIPE4
uint8_t pipe4Buffer[PIPE4_BUFFER_LENGTH];
#endif

#ifdef IS_PIPE5
uint8_t pipe5Buffer[PIPE5_BUFFER_LENGTH];
#endif

#ifdef IS_PIPE6
uint8_t pipe6Buffer[PIPE6_BUFFER_LENGTH];
#endif

const tPipeMap pipeMap[NUM_OF_PIPES] =
{
	#ifdef IS_PIPE1
	{
		pipeDevice1ByteReceived,
		pipe1Buffer,
		sizeof(pipe1Buffer)/sizeof(uint8_t),
	},
	#endif
	#ifdef IS_PIPE2
	{
		pipeDevice2ByteReceived,
		pipe2Buffer,
		sizeof(pipe2Buffer)/sizeof(uint8_t),
	},
	#endif
	#ifdef IS_PIPE3
	{
		pipeDevice3ByteReceived,
		pipe3Buffer,
		sizeof(pipe3Buffer)/sizeof(uint8_t),
	},
	#endif
	#ifdef IS_PIPE4
	{
		pipeDevice4ByteReceived,
		pipe4Buffer,
		sizeof(pipe4Buffer)/sizeof(uint8_t),
	},
	#endif
	#ifdef IS_PIPE5
	{
		pipeDevice5ByteReceived,
		pipe5Buffer,
		sizeof(pipe5Buffer)/sizeof(uint8_t),
	},
	#endif
	#ifdef IS_PIPE6
	{
		pipeDevice6ByteReceived,
		pipe6Buffer,
		sizeof(pipe6Buffer)/sizeof(uint8_t),
	},
	#endif
};

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/

#endif // _PIPEDEVICE_MAP_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
