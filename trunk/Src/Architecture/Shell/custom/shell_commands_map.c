/*************************************************************************//**
 * @file
 *
 * @author Jordi Castells
 *
 * @date 4/05/2016
 * @brief Shell commands map definition
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup ShellCommandsMap
 * @{
 *
 ****************************************************************************/
#include "shell_commands_map.h"
#include "shell_basic_commands.h"

#ifdef MON_SHELL_I2C_COMMANDS
#include "shell_i2c_commands.h"
#endif // MON_SHELL_I2C_COMMANDS

#ifdef MON_SHELL_PROJECT_SPECIFIC_COMMANDS
#include "shell_project_commands.h"
#endif // MON_SHELL_PROJECT_SPECIFIC_COMMANDS

tShellCommandTableEntry shellCommandsTable[] = {
    /* SHELL_COMMAND_HELP      	*/	{ "help",  0, &runShellCommandHelp },
#ifdef MON_SHELL_COMMAND_SREG_RD
    /* SHELL_COMMAND_RSREG     	*/	{ "rsreg", 1, &runShellCommandReadSreg },
#endif
#ifdef MON_SHELL_COMMAND_SREG_WR
    /* SHELL_COMMAND_WSREG     	*/	{ "wsreg", 2, &runShellCommandWriteSreg },
#endif

#ifdef MON_SHELL_COMMAND_HREG_RD
    /* SHELL_COMMAND_RHREG     	*/	{ "rhreg", 2, &runShellCommandReadHreg },
#endif

#ifdef MON_SHELL_COMMAND_HREG_WR
    /* SHELL_COMMAND_WHREG     	*/	{ "whreg", 3, &runShellCommandWriteHreg },
#endif

#ifdef MON_SHELL_COMMAND_LOG_LVL
    /* SHELL_COMMAND_VERBOSE   	*/	{ "log",   1, &runShellCommandSetLogLevel},
#endif

#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
    /* SHELL_COMMAND_TRACE_REGS	*/	{ "trace", 1, &runShellCommandTraceRegs},
    /* SHELL_COMMAND_VHREG     	*/	{ "vhreg", 3, &runShellCommandVerboseHreg},
    /* SHELL_COMMAND_VSREG     	*/	{ "vsreg", 2, &runShellCommandVerboseSreg},
#endif

#ifdef MON_SHELL_COMMAND_SCHEDULER_TASKS
	/* SHELL_COMMAND_START		*/	{ "tstart", 1, &runShellCommandSchedulerStartTask},
	/* SHELL_COMMAND_STOP		*/	{ "tstop", 1, &runShellCommandSchedulerStopTask},
#endif

#ifdef MON_SHELL_I2C_COMMANDS
    /* SHELL_COMMAND_RDI2C */	{ "rdi2c16", 2, &runShellCommandReadI2c16 },
    /* SHELL_COMMAND_WRI2C */	{ "wri2c16", 3, &runShellCommandWriteI2c16 },
    /* SHELL_COMMAND_RDI2C */	{ "rdi2c8", 2, &runShellCommandReadI2c8 },
    /* SHELL_COMMAND_WRI2C */	{ "wri2c8", 3, &runShellCommandWriteI2c8 },
    /* SHELL_COMMAND_LSI2C */	{ "lsi2c", 0, &runShellCommandLsI2c },
#endif

#ifdef MON_SHELL_PROJECT_SPECIFIC_COMMANDS
    /* SHELL_COMMAND_RESET    */{ "Example1"  , 0, &projectExampleCommand1},
    /* SHELL_COMMAND_BOOTMODE */{ "Example2"  , 0, &projectExampleCommand2},
#endif

};

/*************************************************************************//**
 * @brief   Gets the number of shell commands defined
 * @return  Total number of commands defined for this shell
 ****************************************************************************/
inline uint32_t shellGetNumOfCommands( void )
{
    return (sizeof(shellCommandsTable) / sizeof(tShellCommandTableEntry));
}

/****************************************************************************
 *    }@}@
 ***************************************************************************/
