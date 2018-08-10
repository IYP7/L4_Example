/***********************************************************************
 * @file        Timer.h
 *
 * @brief       ...
 *
 * @author      Jose Antonio Garcia Peiro
 *
 * @date        DD-MM-2012
 *
 **********************************************************************/
#ifndef __SWTIMER_H__
#define __SWTIMER_H__

/**********************************************************************
 *  COMMON INCLUDE
 **********************************************************************/
#include "common.h"

/**********************************************************************
 *  ARCHITECTURE INCLUDES
 **********************************************************************/

/**********************************************************************
 * PUBLIC INCLUDES
 **********************************************************************/
#include "SwTimer_map.h"

/***********************************************************************
 * PUBLIC DEFINES
 **********************************************************************/
#define	TIMER_NAME_MAX		 TIMER_INDEX_MAX

/***********************************************************************
 * PUBLIC TYPES
 **********************************************************************/
typedef enum eTimerState{
	SW_TIMER_STATE_STOPPED = 0,
	SW_TIMER_STATE_STARTED,
	SW_TIMER_STATE_CANCELLED,
	SW_TIMER_STATE_EXPIRED,

	SW_TIMER_NUM_OF_STATES
} tSwTimerState;

typedef struct sSwTimer{
	uint32_t startMs;
	uint32_t timeoutMs;
	tSwTimerState state;
	tBool (*pFuncCB)(tSwTimerIndex SwTimerIndex);
} tSwTimer;


/***********************************************************************
 * PUBLIC VARIABLES DECLARATIONS
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUNCTIONS DECLARATIONS
 **********************************************************************/
eError SwTimerInit( void );
eError SwTimerStart( void );
eError SwTimerStop( void );
eError SwTimerSleep( void );
eError SwTimerWake( void );

eError SwTimerStartTimer( tSwTimerIndex SwTimerIndex );
eError SwTimerStopTimer( tSwTimerIndex SwTimerIndex );
eError SwTimerResetTimer( tSwTimerIndex SwTimerIndex );
eError SwTimerUpdateTimer( tSwTimerIndex SwTimerIndex );
eError SwTimerSetTimeout( tSwTimerIndex SwTimerIndex, uint32_t timeoutMs );
uint32_t SwTimerGetElapsed( tSwTimerIndex SwTimerIndex );
uint32_t SwTimerGetPending( tSwTimerIndex SwTimerIndex );
tSwTimerState SwTimerGetState( tSwTimerIndex SwTimerIndex  );
eError SwTimerSetState( tSwTimerIndex SwTimerIndex, tSwTimerState state );
tBool SwTimerIsExpired( tSwTimerIndex SwTimerIndex );
eError SwTimerSetCallbackFunction( tSwTimerIndex SwTimerIndex, tBool (*callbackFunction)(tSwTimerIndex SwTimerIndex) );

void SwTimerIrq( void );

#endif  /*_SWTIMER_H_*/
