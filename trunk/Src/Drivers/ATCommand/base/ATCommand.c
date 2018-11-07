/*************************************************************************//**
 * @file ATCommand.c
 *
 * @brief Module for manage communication by AT commands
 *
 * @author Gabriel Perez
 *
 * @date 26/10/2018
 *
 ****************************************************************************/

#ifndef _ATCOMMAND_C_
#define _ATCOMMAND_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/
#include "statemachine.h"
#include "ATCommand_fsm.h"

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "ATCommand.h"
#include "string.h"

/****************************************************************************
 *  DEFINES
 ****************************************************************************/

/****************************************************************************
 *  EXTERN VARIABLES
 ****************************************************************************/
extern const tListFsmState fsmStateATCommand[NUM_OF_ATCOMMAND_FSM_EVENTS];
extern tFsmEvent fsmEventATCommand[NUM_OF_ATCOMMAND_FSM_EVENTS];

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
extern tAtClientMap ATClient[NUM_OF_AT_CLIENTS];
static uint8_t commandHeader[15];
static tBool isRequestInProgress = FALSE;
static tAtRequest ATActualRequest;
static tAtClients ATActualClient = NUM_OF_AT_CLIENTS;
uint16_t commandSize;
uint16_t respSize;
uint8_t actualCommand[AT_CMD_BUFFER_SIZE];
uint8_t AtRxBuffer[AT_CMD_BUFFER_SIZE];
uint8_t AtRxBufferBytes;
static uint8_t toRetries;

static tFsm fsmATCmd;
static uint8_t indexfsmATCmd;
/****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/
eError ATCommandParse(void)
{
	eError ret = RET_INVALID_COMMAND;
	uint8_t initOffset = 0;
	uint8_t headerSize = 0;
	uint8_t i = 0;

	if((ATActualRequest.command[0] == 'A') && (ATActualRequest.command[1] == 'T'))
	{
		memset(commandHeader, 0, 15);
		commandSize = strlen(ATActualRequest.command);
		if((commandSize+2) <= AT_CMD_BUFFER_SIZE)
		{
			for(i=2; i<commandSize; i++)
			{
				if((ATActualRequest.command[i]!='+') && (ATActualRequest.command[i]!='#') && (ATActualRequest.command[i]!=0)
					&& (ATActualRequest.command[i]!='=') && (ATActualRequest.command[i]!='?'))
				{
					headerSize++;
					if(!initOffset)
					{
						initOffset = i;
					}
				}
				else if(initOffset)
				{
					memcpy(commandHeader, &ATActualRequest.command[initOffset], headerSize);
					memcpy(actualCommand, ATActualRequest.command, commandSize);
					memcpy(&actualCommand[commandSize], "\r\n", 2);
					commandSize += 2;
					ret = RET_OK;
					break;
				}
			}
		}
	}

	return ret;
}

eError AtProcessResponse(tAtClients client)
{
	eError ret = RET_OK;
	uint8_t clientPipe = 0;

	if(ATClient[client].pipe == ATClient[ATActualClient].pipe)
	{
		if(isRequestInProgress)
		{
			ATClient[ATActualClient].callback(AT_RESP_OK, AtRxBuffer, AtRxBufferBytes);
		}
	}
	else
	{
		for(clientPipe=0; clientPipe<NUM_OF_AT_CLIENTS; clientPipe++)
		{
			if(ATClient[clientPipe].pipe == ATClient[client].pipe)
			{
				if(ATClient[clientPipe].isURCSubscribed)
				{
					ATClient[clientPipe].callback(AT_RESP_URC, AtRxBuffer, AtRxBufferBytes);
				}
			}
		}
	}

	announceFsmEvent(indexfsmATCmd, AT_RESPONSE_REC_EVENT);
	return ret;
}

/* GUARD FUNCTION */

tBool AtCheckMaxRetries(uint8_t val)
{

	return FALSE;
}

tBool AtCheckExpectedResponse(uint8_t val)
{

	return TRUE;
}

tBool AtCheckNoMaxRetries(uint8_t val)
{

	return TRUE;
}

tBool AtCheckNoExpectedResponse(uint8_t val)
{

	return FALSE;
}

/* ACTION FUNCTION */

eError AtSendCommand(uint8_t val)
{
	eError ret = RET_OK;

//	WRITE_HREG(ATClient[ATActualClient].UARTInstance, HREG_UART_BUFFER_SIZE, strlen((const char *)(ATActualRequest.command)));
//	WRITE_HREG(ATClient[ATActualClient].UARTInstance, HREG_UART_DATA, ATActualRequest.command);
	uartDriverSetBufferSize((tUart)ATClient[ATActualClient].UARTInstance, commandSize);
	uartDriverWrite((tUart)ATClient[ATActualClient].UARTInstance, (uint8_t*)actualCommand);

	return ret;
}

eError AtCheckResponse(uint8_t val)
{
	eError ret = RET_OK;
	uint8_t client = 0;

	for(client=0; client<NUM_OF_AT_CLIENTS; client++)
	{
		if(pipeDeviceParseCommand(ATClient[client].pipe, AtRxBuffer, AT_CMD_BUFFER_SIZE, "\r\n", 2, &AtRxBufferBytes))
		{
			ret = AtProcessResponse(client);
		}
		else if(isRequestInProgress && (ATActualClient==client))
		{
			/* Check timeout */
			if(ATActualRequest.timeout > 0)
			{
				toRetries++;
				if(toRetries >= ATActualRequest.timeoutRetries)
				{
					pipeDeviceExtractData(ATClient[client].pipe, AtRxBuffer, AT_CMD_BUFFER_SIZE, &AtRxBufferBytes);
					isRequestInProgress = FALSE;
					announceFsmEvent(indexfsmATCmd, AT_RESP_TIMEOUT_EVENT);
				}
			}
		}
	}

	return ret;
}

eError AtReportTimeout(uint8_t val)
{
	ATClient[ATActualClient].callback(AT_RESP_TIMEOUT, AtRxBuffer, AtRxBufferBytes);
	return RET_OK;
}

eError AtReportResponse(uint8_t val)
{
	ATActualClient = NUM_OF_AT_CLIENTS;
	isRequestInProgress = FALSE;
	AtRxBufferBytes = 0;
	return RET_OK;
}

eError AtPublishURC(uint8_t val)
{
	return RET_OK;
}

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/

/*************************************************************************//**
 * @brief   Initialialize AT client
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError AtCommandInit(void)
{
	eError ret = RET_OK;

	return ret;
}

/*************************************************************************//**
 * @brief   Start AT client
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError AtCommandStart(void)
{
	eError ret = RET_OK;

	/* Configure FSM */
	fsmATCmd.listState = fsmStateATCommand;
	fsmATCmd.event = fsmEventATCommand;
	fsmATCmd.numOfEvents = NUM_OF_ATCOMMAND_FSM_EVENTS;
	fsmATCmd.numOfStates = NUM_OF_ATCOMMAND_FSM_STATES;
	fsmATCmd.numFsm = 0;
	fsmATCmd.initState = AT_IDLE_STATE;
	fsmATCmd.name = (uint8_t *)"FSM AT_CMD";
	indexfsmATCmd = configureFsm(&fsmATCmd, FSM_ATCOMMAND_FSM_LOOP_TIME);
	setLogFsm(indexfsmATCmd, FALSE);

	/* Start FSM */
	resetFsm(indexfsmATCmd);

	return ret;
}

eError ATCommandSetRequest(tAtClients ATClient, tAtRequest ATRequest)
{
	eError ret = RET_BUSY;

	if(!isRequestInProgress)
	{
		isRequestInProgress = TRUE;
		ATActualRequest = ATRequest;
		ATActualClient = ATClient;

		ret = ATCommandParse();

		if(ret == RET_OK)
		{
			ret = announceFsmEvent(indexfsmATCmd, AT_COMMAND_REQ_EVENT);
		}
	}

	return ret;
}

#endif // _ATCOMMAND_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
