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
#define MAX_NUM_OF_TRANSITIONS 20
//#define MAX_NUM_OF_STATES 1
//#define MAX_NUM_OF_EVENTS 20
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

	typedef struct sFsmTransition
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
		tFsmTransition transition[MAX_NUM_OF_TRANSITIONS];
	}tFsmState;

	typedef struct sFsm
	{
		uint8_t numFsm;
		uint8_t * name;
		const tFsmState * state;
		tFsmEvent * event;
		uint8_t numOfEvents;
		uint8_t numOfStates;
		uint8_t currentState;
		uint8_t initState;
		uint32_t tickCounter;
		tBool   logEnable;
	}tFsm;

/****************************************************************************
 *  PUBLIC FUNCTION PROTOTYPES
 ****************************************************************************/

eError initFsm( tFsm * fsm );
eError resetFsm( tFsm * fsm );
eError callFsmTask( tFsm * fsm );
eError announceFsmEvent(tFsm * fsm,uint8_t eventId );
uint8_t getFsmState(tFsm * fsm );
eError setLogFsm(tFsm * fsm,tBool value );

#endif /* _STATE_MACHINE_H_ */

/****************************************************************************
 *    End of file
 *    }@}@
 ***************************************************************************/
