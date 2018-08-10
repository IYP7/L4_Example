/*************************************************************************//**
 * @file
 *
 * @author Jordi Castells
 *
 * @date 4/05/2016
 * @brief Set of shell commands that make use of smbUtils to read/write 8/16
 * bits of information from different devices directly from the shell
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup ShellCommands-i2c
 * @{
 * @brief a set of commands to access the I2C bus directly from the shell
 * @detailed this commands give more features to higher level Montseny shells
 * like montseny serialSHell
 *
 ****************************************************************************/

#ifndef _SHELL_I2C_COMMANDS_H_
#define _SHELL_I2C_COMMANDS_H_

/****************************************************************************
 * INCLUDE
 ***************************************************************************/
#include "shell_commands_map.h"

#ifdef MON_SHELL_I2C_COMMANDS

#include "Shell.h"

void runShellCommandReadI2c16(tParsedCommand*);
void runShellCommandWriteI2c16(tParsedCommand*);
void runShellCommandReadI2c8(tParsedCommand*);
void runShellCommandWriteI2c8(tParsedCommand*);
void runShellCommandLsI2c(tParsedCommand*);

#endif //MON_SHELL_I2C_COMMANDS

#endif /* SHELL_I2C_COMMANDS_H_ */

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
