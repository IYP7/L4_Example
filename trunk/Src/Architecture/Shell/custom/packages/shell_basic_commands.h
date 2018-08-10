/*************************************************************************//**
 * @file
 *
 * @author Jordi Castells
 *
 * @date 4/05/2016
 * @brief Declaration of basic commands
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup ShellCommands-basics
 * @{
 * @brief the legacy and basic shell commands.
 *
 ****************************************************************************/

#ifndef _MON_SHELL_BASIC_COMMANDS_H_
#define _MON_SHELL_BASIC_COMMANDS_H_

/****************************************************************************
 * INCLUDE
 ***************************************************************************/
#include "shell.h"

#include "hreg_dat.h" //For HREG commands
#include "sreg_dat.h" //For SREG commands

#include "Scheduler.h"  //For Scheduling tasks
#include "Scheduler_map.h"

/****************************************************************************
 *  PUBLIC FUNCTION PROTOTYPES
 ****************************************************************************/
//Always enabled
void runShellCommandHelp(tParsedCommand*);

#ifdef MON_SHELL_COMMAND_SREG_RD
void runShellCommandReadSreg(tParsedCommand*);
#endif

#ifdef MON_SHELL_COMMAND_HREG_RD
void runShellCommandReadHreg(tParsedCommand*);
#endif

#ifdef MON_SHELL_COMMAND_SREG_WR
void runShellCommandWriteSreg(tParsedCommand*);
#endif

#ifdef MON_SHELL_COMMAND_HREG_WR
void runShellCommandWriteHreg(tParsedCommand*);
#endif

#ifdef MON_SHELL_COMMAND_LOG_LVL
void runShellCommandSetLogLevel(tParsedCommand*);
#endif

#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
void runShellCommandTraceRegs(tParsedCommand*);
void runShellCommandVerboseHreg(tParsedCommand*);
void runShellCommandVerboseSreg(tParsedCommand*);
#endif

#ifdef MON_SHELL_COMMAND_SCHEDULER_TASKS
void runShellCommandSchedulerStartTask(tParsedCommand*);
void runShellCommandSchedulerStopTask(tParsedCommand*);
#endif

#endif //_MON_SHELL_BASIC_COMMANDS_H_

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
