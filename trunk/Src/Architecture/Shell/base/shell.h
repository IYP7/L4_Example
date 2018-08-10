/*************************************************************************//**
 * @file shell.h
 *
 * @author Jordi Castells
 *
 * @date 22/02/2016
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup Shell
 * @{
 *
 ****************************************************************************/

#ifndef _SHELL_H_
#define _SHELL_H_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDE
 ***************************************************************************/
#include "hreg.h"
#include "sreg.h"

/****************************************************************************
 *  DEFINES
 ****************************************************************************/
#define SHELL_MESSAGE_LENGHT_MAX  39
#define END_OF_LINE_CHARACTER 13        //EOL character to search for
#define CR_CHARACTER 10                 //Carriage return
#define SHELL_NUMBER_OF_PARAMETERS 3    //Maximum number of parameters for a Shell command
#define MON_SHELL_PARAM_BUFFER_SIZE 32  //Maximum size of a param buffer
#define SHELL_COMMAND_VERBOSITY VERBOSE_EVENT
#define SHELL_MAX_COMMAND_NAME_SIZE 8

/**
* SET of helper macros
*/
#define SHELL_PRINT_HEX_VALUE(x)    printShellValue(x, TYPE_HEX, SHELL_COMMAND_VERBOSITY)
#define SHELL_PRINT_DEC_VALUE(x)    printShellValue(x, TYPE_DECIMAL, SHELL_COMMAND_VERBOSITY)
#define SHELL_PRINT_STR(x)          printShellString((int8_t*)x, SHELL_COMMAND_VERBOSITY)

#define SHELL_PRINTLN_HEX_VALUE(x)  printShellValue(x, TYPE_HEX, SHELL_COMMAND_VERBOSITY); \
                                    SHELL_PRINT_NEWLINE()

#define SHELL_PRINTLN_DEC_VALUE(x)  printShellValue(x, TYPE_DECIMAL, SHELL_COMMAND_VERBOSITY); \
                                    SHELL_PRINT_NEWLINE()

#define SHELL_PRINTLN_STR(x)        printShellString((int8_t*)x, SHELL_COMMAND_VERBOSITY); \
                                    SHELL_PRINT_NEWLINE()

#define SHELL_PRINT_NEWLINE()       printShellString((int8_t*)"\n", SHELL_COMMAND_VERBOSITY)
#define SHELL_PRINT_ERROR(err)      printCommonString(err, VERBOSE_ERROR)
#define SHELL_PRINT_STORED_STR(x)   printCommonString(x, SHELL_COMMAND_VERBOSITY)

/****************************************************************************
 *  TYPES DEFINITION
 ****************************************************************************/

/** @brief Values of SREG_SHELL_STACK_STATE */
typedef enum
{
	SHELL_STACK_NOT_READY = 0x00,
	SHELL_STACK_READY,
	SHELL_STACK_OFF,

	SHELL_STACK_MAX
} eShellStates;

/** @brief SHELL predefined strings */
typedef enum {
    SHELL_INVALID_REGISTER = 0,
    SHELL_WRITE_ERROR,
    SHELL_ERROR_NOT_INIT,
    SHELL_COMPLETED,
    SHELL_INVALID_COMMAND,
    SHELL_INVALID_PARAMETER,
    SHELL_BUFFER_TOO_BIG,
    SHELL_READ_ERROR,

    SHELL_NO_MESSAGE,
    SHELL_MAX_COMMON_COMMANDS
} tShellStringId;

/**@brief Enumeration for an input parameter type (after parsing)*/
typedef enum
{
    PARAM_UINT,         /**< the parameter is an unsigned int*/
    PARAM_DATA,         /**< the parameter is a string */
    NUM_OF_PARAM_TYPES
} eParamType;

/** @typedef @brief A simple parsed parameter */
typedef struct sParsedParam{
    eParamType type;
    uint32_t value;     /**< Depending on the Type this is treated as a number or as a pointer*/
} tShellParam;

/**
* @typedef @breif Fully parsed command. This is the structure that any shell command
* execution function will receive.
*/
typedef struct sParsedCommand
{
    uint32_t        command;
    tShellParam     params[SHELL_NUMBER_OF_PARAMETERS]; //Maximum of 3 params
} tParsedCommand;

/** @typedef @brief Command function pointer. For callbacks */
typedef void (*shellCommandFunction)(tParsedCommand*);

/** @typedef @brief A shell command entry */
typedef struct sShellCommandTableEntry
{
    int8_t                  command[SHELL_MAX_COMMAND_NAME_SIZE];
    uint8_t                 numOfParams;
    shellCommandFunction    run;

} tShellCommandTableEntry;

/****************************************************************************
 *  PUBLIC FUNCTION PROTOTYPES
 ****************************************************************************/
void initShellStack( void );
void callShellStackTask( void );
void printShellString ( int8_t* string, tVerboseLevels vLevel );
void printShellValue ( int32_t value, uint8_t type, tVerboseLevels vLevel);

void printCommonString( uint8_t, tVerboseLevels);

#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
eError shellSetHREGVerboseStatus(uint32_t regIndex, uint8_t instance, bool state );
uint8_t shellEvaluateHREGVerboseStatus( uint16_t regIndex, uint8_t instance, uint32_t value );
eError shellSetSREGVerboseStatus(uint32_t regIndex, bool state );
uint8_t shellEvaluateSREGVerboseStatus( uint16_t regIndex, uint32_t value );
#endif //MON_SHELL_COMMAND_VERBOSE_REGS

BOOL shellGetDebugState( void );
void shellSetVerboseLevel(tVerboseLevels);
tVerboseLevels shellGetVerboseLevel(void);

#endif /* _SHELL_H_ */

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
