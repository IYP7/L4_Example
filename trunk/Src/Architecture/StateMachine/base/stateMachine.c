/*************************************************************************//**
 * @file stateMachine.c
 * @date 10/02/2017
 * @Author Dario Davide
 *
 * @addtogroup Arquitecture
 * @{
 * @addtogroup Arquitecture
 * @{
 *
 * @brief This module provide a engine to execute a list of finite-state-machine *
 * The basic architecture of system contains:
 * 1)	Profile : Builds a complete movement profile for doing a movement, taking into
 *		Account the internal parameters (acceleration, jerk, etc...). In the configuration
 *		phase is possible choice between two types of profile, sharp (trapezoidal)
 *		and smooth (parabolic)
 *
 * 2)	Servo iterations: the motor will follow the section defined by the movement.
 *
 * 3)	States : The Subsystem support the following predefined states :
 *			a.	STATE_OFF : Motor is stopped
 *			b.	STATE_ACCEL : Motor is accelerating
 *			c.	STATE_SLEW : Motor is moving at a constant speed
 *			d.	STATE_DECEL : Motor is decelerating
 *			e.	STATE_HOLD : Motor is maintaining the target position
 *			f.	STATE_HALT : Motor is being stopped abruptly
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
static eError startFsm( uint8_t instance);
static eError executeFsm(uint8_t instance );

static tBool checkCondition( tFsm * fsm,uint8_t trans);
static tBool checkEvent( uint8_t instance,uint8_t eventId);
static eError checkAction (tFsm * fsm,uint8_t trans);
static tBool checkDestination(uint8_t instance,uint8_t stateDestination,uint8_t transition);
static void checkExitAction(tFsm * fsm);
static void checkEntryAction(tFsm * fsm);
static void clearEvents(tFsm * fsm);
static void clearEvent(tFsm * fsm,uint8_t event_id);
static void checkTimingEvents(uint8_t instance);
/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/

static tFsmContext fsmContext[MAX_NUM_OF_FSM];

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/

/*****************************************************************************
 * @brief init state Machine module.
 ****************************************************************************/
void initFsm( void )
{
	uint8_t var;

	for (var = 0; var < MAX_NUM_OF_FSM; ++var) {

		fsmContext[var].configured = FALSE;
		fsmContext[var].loopTiming = 0;
		fsmContext[var].newEvent = FALSE;
		fsmContext[var].started = FALSE;
		fsmContext[var].timeElapsed = 0;

	}
}

/*****************************************************************************
 * @brief task for state Machine module. executed by SCheduler
 ****************************************************************************/
void callFsmTask( void )
{

	uint8_t var;

	for (var = 0; var < MAX_NUM_OF_FSM; ++var)
	{
		if (fsmContext[var].configured && fsmContext[var].started)
		{
			fsmContext[var].timeElapsed ++;
			checkTimingEvents(var);
			if ((fsmContext[var].timeElapsed%fsmContext[var].loopTiming)==0 || fsmContext[var].newEvent)
			{
				fsmContext[var].newEvent = FALSE;
				executeFsm(var);
			}
		}
	}
}

/*****************************************************************************
 * @brief Configure state Machine. The function has been called after to initFsm function
 * @param fsm: 			object pointer to the C-generated fsm structure.
 * @param loopTiming :	Refresh Time expressed in ms
 * @return indexFSm  :  index of state Machine
 ****************************************************************************/
uint8_t configureFsm( tFsm * fsm,uint16_t loopTiming )
{
	eError ret = RET_FAIL;
	uint8_t var =0;
	tBool found = FALSE;

	while (var<MAX_NUM_OF_FSM && !found)
	{
		found = !fsmContext[var].configured;
		var++;
	}
	var--;
	if (found)
	{
		fsmContext[var].fsm = fsm;
		fsmContext[var].configured = TRUE;
		fsmContext[var].loopTiming = loopTiming;
		ret = RET_OK;
	}
	if (ret!=RET_OK)
	{
		var = 255;
	}

	return var;
}

/*****************************************************************************
 * @brief Reset state-Machine
 * @param instance of fsm
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
eError resetFsm( uint8_t instance )
{
	eError ret = RET_FAIL;
	if (fsmContext[instance].configured)
	{
		clearEvents(fsmContext[instance].fsm);
		ret = startFsm(instance);
	}

	return ret;
}

/*****************************************************************************
 * @brief Stop state-Machine
 * @param instance of fsm
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
eError stopFsm( uint8_t instance )
{
	eError ret = RET_OK;
	if (fsmContext[instance].configured)
	{
		fsmContext[instance].started = FALSE;
		my_printf((fsmContext[instance].fsm->logEnable & FSM_LOG) == FSM_LOG,VERBOSE_GENERAL, "FSM : %s num : %d Stop Fsm %s.\n",fsmContext[instance].fsm->name,fsmContext[instance].fsm->numFsm);
	}

	return ret;
}

/*****************************************************************************
 * @brief send an asyncronous event
 * @param instance of fsm
 * @param Event Id
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
eError announceFsmEvent( uint8_t instance,uint8_t eventId )
{
	eError retValue = RET_FAIL;

	if (fsmContext[instance].configured && fsmContext[instance].started && fsmContext[instance].fsm->event[eventId].type==CALLBACK)
	{
		fsmContext[instance].fsm->event[eventId].value = TRUE;
		fsmContext[instance].newEvent = TRUE;
		my_printf((fsmContext[instance].fsm->logEnable & FSM_LOG) == FSM_LOG,VERBOSE_GENERAL, "FSM %s num : %d announce event %s.\n",fsmContext[instance].fsm->name,fsmContext[instance].fsm->numFsm,fsmContext[instance].fsm->event[eventId].EventName);

		if ((fsmContext[instance].fsm->logEnable & FSM_TRACE) == FSM_TRACE)
		{
			uint8_t buffer[4];
			BufferSetData8(buffer, 0, instance);
			BufferSetData8(buffer, 1, FSM_TRACE_EVENT);
			BufferSetData8(buffer, 2, fsmContext[instance].fsm->currentState);
			BufferSetData8(buffer, 3, eventId);
			my_printTraceBin(FSM_PARAMETERS*10 + PROTOCOL_DEVICE_INSTANCE,buffer,4);
		}
		retValue = RET_OK;
	}

	return retValue;
}

/*****************************************************************************
 * @brief  Get the current state of fsm
 * @param  instance of fsm
 * @return stateId the identifier of the state.
 ****************************************************************************/
uint8_t getFsmState( uint8_t instance )
{
	uint8_t state = 255;
	if (fsmContext[instance].configured && fsmContext[instance].started)
	{
		my_printf((fsmContext[instance].fsm->logEnable & FSM_LOG) == FSM_LOG,VERBOSE_GENERAL, "FSM : %s num : %d get State : %s.\n",fsmContext[instance].fsm->name,fsmContext[instance].fsm->numFsm,fsmContext[instance].fsm->listState[fsmContext[instance].fsm->currentState].state->StateName);
		state = fsmContext[instance].fsm->currentState;
	}
	return state;
}

/*****************************************************************************
 * @brief  Enable logging
 * @param  instance of fsm
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
eError setLogFsm(uint8_t instance,tBool value )
{
	eError ret = RET_FAIL;

	if (fsmContext[instance].configured)
	{
		fsmContext[instance].fsm->logEnable = value;
		ret = RET_OK;
	}

	return ret;
}

/*****************************************************************************
 * @brief  Returns the FSM Context Ptr
 * @param  instance of fsm
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
uint32_t getFsmContextPtr( uint8_t* maxNumOfFSM, uint8_t* sizeContext )
{
	*maxNumOfFSM = MAX_NUM_OF_FSM;
	*sizeContext = (sizeof(fsmContext)/sizeof(tFsmContext));
	return (uint32_t)(&fsmContext[0]);
}


/****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/

/*****************************************************************************
 * @brief  Start Fsm, set the start flag to execute the fsm by calltaskfsm
 * @param  instance of fsm
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
static eError startFsm( uint8_t instance)
{
	fsmContext[instance].fsm->currentState = fsmContext[instance].fsm->initState;
	fsmContext[instance].timeElapsed = 0;

	my_printf((fsmContext[instance].fsm->logEnable & FSM_LOG) == FSM_LOG,VERBOSE_GENERAL, "FSM : %s num : %d init setToState %s.\n",fsmContext[instance].fsm->name,fsmContext[instance].fsm->numFsm,fsmContext[instance].fsm->listState[fsmContext[instance].fsm->currentState].state->StateName);

	fsmContext[instance].started = TRUE;

	checkEntryAction(fsmContext[instance].fsm);

	return RET_OK;
}

/*****************************************************************************
 * @brief  execute Fsm, analyze the transitions for the current state :
 * 			1) check if the linked EVENT has been announced
 * 			2) call the GUARD function and check the result if the before check is TRUE
 * 			3) call the ACTION function if the response of guard function is TRUE
 * 			4) call the changeState function if the action function has been executed properly
 * @param  instance of fsm
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
static eError executeFsm(uint8_t instance )
{
	//Check if there is any event
	uint8_t i =0,currentState;
	tBool changeState = FALSE;

	currentState = fsmContext[instance].fsm->currentState;

	while (i<fsmContext[instance].fsm->listState[currentState].state->numOfTransition && !changeState)
	{
		if (checkEvent(instance,fsmContext[instance].fsm->listState[currentState].state->transition[i].eventId))
		{
			if (checkCondition(fsmContext[instance].fsm,i))
			{
				eError retAction = checkAction(fsmContext[instance].fsm,i);

				if (retAction == RET_OK)
				{
					changeState = checkDestination(instance,fsmContext[instance].fsm->listState[currentState].state->transition[i].destination,i);
				} else
				{
					clearEvent(fsmContext[instance].fsm,fsmContext[instance].fsm->listState[currentState].state->transition[i].eventId);
				}


			}
		}
		i++;
	}

	return RET_OK;
}

/*****************************************************************************
 * @brief  call the guard condition linked to current transition examined
 * @param  instance of fsm
 * @param  index of transition
 * @return TRUE or FALSE
 ****************************************************************************/
static tBool checkCondition( tFsm * fsm,uint8_t trans)
{
	tBool value = TRUE;

	if ( fsm->listState[fsm->currentState].state->transition[trans].condition != NULL)
	{
		uint8_t numFsm = fsm->numFsm;
		value = fsm->listState[fsm->currentState].state->transition[trans].condition(numFsm);
	}


	return value;
}

/*****************************************************************************
 * @brief  return if the event, linked to current transition examined, has been announced
 * @param  instance of fsm
 * @param  event ID
 * @return TRUE or FALSE
 ****************************************************************************/
static tBool checkEvent( uint8_t instance,uint8_t eventId)
{
	tBool value = FALSE;
	if (eventId < fsmContext[instance].fsm->numOfEvents)
	{
		switch (fsmContext[instance].fsm->event[eventId].type)
		{
			case ANY:
				value = TRUE;
				break;
			case TIMING:
				value = fsmContext[instance].fsm->event[eventId].value;
				break;
			case CALLBACK:
				value = fsmContext[instance].fsm->event[eventId].value;
				break;
			default:
				break;
		}
	} else if(eventId == DO_EVENT)
	{
		if(((fsmContext[instance].timeElapsed%fsmContext[instance].loopTiming)==0) || (fsmContext[instance].timeElapsed == 1))
		{
			value = TRUE;
		}
	}
	return value;
}

/*****************************************************************************
 * @brief  call the action function linked to current transition examined
 * @param  instance of fsm
 * @param  index of transition
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
static eError checkAction (tFsm * fsm,uint8_t trans)
{
	eError retValue = RET_OK;

	if ( fsm->listState[fsm->currentState].state->transition[trans].action != NULL)
	{
		uint8_t numFsm = fsm->numFsm;
		retValue = fsm->listState[fsm->currentState].state->transition[trans].action(numFsm);
	}

	return retValue;
}

/*****************************************************************************
 * @brief  change the current state of fsm :
 * 			1) call the exit action function linked to current state
 * 			2) set the current state with the destination state
 * 			3) call the entry action function linked to new current state
 * @param  instance of fsm
 * @param  index of transition
 * @return TRUE or FALSE
 ****************************************************************************/
static tBool checkDestination(uint8_t instance,uint8_t stateDestination,uint8_t transition)
{
	tBool value = FALSE;
	if (stateDestination < fsmContext[instance].fsm->numOfStates)
	{
		checkExitAction(fsmContext[instance].fsm);
		fsmContext[instance].timeElapsed = 0;
		fsmContext[instance].newEvent = TRUE;
		clearEvents(fsmContext[instance].fsm);
		if ((fsmContext[instance].fsm->logEnable & FSM_TRACE) == FSM_TRACE)
		{
			uint8_t buffer[5];
			BufferSetData8(buffer, 0, instance);
			BufferSetData8(buffer, 1, FSM_TRACE_TRANSITION);
			BufferSetData8(buffer, 2, fsmContext[instance].fsm->currentState);
			BufferSetData8(buffer, 3, transition);
			BufferSetData8(buffer, 4, stateDestination);
			my_printTraceBin(FSM_PARAMETERS*10 + PROTOCOL_DEVICE_INSTANCE,buffer,5);
		}

		fsmContext[instance].fsm->currentState = stateDestination;
		my_printf((fsmContext[instance].fsm->logEnable & FSM_LOG) == FSM_LOG,VERBOSE_GENERAL, "FSM %s num : %d Transition %d change State %s.\n",fsmContext[instance].fsm->name,fsmContext[instance].fsm->numFsm,transition,fsmContext[instance].fsm->listState[stateDestination].state->StateName);
		checkEntryAction(fsmContext[instance].fsm);
		value = TRUE;
	}

	return value;
}

/*****************************************************************************
 * @brief  call the action function linked to exit action of the state
 * @param  instance of fsm
 ****************************************************************************/
static void checkExitAction(tFsm * fsm)
{
	if ( fsm->listState[fsm->currentState].state->exitAction != NULL)
	{
		uint8_t numFsm = fsm->numFsm;
		fsm->listState[fsm->currentState].state->exitAction(numFsm);
	}


}
/*****************************************************************************
 * @brief  call the action function linked to entry action of the state
 * @param  instance of fsm
 ****************************************************************************/
static void checkEntryAction(tFsm * fsm)
{
	if ( fsm->listState[fsm->currentState].state->entryAction != NULL)
	{
		uint8_t numFsm = fsm->numFsm;
		fsm->listState[fsm->currentState].state->entryAction(numFsm);
	}

}

/*****************************************************************************
 * @brief  clear the list of the events
 * @param  instance of fsm
 ****************************************************************************/
static void clearEvents(tFsm * fsm)
{
	uint8_t i;

	for (i=0;i<fsm->numOfEvents;i++)
	{
		fsm->event[i].value = FALSE;
	}

}

/*****************************************************************************
 * @brief  clear a specific event
 * @param  instance of fsm
 * @param  Event Id
 ****************************************************************************/
static void clearEvent(tFsm * fsm,uint8_t event_id)
{
	fsm->event[event_id].value = FALSE;

}

/*****************************************************************************
 * @brief  The algorithm evaluates the Time Events to check if the timeOut expired
 * @param  instance of fsm
 ****************************************************************************/
static void checkTimingEvents(uint8_t instance)
{
	uint8_t var;
	for (var = 0; var < fsmContext[instance].fsm->numOfEvents; ++var)
	{
		if (fsmContext[instance].fsm->event[var].type == TIMING)
		{
			fsmContext[instance].fsm->event[var].value = ((fsmContext[instance].timeElapsed%fsmContext[instance].fsm->event[var].timeOut)==0);
			fsmContext[instance].newEvent |= fsmContext[instance].fsm->event[var].value;
		}
	}

}
/****************************************************************************
 *    End of file
 *    }@}@
 ***************************************************************************/
