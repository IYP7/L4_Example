/*****************************************************************************
* @file 	ATCommand_fsm.h
*
* @brief	ATCommand State Machine
*
* @author 	FV4GPD0
*
* @date 	05-Nov-18 12:06:40
 *
 ****************************************************************************/
#ifndef _ATCOMMAND_FSM_H_
#define _ATCOMMAND_FSM_H_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"
/****************************************************************************
 *  DEFINES
 ****************************************************************************/
#define FSM_ATCOMMAND_FSM_LOOP_TIME 50
/****************************************************************************
 *  TYPES DEFINITION
 ****************************************************************************/

typedef enum eATCommandStates
{
		AT_IDLE_STATE, 
		AT_WAIT_RESPONSE_STATE, 
		NUM_OF_ATCOMMAND_FSM_STATES,
}tATCommandStates;


typedef enum eATCommandEvents
{
		AT_RESPONSE_REC_EVENT,
		AT_RESP_TIMEOUT_EVENT,
		AT_COMMAND_REQ_EVENT,
		NUM_OF_ATCOMMAND_FSM_EVENTS,
}tATCommandEvents;



/****************************************************************************
 *    VARIABLES
 ****************************************************************************/

 /****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/


 /* GUARD FUNCTION */

tBool AtCheckMaxRetries(uint8_t );
tBool AtCheckExpectedResponse(uint8_t );
tBool AtCheckNoMaxRetries(uint8_t );
tBool AtCheckNoExpectedResponse(uint8_t );

 /* ACTION FUNCTION */

eError AtPublishURC(uint8_t );
eError AtCheckResponse(uint8_t );
eError AtSendCommand(uint8_t );
eError AtReportTimeout(uint8_t );
eError AtReportResponse(uint8_t );


#endif // ATCOMMAND_FSM_H_
/****************************************************************************
 *    End of file
 ***************************************************************************/
