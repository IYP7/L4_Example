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
#include "string.h"
/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/
#include "statemachine.h"
#include "hreg_dat.h"
#include "SwTimer_map.h"
/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "ATCommand.h"
#include "ATCommand_fsm.h"

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

tBool AtTimeoutCallback(tSwTimerIndex SwTimerIndex);
/****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/
eError ATCommandParse(void)
{
	eError ret = RET_INVALID_COMMAND;
	uint8_t initOffset = 0;
	uint8_t headerSize = 0;
	uint8_t i = 0;

	commandSize = strlen(ATActualRequest.command);

	if(ATActualRequest.mode == AT_REQ_CMD)
	{
		if((ATActualRequest.command[0] == 'A') && (ATActualRequest.command[1] == 'T'))
		{
			if((commandSize+2) <= AT_CMD_BUFFER_SIZE)
			{
				memset(commandHeader, 0, 15);
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
						break;
					}
				}
				memcpy(commandHeader, &ATActualRequest.command[initOffset], headerSize);
				ret = RET_OK;
			}
		}
	}
	else if(ATActualRequest.mode == AT_REQ_TEXT)
	{
		ret = RET_OK;
	}

	if(ret == RET_OK)
	{
		memcpy(actualCommand, ATActualRequest.command, commandSize);
		memcpy(&actualCommand[commandSize], "\r\n", 2);
		commandSize += 2;
	}

	return ret;
}

eError AtProcessResponse(tAtClients client, tAtResponseID event)
{
	eError ret = RET_OK;
	uint8_t clientPipe = 0;

	if(ATClient[client].pipe == ATClient[ATActualClient].pipe)
	{
		if(isRequestInProgress)
		{
			ATClient[ATActualClient].callback(event, AtRxBuffer, AtRxBufferBytes);
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
	tBool ret = FALSE;

	if(toRetries >= ATActualRequest.timeoutRetries)
	{
		toRetries = 0;
		ret = TRUE;
	}

	return ret;
}

tBool AtCheckNoMaxRetries(uint8_t val)
{
	tBool ret = FALSE;

	if(toRetries < ATActualRequest.timeoutRetries)
	{
		toRetries++;
		ret = TRUE;
	}

	return ret;
}

tBool AtCheckExpectedResponse(uint8_t val)
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

	/* Send command through UART */
	WRITE_HREG(ATClient[ATActualClient].UARTInstance, HREG_UART_BUFFER_SIZE, commandSize);
	WRITE_HREG(ATClient[ATActualClient].UARTInstance, HREG_UART_DATA, actualCommand);

	/* Start timeout timer */
	if(ATActualRequest.timeout > 0)
	{
		WRITE_HREG(SW_TIMER_AT, HREG_SWTIMER_SET, ATActualRequest.timeout);
		WRITE_HREG(SW_TIMER_AT, HREG_SWTIMER_STATE, SW_TIMER_STATE_STARTED);
	}
	return ret;
}

eError AtCheckResponse(uint8_t val)
{
	eError ret = RET_OK;
	uint8_t client = 0;

	for(client=0; client<NUM_OF_AT_CLIENTS; client++)
	{
		if(pipeDeviceParseCommand(ATClient[client].pipe, AtRxBuffer, AT_CMD_BUFFER_SIZE,(int8_t *) "\r\nOK\r\n", 6, &AtRxBufferBytes))
		{
			ret = AtProcessResponse(client, AT_RESP_OK);
			/* Stop timer */
			WRITE_HREG(SW_TIMER_AT, HREG_SWTIMER_STATE, SW_TIMER_STATE_CANCELLED);
		}
		else if(pipeDeviceParseCommand(ATClient[client].pipe, AtRxBuffer, AT_CMD_BUFFER_SIZE,(int8_t *) "\r\nERROR\r\n", 9, &AtRxBufferBytes))
		{
			ret = AtProcessResponse(client, AT_RESP_ERROR);
			/* Stop timer */
			WRITE_HREG(SW_TIMER_AT, HREG_SWTIMER_STATE, SW_TIMER_STATE_CANCELLED);
		}
		else if(pipeDeviceParseCommand(ATClient[client].pipe, AtRxBuffer, AT_CMD_BUFFER_SIZE,(int8_t *) "\r\n", 2, &AtRxBufferBytes))
		{
			ret = AtProcessResponse(client, AT_RESP_DATA);
			/* Stop timer */
			WRITE_HREG(SW_TIMER_AT, HREG_SWTIMER_STATE, SW_TIMER_STATE_CANCELLED);
		}
	}

	return ret;
}

eError AtReportTimeout(uint8_t val)
{
	isRequestInProgress = FALSE;
	pipeDeviceExtractData(ATClient[ATActualClient].pipe, AtRxBuffer, AT_CMD_BUFFER_SIZE, &AtRxBufferBytes);
	ATClient[ATActualClient].callback(AT_RESP_TIMEOUT, AtRxBuffer, AtRxBufferBytes);
	ATActualClient = NUM_OF_AT_CLIENTS;
	AtRxBufferBytes = 0;
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
	AtRxBufferBytes = 0;
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

	/* Configure SW timer */
	WRITE_HREG(SW_TIMER_AT, HREG_SWTIMER_CALLBACK_FUNCTION, AtTimeoutCallback);

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
		toRetries = 0;

		ret = ATCommandParse();

		if(ret == RET_OK)
		{
			ret = announceFsmEvent(indexfsmATCmd, AT_COMMAND_REQ_EVENT);
		}
	}

	return ret;
}

tBool AtTimeoutCallback(tSwTimerIndex SwTimerIndex)
{
	if(isRequestInProgress)
	{
		announceFsmEvent(indexfsmATCmd, AT_RESP_TIMEOUT_EVENT);
	}

	return FALSE;
}

#endif // _ATCOMMAND_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
