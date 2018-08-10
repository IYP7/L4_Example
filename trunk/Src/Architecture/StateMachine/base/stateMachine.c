/*************************************************************************//**
 * @file StateMachine.c
 * @date 10/11/2016
 *
 *
 * @author Dario Davide
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup State Machine
 * @{
 *
 *
 ****************************************************************************/

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDE
 ***************************************************************************/
/****************************************************************************
 *    INCLUDE
 ****************************************************************************/

#include "stateMachine.h"

/****************************************************************************
 *    CONSTANTS
 ****************************************************************************/

/****************************************************************************
 *  PRIVATE FUCNTION PROTOTYPE
 ***************************************************************************/
static tBool checkCondition( tFsm * fsm,uint8_t trans);
static tBool checkEvent( tFsm * fsm,uint8_t eventId);
static eError checkAction (tFsm * fsm,uint8_t trans);
static tBool checkDestination(tFsm * fsm,uint8_t stateDestination);
static void checkExitAction(tFsm * fsm);
static void checkEntryAction(tFsm * fsm);
static void clearEvents(tFsm * fsm);
static void clearEvent(tFsm * fsm,uint8_t event_id);
/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/


/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/


eError initFsm( tFsm * fsm )
{
	fsm->currentState = fsm->initState;
	fsm->tickCounter = 0;

	my_printf(fsm->logEnable,VERBOSE_GENERAL, "FSM : %s num : %d init setToState %s.\n",fsm->name,fsm->numFsm,fsm->state[fsm->currentState].StateName);

	checkEntryAction(fsm);

	return RET_OK;
}

eError resetFsm( tFsm * fsm )
{
	clearEvents(fsm);
	return initFsm(fsm);
}

eError callFsmTask( tFsm * fsm )
{
	//Check if there is any event
	uint8_t i =0,currentState;
	tBool changeState = FALSE;

	fsm->tickCounter++;
	currentState = fsm->currentState;

	while (i<fsm->state[currentState].numOfTransition && !changeState)
	{
		if (checkEvent(fsm,fsm->state[currentState].transition[i].eventId))
		{
			if (checkCondition(fsm,i))
			{
				eError retAction = checkAction(fsm,i);

				if (retAction == RET_OK)
				{
					changeState = checkDestination(fsm,fsm->state[currentState].transition[i].destination);
				} else
				{
					clearEvent(fsm,fsm->state[currentState].transition[i].eventId);
				}


			}
		}
		i++;
	}

	return RET_OK;
}

eError announceFsmEvent(tFsm * fsm,uint8_t eventId )
{
	eError retValue = RET_OK;

	if (fsm->event[eventId].type==CALLBACK)
	{
		fsm->event[eventId].value = TRUE;
	}
	else
	{
		retValue = RET_FAIL;
	}

	my_printf(fsm->logEnable,VERBOSE_GENERAL, "FSM %s num : %d announce event %s.\n",fsm->name,fsm->numFsm,fsm->event[eventId].EventName);


	return retValue;
}

uint8_t getFsmState(tFsm * fsm )
{
	return fsm->currentState;
}

eError setLogFsm(tFsm * fsm,tBool value )
{
	fsm->logEnable = value;

	return RET_OK;
}

/****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/

static tBool checkCondition( tFsm * fsm,uint8_t trans)
{
	tBool value = TRUE;

	if ( fsm->state[fsm->currentState].transition[trans].condition != NULL)
	{
		uint8_t numFsm = fsm->numFsm;
		value = fsm->state[fsm->currentState].transition[trans].condition(numFsm);
	}


	return value;
}

static tBool checkEvent( tFsm * fsm,uint8_t eventId)
{
	tBool value = FALSE;
	if (eventId < fsm->numOfEvents)
	{
		switch (fsm->event[eventId].type)
		{
			case ANY:
				value = TRUE;
				break;
			case TIMING:
				value = ((fsm->tickCounter%fsm->event[eventId].timeOut)==0);
				break;
			case CALLBACK:
				value = fsm->event[eventId].value;
				//fsm->event[eventId].value = FALSE;
				break;
			default:
				break;
		}
	}
	return value;
}

static eError checkAction (tFsm * fsm,uint8_t trans)
{
	eError retValue = RET_OK;

	if ( fsm->state[fsm->currentState].transition[trans].action != NULL)
	{
		uint8_t numFsm = fsm->numFsm;
		retValue = fsm->state[fsm->currentState].transition[trans].action(numFsm);
	}

	return retValue;
}

static tBool checkDestination(tFsm * fsm,uint8_t stateDestination)
{
	tBool value = FALSE;
	if (stateDestination < fsm->numOfStates)
	{
		checkExitAction(fsm);
		fsm->tickCounter = 0;
		clearEvents(fsm);
		fsm->currentState = stateDestination;
		checkEntryAction(fsm);
		value = TRUE;

		my_printf(fsm->logEnable,VERBOSE_GENERAL, "FSM %s num : %d change State %s.\n",fsm->name,fsm->numFsm,fsm->state[stateDestination].StateName);

	}

	return value;
}

static void checkExitAction(tFsm * fsm)
{
	if ( fsm->state[fsm->currentState].exitAction != NULL)
	{
		uint8_t numFsm = fsm->numFsm;
		fsm->state[fsm->currentState].exitAction(numFsm);
	}

}

static void checkEntryAction(tFsm * fsm)
{
	if ( fsm->state[fsm->currentState].entryAction != NULL)
	{
		uint8_t numFsm = fsm->numFsm;
		fsm->state[fsm->currentState].entryAction(numFsm);
	}

}

static void clearEvents(tFsm * fsm)
{
	uint8_t i;

	for (i=0;i<fsm->numOfEvents;i++)
	{
		fsm->event[i].value = FALSE;
	}

}

static void clearEvent(tFsm * fsm,uint8_t event_id)
{
	fsm->event[event_id].value = FALSE;

}
/****************************************************************************
 *    End of file
 *    }@}@
 ***************************************************************************/
