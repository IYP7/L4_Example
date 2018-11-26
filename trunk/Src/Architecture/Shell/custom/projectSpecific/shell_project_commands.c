/*************************************************************************//**
 * @file
 *
 * @author Jordi Castells
 *
 * @date 24/05/2016
 * @brief Set of utility commands for grifols
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup ShellCommands-Grifols
 * @{
 *
 * @brief A set of commands exclusive to the Grifols Firmware
 *
 ****************************************************************************/

#ifndef _MON_SHELL_PROJECT_COMMANDS_C_
#define _MON_SHELL_PROJECT_COMMANDS_C_

/****************************************************************************
 * INCLUDE
 ***************************************************************************/
#include "shell_commands_map.h"

#ifdef MON_SHELL_PROJECT_SPECIFIC_COMMANDS

#include "common.h"
#include "Hreg_dat.h"
#include "shell.h"      //shell utils
#include <string.h>

/*************************************************************************//**
 * @brief       Example command without behaviour
 * @param[in]	command Pointer to command
 * @return      None.
 ****************************************************************************/
void projectExampleCommand1(tParsedCommand* command)
{

}

/*************************************************************************//**
 * @brief       Example command without behaviour
 * @param[in]	command Pointer to command
 * @return      None.
 ****************************************************************************/
void projectExampleCommand2(tParsedCommand* command)
{

}

/*************************************************************************//**
 * @brief       Send an AT Command request
 * @param[0]	mode
 * @param[1]	timeout in ms
 * @param[2]	timeout retries
 * @param[3]	command
 * @return      None.
 ****************************************************************************/
void projectAtCmd(tParsedCommand* command)
{
	uint8_t cmd[128]={0};
	uint8_t size = command->params[3].size - 2;
	tAtRequest req;/* = {0, 0, 0, 0, cmd};*/
	req.errorRetries = 0;
	req.timeout = command->params[1].value;
	req.timeoutRetries = command->params[2].value;
	req.mode = command->params[0].value;
	req.command = (uint8_t *) "AT+dummy";

	memcpy(cmd, (uint8_t *)command->params[3].value, size);
	req.command = cmd;
	ATCommandSetRequest(AT_CLIENT_SHELL, req);
}

void ATShellCallback(tAtResponseID eventID, uint8_t *buffer, uint16_t length)
{
	switch(eventID)
	{
		case AT_RESP_OK:
			printShellString((int8_t *)"AT_RESP_OK:\n", VERBOSE_GENERAL);
			break;
		case AT_RESP_DATA:
			printShellString((int8_t *)"AT_RESP_DATA:\n", VERBOSE_GENERAL);
			break;
		case AT_RESP_ERROR:
			printShellString((int8_t *)"AT_RESP_ERROR:\n", VERBOSE_GENERAL);
			break;
		case AT_RESP_TIMEOUT:
			printShellString((int8_t *)"AT_RESP_TIMEOUT:\n", VERBOSE_GENERAL);
			break;
		case AT_RESP_URC:
			printShellString((int8_t *)"AT_RESP_URC:\n", VERBOSE_GENERAL);
			break;
	}
	WRITE_HREG(UART_LOG, HREG_UART_BUFFER_SIZE, length);
	WRITE_HREG(UART_LOG, HREG_UART_DATA, buffer);
}

#endif //MON_SHELL_PROJECT_SPECIFIC_COMMANDS

#endif //_MON_SHELL_PROJECT_COMMANDS_C_

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/

