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

#endif //MON_SHELL_PROJECT_SPECIFIC_COMMANDS

#endif //_MON_SHELL_PROJECT_COMMANDS_C_

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/

