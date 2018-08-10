/*************************************************************************//**
 * @file shell.c
 * @date 22/02/2016
 *
 *
 * @author Jordi Castells
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup Shell
 * @{
 *
 * @brief shell input/output implementation.
 * @detail This file is providing the interface to a LOG output
 *
 * This provides a parsing of commands mechanism with a configurable set of
 * commands (Check config.h for the enabled/disabled commands) And ease
 * of extension
 *
 * For Information on then commands check the SHELL_README.md file.
 *
 ****************************************************************************/


/****************************************************************************
 *    INCLUDE
 ****************************************************************************/
#include "common.h"

#include "hreg_dat.h"
#include "sreg_dat.h"

#include "System_app.h"
#include "pipeDevice.h"
#include <string.h>
#include "shell_commands_map.h" //Where the commands table is defined
#include "shell.h"

/****************************************************************************
 *    PRIVATE TYPE DEFINITIONS
 ****************************************************************************/
/** @typedef A single shell token. Used during the parsing of the shell */
typedef struct sShellToken
{
    int8_t  string[MON_SHELL_PARAM_BUFFER_SIZE];
    uint8_t size;
} tShellToken;

/** @typedef Previously defined shell message */
typedef struct sShellMessage
{
    int8_t message[SHELL_MESSAGE_LENGHT_MAX];
} tShellMessage;

/****************************************************************************
 *    CONSTANTS
 ****************************************************************************/
#define SHELL_MAX_BUFFER 50
#define SHELL_INPUT_COMMANDBUFFER 128

/****************************************************************************
 *  PRIVATE FUCNTION PROTOTYPE
 ***************************************************************************/
//static void printCommonString( uint8_t index, eVerboseLevels vLevel );
static void parseShellStack ( void );
static void printStartingMessage(tBool fullVersion);
void initVerboseRegs(void);

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
static eShellStates shellState = SHELL_STACK_NOT_READY;

static const tShellMessage shellCommonMessages[] = {
    /*SHELL_INVALID_REGISTER	*/	{"ERR: Invalid Register\n\0"},
    /*SHELL_WRITE_ERROR			*/	{"ERR: Write error\n\0"},
    /*SHELL_ERROR_NOT_INIT		*/	{"ERR: Illegal SHELL call before INIT\n\0"},
    /*SHELL_COMPLETED			*/	{"SHELL Stack Created.\n\0"},
    /*SHELL_INVALID_COMMAND		*/	{"ERR: Invalid command\n\0"},
    /*SHELL_INVALID_PARAMETER	*/	{"ERR: Invalid parameter\n\0"},
    /*SHELL_BUFFER_TOO_BIG	    */	{"ERR: Buffer is Too Big\n\0"},
    /*SHELL_READ_ERROR			*/	{"ERR: Read error\n\0"},
};

static uint8_t command[SHELL_INPUT_COMMANDBUFFER];
//Data buffer to send buffer pointers to HREGs that require them
uint8_t  shellDataBuffer[SHELL_INPUT_COMMANDBUFFER]; //this will always be equal or less

#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
static uint32_t     verboseHREGS[NUM_OF_HREGS];
static uint8_t      verboseSREGS[NUM_OF_SREGS];
static uint8_t      traceRegs   = FALSE;
#endif // MON_SHELL_COMMAND_VERBOSE_REGS

static tVerboseLevels verboseLevel = VERBOSE_GENERAL;
static uint8_t        debugEnable = FALSE;


/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/
/*************************************************************************//**
 * @brief   Adjust shell versbose level
 * @param	lvl New verbose level
 * @return  None.
 ****************************************************************************/
void shellSetVerboseLevel(tVerboseLevels lvl)
{
    verboseLevel = lvl;
}

/*************************************************************************//**
 * @brief  Get current verbose level
 * @return  None.
 ****************************************************************************/
tVerboseLevels shellGetVerboseLevel(void)
{
    return verboseLevel;
}

#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
/*************************************************************************//**
 * @brief   Initialize structures for verbose HREGS.
 * @return  None.
 * @todo THIS is command dependant. And we should be able to provide initialization
 * functions for commands that need so
 ****************************************************************************/
void initVerboseRegs(void)
{
    uint8_t i = 0;
    uint8_t j = 0;

    traceRegs = FALSE;

    /* Initialize verbose status of HREGS */
    for(i = 0; i < NUM_OF_HREGS; i++)
    {
		verboseHREGS[i] = 0;
    }
    /* Initialize verbose status of SREGS */
    for(i = 0; i < NUM_OF_SREGS; i++)
    {
		verboseSREGS[i] = 0;
    }

}
#endif

/*************************************************************************//**
 * @brief   Initialization of the Shell application module.
 * @param	Initial message to be printed.
 * @return  None.
 ****************************************************************************/
void initShellStack( void )
{
    /* Initialises resources and print out INIT information */
    verboseLevel = VERBOSE_GENERAL;
    debugEnable = FALSE;

#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
    initVerboseRegs();
#endif //MON_SHELL_COMMAND_VERBOSE_REGS

    shellState = SHELL_STACK_READY;

    /* Print welcome message, power up reason and HW/SW version */
    printStartingMessage(FALSE);

    printCommonString(SHELL_COMPLETED, VERBOSE_GENERAL);

    printShellString((int8_t*)"\r\nSHELL-> \0", VERBOSE_GENERAL);
}

/*************************************************************************//**
 * @brief   Shell application task module.
 *
 * @param	None.
 *
 * @return  None.
 *
 * @details Checks for input data and generic app control
 *
 ****************************************************************************/
void callShellStackTask( void )
{
    uint8_t bytesCount = 0;
    int8_t pattern[2] = { 0x0D, 0x0A };

    while ( pipeDeviceParseCommand( PIPE_LOG, command, SHELL_INPUT_COMMANDBUFFER, pattern, 2, &bytesCount) )
    {
        parseShellStack();
    }
}


/*************************************************************************//**
 * @brief   From inputBuffer, extract The tokens (command & params).
 * @todo    This does not check for a command bigger than the buffer
 *
 * @param[in]	commandBuffer Input string buffer with the command
 * @param[out]  tokens Array with tokens
 * @param[out]  size Amount of tokens parsed
 * @return  nothing
 *
 ****************************************************************************/
void tokenizeShellCommand(uint8_t *commandBuffer, tShellToken *tokens, uint8_t* size )
{
    uint8_t counter = 0;
    uint8_t currentToken = 0;
    uint8_t elementCounter = 0;

    *size = 0;

    //Strip leading
    while (commandBuffer[counter] == ' '){
        counter++;
    }

    /* allocate received string on diferent buffers separeted by space chararcter*/
    while (counter < SHELL_INPUT_COMMANDBUFFER){
        //space -> New parameter
        if (commandBuffer[counter] == ' '){
            tokens[currentToken].size = elementCounter;
            tokens[currentToken].string[elementCounter] = '\0';
            elementCounter = 0;
            currentToken++;

            if ( currentToken == SHELL_NUMBER_OF_PARAMETERS + 1 ){
				break;
            }
            counter++;
        }
        //EOL -> End of command
		else if(commandBuffer[counter] == END_OF_LINE_CHARACTER){
			    tokens[currentToken].size = elementCounter;
	            tokens[currentToken].string[elementCounter] = '\0';
			    break;
        }
        //Otherwise, copy the character to its own parameter
        else{
            //@todo What if parameter bigger than expected?
            tokens[currentToken].string[elementCounter] = commandBuffer[counter];
            elementCounter++;
            counter++;
        }
    }

    *size = currentToken;
}

/*************************************************************************//**
 * @brief   Extract the shellCommand from a token
 *
 * @param	[r] A shell Token with a string command
 * @return  Enum value with the shell command
 *
 ****************************************************************************/
uint32_t shellCommandIdFromToken(tShellToken* token)
{
    uint8_t i;

    //Check which command matches
    for (i = 0; i < shellGetNumOfCommands(); i++){
        if(!strncmp((char*)token->string, (char*)shellCommandsTable[i].command, SHELL_MAX_COMMAND_NAME_SIZE)){
            return i; //This is the command
        }
    }

    //None found, return a number not corresponding with any command
    return shellGetNumOfCommands() + 1;
}

/*************************************************************************//**
 * @brief   parse a shell param from a token to an input param structure
 *
 * @param[in]	token A shell Token with a parameter
 * @param[out]	sParam a parameter structure to update with token information
 * @param[out]	wrBuffer Buffer where to copy the parameter data if necessary
 *              The buffer should be at least token->size
 *              Returns it as a pointer to the last written direction
 * @param[out]	bufferWrSize Number of bytes written into the buffer
 * @return      String ID with the error ocurred. Or SHELL_NO_MESSAGE if everything OK
 *
 ****************************************************************************/
tShellStringId parseShellParamToken(const tShellToken* token, tShellParam* sParam, uint8_t* wrBuffer, uint8_t* bufferWrSize)
{
    uint8_t size;

    //This is a type string
    if(token->string[0] == '"'){
        if(token->size > MON_SHELL_PARAM_BUFFER_SIZE){
            return SHELL_BUFFER_TOO_BIG;
        }

        //+1 to avoid the first quote. -2 because the size is TEXT - two quotes
        memcpy(wrBuffer, token->string + 1, token->size - 2 );

        sParam->type  = PARAM_DATA;
        sParam->value = (uint32_t) wrBuffer; //Integer from pointer

        //Update shellDataBufferPointer
        *bufferWrSize = token->size - 2;
    }
    //Default is treated as Unsigned int
    else{
        size = token->size;
        sParam->type  = PARAM_UINT;
        _atoi( &(token->string), &(sParam->value), &size);
        if ( size != 0 ){
            return SHELL_INVALID_REGISTER;
        }
    }

    return SHELL_NO_MESSAGE;
}

/*************************************************************************//**
 * @brief   Parse incoming command.
 *
 * @param[in]   commandBuffer Input buffer to be parsed.
 * @param[out]  commandParsed struct with the parsed command
 *
 * @return  StringId with the parsing error, or OK
 *
 ****************************************************************************/
tShellStringId parseShellCommand(uint8_t *commandBuffer, tParsedCommand *commandParsed )
{
    tShellToken  elements[SHELL_NUMBER_OF_PARAMETERS + 1];
    tShellToken* pElement;  //parsed Element
    tShellParam* sParam;    //shell Parameter
    uint8_t i, size, numOfParams;

    uint32_t sCommand; //Which command is parsed.
    uint8_t* shellDataBufferWrPtr;
    tShellStringId retString;

    //The buffer is by default the initi of the shell buffer
    shellDataBufferWrPtr = shellDataBuffer;

    //Tokenize from input
    tokenizeShellCommand(commandBuffer, elements, &numOfParams);

    /**COMMAND PARSING (the first parameter)*/
    sCommand = shellCommandIdFromToken(&(elements[0]));

    //If it does not match any command, or the parameters do not match. Fail!
    if ( (sCommand >= shellGetNumOfCommands())
         || (numOfParams != shellCommandsTable[sCommand].numOfParams) ){
        return SHELL_INVALID_COMMAND;
    }
    else{
        commandParsed->command = sCommand;
    }

    /**PARAMETER PARSING (remaining parameters)*/
    for(i=0; i < numOfParams; i++){
        pElement = &(elements[i+1]);
        sParam   = &(commandParsed->params[i]);

        //We are writing too far ahead, Stop, this is an error
        if((uint32_t)(shellDataBufferWrPtr + pElement->size) > (uint32_t)(&(shellDataBuffer) + SHELL_INPUT_COMMANDBUFFER)){
            return SHELL_BUFFER_TOO_BIG;
        }

        retString = parseShellParamToken(pElement, sParam, shellDataBufferWrPtr, &size);

        //@todo this is the default handling of the shell buffer. Simply copy
        //and update the used buffer
        shellDataBufferWrPtr += size;

        if(retString != SHELL_NO_MESSAGE) {
            return retString;
        }
    }

    return SHELL_COMPLETED;
}

/*************************************************************************//**
 * @brief   Prints a Value to the WINDOWS CONSOLE or HW driver.
 * 			Verbose Level of request should be bigger than actual verbose value.
 *
 * @param	Value to be displayed.
 * @param   Value type.
 * @param   verbose Level of request
 *
 * @return  None.
 *
 ****************************************************************************/
void printShellValue ( int32_t value, uint8_t type, tVerboseLevels vLevel)
{
    int8_t buffer[SHELL_MAX_BUFFER];

	itos( value , type , buffer );
	printShellString( buffer, vLevel );
}

/*************************************************************************//**
 * @brief   Prints String to the WINDOWS CONSOLE or HW driver.
 * 			Verbose Level of request should be bigger than actual verbose value.
 *
 * @param	String to be displayed.
 * @param	Verbose level of String.
 *
 * @return  None.
 *
 * @details This function is protected against SHELL not initialised.
 *
 ****************************************************************************/
void printShellString ( int8_t* buffer, tVerboseLevels vLevel )
{

    if ( vLevel >= verboseLevel )
    {
        /* Init protection */
        if( shellState == SHELL_STACK_READY )
        {
#ifdef UART_LOG
            WRITE_HREG(UART_LOG, HREG_UART_BUFFER_SIZE, strlen((char* )buffer));
            WRITE_HREG(UART_LOG, HREG_UART_DATA, buffer);
#endif
#ifdef CAN_LOG
            ProtocolTraceSend((uint8_t *)buffer, 0, strlen((char* )buffer));
#endif
        }
    }
}

/****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/


/*************************************************************************//**
 * @brief   Prints welcome message, power up reason and HW/SW version.
 * @param	None.
 * @return  None.
 ****************************************************************************/
static void printStartingMessage(tBool fullVersion)
{
    tPowerUpReasons reason = 0;
#ifdef TEST_VERSION
	uint8_t testVersion[2];
#endif

    printShellString((int8_t*)"\n", VERBOSE_GENERAL);
#ifdef TEST_VERSION
    printShellString(((int8_t *)"TEST VERSION OF: "), VERBOSE_GENERAL);
#endif
    printShellString(((int8_t *)START_APPLICATION_MESSAGE), VERBOSE_GENERAL);

    READ_HREG( UNIQUE_INSTANCE, HREG_SYSTEM_POWER_UP_REASON, &reason );
    if (  reason == SYSTEM_FIRST_POWER_UP )
    {
		printShellString((int8_t*)"\nPowering up first time.", VERBOSE_GENERAL);
    }
    else
    {
		printShellString((int8_t*)"\nPowering up from reset.", VERBOSE_GENERAL);
    }
    printShellString((int8_t*)"\nHW Version : ", VERBOSE_GENERAL);
    printShellString((int8_t*)"\nFW Version : V.", VERBOSE_GENERAL);
    printShellValue(FW_MAJOR_VERSION, TYPE_DECIMAL, VERBOSE_GENERAL);
    printShellString((int8_t*)".", VERBOSE_GENERAL);
    printShellValue(FW_MINOR_VERSION, TYPE_DECIMAL, VERBOSE_GENERAL);
#ifndef TEST_VERSION
    if ( fullVersion == TRUE )
    {
    	printShellString((int8_t*)".RC", VERBOSE_GENERAL);
    	printShellValue(FW_RC_VERSION, TYPE_DECIMAL, VERBOSE_GENERAL);
    }
#else //TEST_VERSION
    printShellString((int8_t*)".TS-", VERBOSE_GENERAL);
    testVersion[0] = 'A' + FW_TEST_VERSION;
    testVersion[1] = 0;
    printShellString((int8_t*)testVersion, VERBOSE_GENERAL);
#endif //TEST_VERSION
    printShellString((int8_t*)"\nNVM Version : ", VERBOSE_GENERAL);
    printShellString((int8_t*)"\n", VERBOSE_GENERAL);
}

/*************************************************************************//**
 * @brief   Prints a predefined string.
 * @param	index of string to print.
 * @return  None.
 ****************************************************************************/
void printCommonString( uint8_t index, tVerboseLevels vLevel )
{
    /* String index */
    if( index != SHELL_NO_MESSAGE)
    {
        printShellString((int8_t*)shellCommonMessages[index].message, vLevel);
    }
}


/*************************************************************************//**
 * @brief   Unknown command
 * @todo    review. Why do we need readShellSreg? and not simply read an sreg?
 * @param	command Pointer to command
 * @return  None.
 ****************************************************************************/
static void runShellCommandUnknown(tParsedCommand* command)
{
    printShellString((int8_t*)"\r\nUNKNOWN COMMAND\n", VERBOSE_WARNING);
}

/*************************************************************************//**
 * @brief   Parse incoming message.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
static void parseShellStack ( void )
{
    uint8_t  i = 0;
    uint8_t  parseError = SHELL_NO_MESSAGE;
    tParsedCommand commandParsed;
    uint8_t  numberOfCommands = 1;
    uint8_t  offset[2];

    offset[0] = 0;
    /* Check if it is a double command */
    while( command[i] != END_OF_LINE_CHARACTER )
    {
        if ( command[i] == '&' && i >= 1 )
        {
			numberOfCommands = 2;
			command[i - 1] = END_OF_LINE_CHARACTER;
			offset[1] = i + 2;
        }
        i++;
    }

    i = 0;
    while ( i < numberOfCommands )
    {
        parseError = parseShellCommand(&command[offset[i]], &commandParsed );

        if ( parseError == SHELL_COMPLETED )
        {
            if(shellCommandsTable[commandParsed.command].run != NULL){
                shellCommandsTable[commandParsed.command].run(&commandParsed);
            }
            else{
                runShellCommandUnknown(&commandParsed);
            }
        }
        else{
            /* Error parsing the Shell command*/
			printShellString((int8_t*)"\r\n", VERBOSE_ERROR);
			printCommonString(parseError, VERBOSE_ERROR);
        }
        i++;
    }
    printShellString((int8_t*)"\r\nSHELL-> \0", VERBOSE_EVENT);
}

#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
/*************************************************************************//**
 * @brief   Enables/Disables Verbose status of an HREG.
 *
 * @param	HREG.
 * @param	New Verbose status.
 *
 * @return  success.
 *
 ****************************************************************************/
eError shellSetHREGVerboseStatus(uint32_t regIndex, uint8_t instance, bool state )
{
    eError  success = RET_OK;

    if ( regIndex < NUM_OF_HREGS )
    {
        verboseHREGS[regIndex] = MON_SET_BIT(verboseHREGS[regIndex], instance, state);
    }
    else
    {
        success = RET_FAIL;
    }
    return success;
}

/*************************************************************************//**
 * @brief   Print HREG value if debug mode is active.
 *
 * @param	HREG.
 *
 * @return  Verbose State.
 *
 ****************************************************************************/
uint8_t shellEvaluateHREGVerboseStatus( uint16_t regIndex, uint8_t instance, uint32_t value )
{
    uint8_t  state = FALSE;

    if ( traceRegs == TRUE )
    {
	    state = MON_GET_BIT(verboseHREGS[regIndex], instance);
		if (state == TRUE)
		{
			printShellString((int8_t*)"rhreg ", VERBOSE_EVENT);
			printShellValue(regIndex, TYPE_DECIMAL, VERBOSE_EVENT);
			printShellString((int8_t*)" ", VERBOSE_EVENT);
			printShellValue(instance, TYPE_DECIMAL, VERBOSE_EVENT);
			printShellString((int8_t*)" ", VERBOSE_EVENT);
			printShellValue(value, TYPE_DECIMAL, VERBOSE_EVENT);
			printShellString((int8_t*)"\n", VERBOSE_EVENT);
		}
    }

    return state;
}


/*************************************************************************//**
 * @brief   Enables/Disables Verbose status of an SREG.
 *
 * @param	SREG.
 * @param	New Verbose status.
 *
 * @return  success.
 *
 ****************************************************************************/
eError shellSetSREGVerboseStatus(uint32_t regIndex, bool state )
{
    eError  success = RET_OK;

    if ( regIndex < NUM_OF_SREGS )
    {
        verboseSREGS[regIndex] = MON_SET_BIT(verboseHREGS[regIndex], 0, state);
    }
    else
    {
        success = RET_FAIL;
    }
    return success;
}

/*************************************************************************//**
 * @brief   Print SREG value if debug mode is active.
 *
 * @param	SREG.
 *
 * @return  Verbose State.
 *
 ****************************************************************************/
uint8_t shellEvaluateSREGVerboseStatus( uint16_t regIndex, uint32_t value )
{
    uint8_t  state = FALSE;

    if ( traceRegs == TRUE )
    {
		state = MON_GET_BIT(verboseSREGS[regIndex], 0);
		if (state == TRUE)
		{
			printShellString((int8_t*)"rsreg ", VERBOSE_EVENT);
			printShellValue(regIndex, TYPE_DECIMAL, VERBOSE_EVENT);
			printShellString((int8_t*)" ", VERBOSE_EVENT);
			printShellValue(value, TYPE_DECIMAL, VERBOSE_EVENT);
			printShellString((int8_t*)"\n", VERBOSE_EVENT);
        }
    }

    return state;
}
#endif // MON_SHELL_COMMAND_VERBOSE_REGS

/*************************************************************************//**
 * @brief   Return current debug state.
 *
 * @param   None.
 *
 * @return  Debug State.
 *
 ****************************************************************************/
BOOL shellGetDebugState( void )
{
    return debugEnable;
}

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
