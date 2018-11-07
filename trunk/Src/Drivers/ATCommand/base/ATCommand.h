/*************************************************************************//**
 * @file ATCommand.h
 *
 * @brief Module for manage communication by AT commands
 *
 * @author Gabriel Perez
 *
 * @date 26/10/2018
 *
 ****************************************************************************/

#ifndef _ATCOMMAND_H
#define _ATCOMMAND_H

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDE
 ***************************************************************************/
#include "AtCommand_map.h"
#include "PipeDevice.h"
#include "uart.h"

/****************************************************************************
 *  DEFINES
 ****************************************************************************/
#define AT_CMD_BUFFER_SIZE 64

/****************************************************************************
 *  TYPES DEFINITION
 ****************************************************************************/
typedef enum
{
	AT_RESP_OK,
	AT_RESP_ERROR,
	AT_RESP_TIMEOUT,
	AT_RESP_URC
} tAtResponseID;

typedef struct sAtClient
{
	tBool 		isURCSubscribed;
	//uint8_t		*AtRxBuffer;
	ePipes		pipe;
	void 		(*callback)(tAtResponseID eventID, uint8_t *buffer, uint16_t length);
	tUart		UARTInstance;
} tAtClientMap;

typedef struct sAtRequest
{
	uint8_t		errorRetries;
	uint8_t		timeoutRetries;
	uint16_t	timeout;
	uint16_t	delay;
	uint8_t		*command;
} tAtRequest;

/****************************************************************************
 *    VARIABLES
 ****************************************************************************/

 /****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/
eError AtCommandStart(void);
eError ATCommandSetRequest(tAtClients ATClient, tAtRequest ATRequest);

#endif // _ATCOMMAND_H

/****************************************************************************
 *    End of file
 ***************************************************************************/
