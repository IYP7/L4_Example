/*****************************************************************************
* @file 	ATCommand_fsm.c
*
* @brief	ATCommand State Machine
*
* @author 	FV4GPD0
*
* @date 	05-Nov-18 12:06:40
 *
 ****************************************************************************/
#ifndef _ATCOMMAND_FSM_C_
#define _ATCOMMAND_FSM_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/
#include "hreg_dat.h"
/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "stateMachine.h"
#include "ATCommand_fsm.h"
/****************************************************************************
 *  EXTERN VARIABLES
 ****************************************************************************/

/****************************************************************************
 *  PRIVATE FUNCTIONS
 ****************************************************************************/

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/

static const tFsmState fsmState0 =
    {(uint8_t *)"AT_IDLE_STATE", NULL , NULL , 3,
         {
                /* TRANSITION  1 */    {AT_COMMAND_REQ_EVENT           , NULL                            , NULL                           , AT_WAIT_RESPONSE_STATE         },
                /* TRANSITION  2 */    {AT_RESPONSE_REC_EVENT          , NULL                            , &AtPublishURC                  , AT_IDLE_STATE                  },
                /* TRANSITION  3 */    {DO_EVENT                       , NULL                            , &AtCheckResponse               , NO_CHANGE                      },
         }
    };
static const tFsmState fsmState1 =
    {(uint8_t *)"AT_WAIT_RESPONSE_STATE", &AtSendCommand , NULL , 5,
         {
                /* TRANSITION  1 */    {AT_RESP_TIMEOUT_EVENT          , &AtCheckMaxRetries              , &AtReportTimeout               , AT_IDLE_STATE                  },
                /* TRANSITION  2 */    {AT_RESPONSE_REC_EVENT          , &AtCheckExpectedResponse        , &AtReportResponse              , AT_IDLE_STATE                  },
                /* TRANSITION  3 */    {AT_RESP_TIMEOUT_EVENT          , &AtCheckNoMaxRetries            , NULL                           , AT_WAIT_RESPONSE_STATE         },
                /* TRANSITION  4 */    {AT_RESPONSE_REC_EVENT          , &AtCheckNoExpectedResponse      , &AtPublishURC                  , AT_WAIT_RESPONSE_STATE         },
                /* TRANSITION  5 */    {DO_EVENT                       , NULL                            , &AtCheckResponse               , NO_CHANGE                      },
         }
    };


const tListFsmState fsmStateATCommand[NUM_OF_ATCOMMAND_FSM_STATES] =
{
          {&fsmState0},
          {&fsmState1},
};


tFsmEvent fsmEventATCommand[NUM_OF_ATCOMMAND_FSM_EVENTS] =
{
    {(uint8_t *)"AT_RESPONSE_REC_EVENT"                  , CALLBACK      ,0, FALSE},
    {(uint8_t *)"AT_RESP_TIMEOUT_EVENT"                  , CALLBACK      ,0, FALSE},
    {(uint8_t *)"AT_COMMAND_REQ_EVENT"                   , CALLBACK      ,0, FALSE},
};



#endif // ATCOMMAND_FSM_C_