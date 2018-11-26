/*************************************************************************//**
 * @file
 *
 * @author Jordi Castells
 *
 * @date 4/05/2016
 * @brief Set of project specific shell commands
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup ShellCommands-Project Specific
 * @{
 * @brief A set of commands exclusive to the project
 *
 ****************************************************************************/

#ifndef _MON_SHELL_PROJECT_COMMANDS_H_
#define _MON_SHELL_PROJECT_COMMANDS_H_

/****************************************************************************
 * INCLUDE
 ***************************************************************************/
#include "shell_commands_map.h"

#ifdef MON_SHELL_PROJECT_SPECIFIC_COMMANDS

void projectExampleCommand1(tParsedCommand*);
void projectExampleCommand2(tParsedCommand*);
void projectAtCmd(tParsedCommand*);
void ATShellCallback(tAtResponseID eventID, uint8_t *buffer, uint16_t length);

#endif //MON_SHELL_PROJECT_SPECIFIC_COMMANDS


#endif //_MON_SHELL_PROJECT_COMMANDS_H_

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
