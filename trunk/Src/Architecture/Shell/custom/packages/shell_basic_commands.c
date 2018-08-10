/*************************************************************************//**
 * @file
 *
 * @author Jordi Castells
 *
 * @date 4/05/2016
 * @brief Basic shell commands implementations
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup ShellCommands-basics
 * @{
 * @brief the legacy and basic shell commands.
 *
 ****************************************************************************/

#include "shell.h"
#include "shell_basic_commands.h"
#include "shell_commands_map.h" //used by help to list all the available commands

/*************************************************************************//**
 * @brief   Shell help command prints all the valid commands
 * @param	command Pointer to command
 * @return  None.
 ****************************************************************************/
void runShellCommandHelp(tParsedCommand* command)
{
    uint8_t i;
    const tShellCommandTableEntry *sCommand;

    printShellString((int8_t*)"\r\nAVAILABLE COMMANDS\n", VERBOSE_EVENT);

    for(i=0;i<shellGetNumOfCommands();i++){
        sCommand = &shellCommandsTable[i];
        printShellString((int8_t*)sCommand->command, VERBOSE_EVENT);
        printShellString((int8_t*)"[", VERBOSE_EVENT);
        printShellValue(sCommand->numOfParams, TYPE_DECIMAL, VERBOSE_EVENT);
        printShellString((int8_t*)"]\n", VERBOSE_EVENT);
        HAL_Delay(10);
    }
}


#ifdef MON_SHELL_COMMAND_SREG_RD
/*************************************************************************//**
 * @brief   Reads an sreg and prints its value.
 * @param	command Pointer to command
 * @return  None.
 ****************************************************************************/
void runShellCommandReadSreg(tParsedCommand* command)
{
    uint16_t regIndex;

    regIndex = command->params[0].value;

    if( regIndex >= getNumOfSregs() ){
        printCommonString(SHELL_INVALID_REGISTER, VERBOSE_ERROR);
    }
    else{
        printShellValue(readShellSreg(regIndex), TYPE_DECIMAL, VERBOSE_EVENT);
    }
}
#endif

#ifdef MON_SHELL_COMMAND_HREG_RD
/*************************************************************************//**
 * @brief   Reads an sreg and prints its value.
 * @param	command Pointer to command
 * @return  None.
 ****************************************************************************/
void runShellCommandReadHreg(tParsedCommand* command)
{
    uint16_t instance, regIndex;
    uint32_t value = 0;
    eError err;

    instance = command->params[0].value;
    regIndex = command->params[1].value;

    if( regIndex >= getNumOfHregs() )
    {
        printCommonString(SHELL_INVALID_REGISTER, VERBOSE_ERROR);
    }
    else
    {
        err = readShellHreg(instance, regIndex, &value);
        if(err != RET_OK)
        {
            printCommonString(SHELL_READ_ERROR, VERBOSE_ERROR);
        }
        else
        {
            printShellValue(value, TYPE_DECIMAL, VERBOSE_EVENT);
        }
    }
}
#endif

#ifdef MON_SHELL_COMMAND_SREG_WR
/*************************************************************************//**
 * @brief       Write an sreg.
 * @param[in]	command Pointer to command
 * @return      None.
 ****************************************************************************/
void runShellCommandWriteSreg(tParsedCommand* command)
{
    uint16_t regIndex;
    uint32_t regValue;

    regIndex = command->params[0].value;
    regValue = command->params[1].value;

    if( writeShellSreg(regIndex, regValue) ){
        printCommonString(SHELL_WRITE_ERROR, VERBOSE_ERROR);
    }
}
#endif

#ifdef MON_SHELL_COMMAND_HREG_WR
/*************************************************************************//**
 * @brief   Reads an sreg and prints its value.
 * @param	command Pointer to command
 * @return  None.
 ****************************************************************************/
void runShellCommandWriteHreg(tParsedCommand* command)
{
    uint16_t instance, regIndex;
    uint32_t regValue;

    instance = command->params[0].value;
    regIndex = command->params[1].value;
    regValue = command->params[2].value;

    if( regIndex >= getNumOfHregs() ){
        printCommonString(SHELL_INVALID_REGISTER, VERBOSE_ERROR);
    }
    else{
        if( writeShellHreg( instance, regIndex, regValue ) != RET_OK ){
            printCommonString(SHELL_WRITE_ERROR, VERBOSE_ERROR);
        }
    }
}
#endif

#ifdef MON_SHELL_COMMAND_LOG_LVL
/*************************************************************************//**
 * @brief       Set the log level
 * @todo        Maybe parse log level from string?
 * @param[in]	command Pointer to command
 * @return      None.
 ****************************************************************************/
void runShellCommandSetLogLevel(tParsedCommand* command)
{
    uint32_t regValue;

    regValue = command->params[0].value;

    if( regValue > NUM_OF_VERBOSE_LEVELS ){
        printCommonString(SHELL_INVALID_PARAMETER, VERBOSE_ERROR);
    }
    else{
        shellSetVerboseLevel(regValue);
    }
}
#endif


#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
/*************************************************************************//**
 * @brief       Set traceability of HREGS/SREGS
 * @param[in]	command Pointer to command
 * @return      None.
 ****************************************************************************/
void runShellCommandTraceRegs(tParsedCommand* command)
{
    if( command->params[0].value == TRUE || command->params[0].value == FALSE ){
        traceRegs = (uint8_t)command->params[0].value;
    }
    else{
        printCommonString(SHELL_INVALID_PARAMETER, VERBOSE_ERROR);
    }
}

void runShellCommandVerboseHreg(tParsedCommand* command)
{
    uint16_t instance, regIndex;
    uint32_t regValue;

    instance = command->params[0].value;
    regIndex = command->params[1].value;
    regValue = command->params[2].value;

    if( shellSetHREGVerboseStatus(instance, regIndex, regValue) ){
        printCommonString(SHELL_INVALID_PARAMETER, VERBOSE_ERROR);
    }
}

void runShellCommandVerboseSreg(tParsedCommand* command)
{
    uint16_t regIndex;
    uint32_t regValue;

    regIndex = command->params[0].value;
    regValue = command->params[1].value;

    if( shellSetSREGVerboseStatus(regIndex, regValue) ){
        printCommonString(SHELL_INVALID_PARAMETER, VERBOSE_ERROR);
    }
}
#endif

#ifdef MON_SHELL_COMMAND_SCHEDULER_TASKS

void runShellCommandSchedulerStartTask(tParsedCommand* command)
{
    if ( command->params[0].value < NUM_OF_SCHED_TASKS ){
    	SchedulerEnableTask(command->params[0].value);
    }
    else{
        printCommonString(SHELL_INVALID_PARAMETER, VERBOSE_ERROR);
    }
}

void runShellCommandSchedulerStopTask(tParsedCommand* command)
{
    if ( command->params[0].value < NUM_OF_SCHED_TASKS ){
    	SchedulerDisableTask(command->params[0].value);
    }
    else{
        printCommonString(SHELL_INVALID_PARAMETER, VERBOSE_ERROR);
    }
}

#endif


/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
