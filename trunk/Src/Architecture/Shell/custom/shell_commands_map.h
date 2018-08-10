/*************************************************************************//**
 * @file
 *
 * @author Jordi Castells
 *
 * @date 4/05/2016
 * @brief Header file containing the declaration of the shellCommands table.
 * this table is implemented in shell_commands_map.c
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup ShellCommandsMap
 * @{
 *
 ****************************************************************************/

#ifndef _MON_SHELL_COMMANDS_MAP_H_
#define _MON_SHELL_COMMANDS_MAP_H_

/****************************************************************************
 * INCLUDE
 ***************************************************************************/
#include "shell.h"

/****************************************************************************
 *  DEFINES
 ****************************************************************************/
/* AVAILABLE SHELL COMMANDS OF BASIC COMMAND PACKAGE */
#define MON_SHELL_COMMAND_HREG_RD           //Enable HREG reading command
#define MON_SHELL_COMMAND_HREG_WR           //Enable HREG writing command
#define MON_SHELL_COMMAND_SREG_RD            //Enable SREG reading command
#define MON_SHELL_COMMAND_SREG_WR            //Enable SREG Writing command
#define MON_SHELL_COMMAND_LOG_LVL            //Enable commands to set the log level
#define MON_SHELL_COMMAND_TRACE_REGS         //Enable trace commands
//#define MON_SHELL_COMMAND_VERBOSE_REGS       //Enable commands for verbose Hreg write/rd
#define MON_SHELL_COMMAND_SCHEDULER_TASKS    //Enable Commands to start/stop tasks

/* AVAILABLE SHELL COMMANDS OF I2C PACKAGE */
//#define MON_SHELL_I2C_COMMANDS

/* PROJECT SPECIFIC COMMAND PACKAGES */
//#define MON_SHELL_PROJECT_SPECIFIC_COMMANDS

/****************************************************************************
 *  EXTERNAL VARIABLES
 ****************************************************************************/
extern tShellCommandTableEntry shellCommandsTable[];

/****************************************************************************
 *  PUBLIC FUNCTION PROTOTYPES
 ****************************************************************************/
inline uint32_t shellGetNumOfCommands( void );

#endif //_MON_SHELL_COMMANDS_MAP_H_

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 *    }@}@
 ***************************************************************************/
