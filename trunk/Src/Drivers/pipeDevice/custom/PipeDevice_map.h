/*************************************************************************//**
 * @file PipeDevice_Map.h
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

#ifndef _PIPEDEVICE_MAP_H_
#define _PIPEDEVICE_MAP_H_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  DEFINES
 ****************************************************************************/
#define IS_PIPE1
#define IS_PIPE2
//#define IS_PIPE3
//#define IS_PIPE4
//#define IS_PIPE5
//#define IS_PIPE6


#ifdef IS_PIPE1
#define PIPE1_BUFFER_LENGTH 150
#endif

#ifdef IS_PIPE2
#define PIPE2_BUFFER_LENGTH 150
#endif

#ifdef IS_PIPE3
#define PIPE3_BUFFER_LENGTH 150
#endif

#ifdef IS_PIPE4
#define PIPE4_BUFFER_LENGTH 150
#endif

#ifdef IS_PIPE5
#define PIPE5_BUFFER_LENGTH 250
#endif

#ifdef IS_PIPE6
#define PIPE6_BUFFER_LENGTH 150
#endif

/****************************************************************************
 *  TYPES DEFINITION
 ****************************************************************************/
typedef enum
{
	INIT_PIPE = -1,

#ifdef IS_PIPE1
	PIPE_1,
#endif

#ifdef IS_PIPE2
	PIPE_2,
#endif

#ifdef IS_PIPE3
	PIPE_3,
#endif

#ifdef IS_PIPE4
	PIPE_4,
#endif

#ifdef IS_PIPE5
	PIPE_5,
#endif

#ifdef IS_PIPE6
	PIPE_6,
#endif

	NUM_OF_PIPES
} ePipes;

/****************************************************************************
 *    VARIABLES
 ****************************************************************************/

 /****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/

#endif // _PIPEDEVICE_MAP_H_

/****************************************************************************
 *    End of file
 ***************************************************************************/
