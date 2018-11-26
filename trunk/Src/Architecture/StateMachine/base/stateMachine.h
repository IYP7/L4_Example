/*************************************************************************//**
 * @file StateMachine.h
 *
 * @author Dario Davide
 *
 * @date 10/11/2016
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup State Machine
 * @{
 *
 ****************************************************************************/

#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDE
 ***************************************************************************/

/****************************************************************************
 *  DEFINES
 ****************************************************************************/
#define MAX_NUM_OF_FSM		   20
#define  DO_EVENT			   255
#define  NO_CHANGE			   255
#define FSM_TRACE_EVENT  0
#define FSM_TRACE_TRANSITION  1
#define FSM_LOG  1
#define FSM_TRACE  2

/****************************************************************************
 *  TYPES DEFINITION
 ****************************************************************************/
typedef enum eFsmTypeEvent
{
	ANY,
	TIMING,
	CALLBACK,
}tFsmTypeEvent;

typedef struct sFsmEvent
{
	uint8_t * EventName;
	tFsmTypeEvent	type;
	uint32_t 	timeOut;
	tBool 		value;
}tFsmEvent;

typedef struct  __attribute__ ((packed)) sFsmTransition
{
	uint8_t eventId;
	tBool (* condition)(uint8_t);
	eError (* action)(uint8_t);
	uint8_t destination;

}tFsmTransition;

typedef struct sFsmState
{
	uint8_t * StateName;
	eError ( * entryAction)(uint8_t);
	eError ( * exitAction)(uint8_t);
	uint8_t numOfTransition;
	tFsmTransition transition[];
}tFsmState;

typedef struct sListFsmState
{
	const tFsmState * state;
}tListFsmState;

typedef struct sFsm
{
	uint8_t numFsm;
	uint8_t * name;
	const tListFsmState * listState;
	tFsmEvent * event;
	uint8_t numOfEvents;
	uint8_t numOfStates;
	uint8_t currentState;
	uint8_t initState;
	uint32_t tickCounter;
	uint8_t logEnable;
}tFsm;


typedef struct sFsmContext
{
	tFsm * 	fsm;
	tBool 	configured;
	tBool 	started;
	tBool	newEvent;
	uint16_t loopTiming;
	uint32_t timeElapsed;

}tFsmContext;

/****************************************************************************
 *  PUBLIC FUNCTION PROTOTYPES
 ****************************************************************************/

void initFsm( void );
void callFsmTask( void );

uint8_t configureFsm( tFsm * fsm,uint16_t loopTiming );
eError  resetFsm( uint8_t instance );
eError  stopFsm( uint8_t instance );
eError  announceFsmEvent( uint8_t instance,uint8_t eventId );
uint8_t getFsmState( uint8_t instance );
eError  setLogFsm(uint8_t instance,tBool value );
uint32_t getFsmContextPtr( uint8_t* maxNumOfFSM, uint8_t* sizeContext );

#endif /* _STATE_MACHINE_H_ */

/****************************************************************************
 *    End of file
 *    }@}@
 ***************************************************************************/
