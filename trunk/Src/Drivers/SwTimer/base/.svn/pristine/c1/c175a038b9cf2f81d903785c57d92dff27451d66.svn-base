/**********************************************************************
 * @file        Timer.c
 *
 * @brief       ...
 *
 * @author      Jose Antonio Garcia Peiro
 *
 * @date        DD-MM-2012
 *
 **********************************************************************/
#ifndef _SWTIMER_C_
#define _SWTIMER_C_

/**********************************************************************
 *  COMMON INCLUDE
 **********************************************************************/
#include "common.h"

/**********************************************************************
 *  ARCHITECTURE INCLUDES
 **********************************************************************/

/**********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "SwTimer.h"

/**********************************************************************
 * PRIVATE DEFINES
 **********************************************************************/

/**********************************************************************
 * PRIVATE TYPES
 **********************************************************************/

/**********************************************************************
 * PRIVATE VARIABLES DEFINITIONS
 **********************************************************************/
// Global time variable.
static uint32_t timeMs = 0;

// Timers array.
static tSwTimer SwTimersArray[NUM_OF_SW_TIMER];

/**********************************************************************
 * PRIVATE FUNCTIONS DECLARATIONS
 **********************************************************************/
static uint32_t diffTime( uint32_t timeA, uint32_t timeB );
static uint32_t getTime( void );

/**********************************************************************
 * PRIVATE FUNCTIONS DEFINITIONS
 **********************************************************************/

/**********************************************************************
 * @brief.	Compute the time difference, with preventing variable EXPIREDflow.
 * @param	timeA:	Start time.
 * @param	timeB:	Stop time.
 * @retval	Elapsed time from timeA to timeB.
 **********************************************************************/
static uint32_t diffTime( uint32_t timeA, uint32_t timeB )
{
    uint32_t retVal = 0;

    if( timeA >= timeB )
    {
            retVal = timeA - timeB;
    }
    else
    {
            retVal = UINT32_MAX - (timeB - timeA);
    }

    return retVal;
}

/**********************************************************************
 * @brief  Get global time in milliseconds.
 * @param
 * @retval Time in milliseconds since the systick has been initialized.
 **********************************************************************/
static uint32_t getTime( void )
{
    uint32_t retVal = 0;

    retVal = timeMs;

    return retVal;
}

/**********************************************************************
 * PUBLIC VARIABLES DEFINITIONS
 **********************************************************************/

/**********************************************************************
 * PUBLIC FUNCTIONS DEFINITIONS
 **********************************************************************/

/**********************************************************************
 * @brief  Initializes timer driver.
 * @param
 * @retval
 **********************************************************************/
eError SwTimerInit( void )
{
    eError result = RET_OK;
    uint8_t i;

    for( i = 0 ; i < NUM_OF_SW_TIMER ; i++ )
    {
        SwTimersArray[i].state = SW_TIMER_STATE_STOPPED;
    }

    return result;
}


/**********************************************************************
 * @brief  Starts the timer driver
 * @param
 * @retval
 **********************************************************************/
eError SwTimerStart()
{
    eError result = RET_OK;

    return result;
}

/**********************************************************************
 * @brief  Stops the timer Driver
 * @param
 * @retval
 **********************************************************************/
eError SwTimerStop()
{
    eError result = RET_OK;

    return result;
}

/**********************************************************************
 * @brief  Sleeps the timer Driver
 * @param
 * @retval
 **********************************************************************/
eError SwTimerSleep()
{
    eError result = RET_OK;

    return result;
}

/**********************************************************************
 * @brief  Wakes up the timer Driver
 * @param
 * @retval
 **********************************************************************/
eError SwTimerWake()
{
    eError result = RET_OK;

    return result;
}


/**********************************************************************
 * @brief  Start selected timer
 * @param  pTimer:	Pointer to timer to be started.
 * @retval
 **********************************************************************/
eError SwTimerStartTimer(tSwTimerIndex SwTimerIndex)
{
    eError result = RET_OK;

    // If timer has been cancelled, reset timer
    if( SwTimersArray[SwTimerIndex].state == SW_TIMER_STATE_CANCELLED ||
        SwTimersArray[SwTimerIndex].state == SW_TIMER_STATE_EXPIRED )
    {
        SwTimerResetTimer( SwTimerIndex );
    }

    // Change timer state.
    SwTimersArray[SwTimerIndex].state = SW_TIMER_STATE_STARTED;
    
    return result;
}

/**********************************************************************
 * @brief  Stops selected timer
 * @param  pTimer:	Pointer to timer to be stopped.
 * @retval
 **********************************************************************/
eError SwTimerStopTimer( tSwTimerIndex SwTimerIndex )
{
    eError result = RET_OK;

    // Change timer state.
    SwTimersArray[SwTimerIndex].state = SW_TIMER_STATE_STOPPED;
    
    return result;
}

/**********************************************************************
 * @brief  Reset and start the timer
 * @param  pTimer:	Pointer to timer to be restart.
 **********************************************************************/
eError SwTimerResetTimer( tSwTimerIndex SwTimerIndex )
{
    eError result = RET_OK;

    // Reset the timer.
    SwTimersArray[SwTimerIndex].startMs = getTime();

    return result;
}

/**********************************************************************
 * @brief  Updates all timers every millisecond
 * @param
 * @retval
 **********************************************************************/
eError SwTimerUpdateTimer( tSwTimerIndex SwTimerIndex )
{
    eError result = RET_OK;
    tBool restartTimer = FALSE;

    // If timer is started, check timeout.
    if( SwTimersArray[SwTimerIndex].state == SW_TIMER_STATE_STARTED )
    {
        // Compute elapsed time
        if( diffTime( timeMs, SwTimersArray[SwTimerIndex].startMs ) >= SwTimersArray[SwTimerIndex].timeoutMs )
        {
            // If elapsed time is greater or equal to timer timeout,
            // execute timer callback and change timer state.
            SwTimersArray[SwTimerIndex].state = SW_TIMER_STATE_EXPIRED;
            if( NULL != SwTimersArray[SwTimerIndex].pFuncCB )
            {
                    restartTimer = SwTimersArray[SwTimerIndex].pFuncCB(SwTimerIndex);
            }
        }
    }
    // If the timer is stopped, simply shifts the start time,
    // this keeps the elapsed time.
    else if( SwTimersArray[SwTimerIndex].state == SW_TIMER_STATE_STOPPED )
    {
        SwTimersArray[SwTimerIndex].startMs++;
    }

    if ( restartTimer == TRUE )
    {
        SwTimerStartTimer(SwTimerIndex);
    }

    return result;
}

/**********************************************************************
 * @brief  Cancels selected timer
 * @param  pTimer:	Pointer to timer to be started.
 **********************************************************************/
eError SwTimerCancelTimer( tSwTimerIndex SwTimerIndex )
{
    eError result = RET_OK;

    // Change timer state.
    SwTimersArray[SwTimerIndex].state = SW_TIMER_STATE_CANCELLED;
    
    return result;
}

/**********************************************************************
 * @brief  Set timer timeout.
 * @param  pTimer:	Pointer to timer to be used.
 * @param  timeoutMs:	New timer timeout value in milliseconds
 **********************************************************************/
eError SwTimerSetTimeout( tSwTimerIndex SwTimerIndex, uint32_t timeoutMs )
{
    eError result = RET_OK;
    // Reset the timer.
    //timersArray[timerIndex].startMs = getTime();

    // Set timer timeout
    SwTimersArray[SwTimerIndex].timeoutMs = TIME_TO_TICKS(timeoutMs);
    return result ;
}

/**********************************************************************
 * @brief  Get timer elapsed time
 * @param  pTimer:	Pointer to timer to be used.
 * @retval			Elapsed timer in milliseconds
 **********************************************************************/
uint32_t SwTimerGetElapsed( tSwTimerIndex SwTimerIndex )
{
    uint32_t retVal = 0;

    // Compute elapsed time.
    retVal = diffTime( getTime(), SwTimersArray[SwTimerIndex].startMs );

    return retVal;
}

/**********************************************************************
 * @brief  Get timer pending time
 * @param  pTimer:	Pointer to timer to be used.
 * @retval			Pending timer in milliseconds
 **********************************************************************/
uint32_t SwTimerGetPending( tSwTimerIndex SwTimerIndex )
{
    uint32_t retVal = 0;

    // Compute elapsed time.
    if( SwTimersArray[SwTimerIndex].state == SW_TIMER_STATE_STARTED )
    {
        retVal = SwTimersArray[SwTimerIndex].timeoutMs - diffTime( getTime(), SwTimersArray[SwTimerIndex].startMs );
    }
    else
    {
        retVal = 0;
    }

    return retVal;
}


/**********************************************************************
 * @brief  Get timer state
 * @param  pTimer:	Pointer to timer to be used.
 * @retval			The timer state.
 **********************************************************************/
tSwTimerState SwTimerGetState( tSwTimerIndex SwTimerIndex  )
{
    tSwTimerState retVal = SW_TIMER_STATE_STOPPED;

    // Read timer state
    retVal = SwTimersArray[SwTimerIndex].state;

    return retVal;
}

/**********************************************************************
 * @brief  Set timer state
 * @param  pTimer:	Pointer to timer to be used.
 * @param  state:	New timer state.
 **********************************************************************/
eError SwTimerSetState( tSwTimerIndex SwTimerIndex, tSwTimerState state )
{
    eError result = RET_OK;

    switch( state )
    {
            case SW_TIMER_STATE_STOPPED:
                    SwTimerStopTimer( SwTimerIndex );
                    break;
            case SW_TIMER_STATE_STARTED:
                    SwTimerStartTimer( SwTimerIndex );
                    break;
            case SW_TIMER_STATE_CANCELLED:
                    SwTimerCancelTimer( SwTimerIndex );
                    break;
            default:
                    break;
    }

    // Change timer state.
    SwTimersArray[SwTimerIndex].state = state;

    return result;
}

/**********************************************************************
 * @brief  Check if timer is EXPIRED
 * @param  timerIndex: Index of timer to be used.
 * @retval Timer has expired or not.
 **********************************************************************/
tBool SwTimerIsExpired( tSwTimerIndex SwTimerIndex )
{
    tBool retVal = FALSE;

    // Check timer state.
    if(SwTimersArray[SwTimerIndex].state == SW_TIMER_STATE_EXPIRED)
    {
            retVal = TRUE;
    }
    else
    {
            retVal = FALSE;
    }
    return retVal;
}

/**********************************************************************
 * @brief  Sets Callback funtion to be called when timer has expired
 * @param  None.
 * @retval None.
 **********************************************************************/
eError SwTimerSetCallbackFunction ( tSwTimerIndex SwTimerIndex, tBool (*callbackFunction)(tSwTimerIndex SwTimerIndex) )
{
    eError result = RET_OK;

    if ( callbackFunction == NULL )
    {
        result = RET_BAD_POINTER_ADDRESS;
    }
    else
    {
        SwTimersArray[SwTimerIndex].pFuncCB = callbackFunction;
    }
    
    return result;
    
}

/**********************************************************************
 * @brief  Interrupt request rutine for periodic tick
 * @param  None.
 * @retval None.
 **********************************************************************/
void SwTimerIrq( void )
{
    uint8_t SwTimerIndex = 0;

    // Increment time global variable.
    timeMs = timeMs + TIMER_PERIOD; //timeMs++;

    // Update timers
    for( SwTimerIndex = 0 ; SwTimerIndex < NUM_OF_SW_TIMER ; SwTimerIndex++ )
    {
        SwTimerUpdateTimer( SwTimerIndex );
    }
}

#endif  /*_SWTIMER_C_*/
