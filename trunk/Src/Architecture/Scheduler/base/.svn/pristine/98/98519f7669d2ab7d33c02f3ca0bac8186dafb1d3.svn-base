/*************************************************************************//**
 * @file Scheduler.c
 *
 * @brief	Scheduler
 *
 * @author Marc Bernabé
 * @author Albert Serrallo
 *
 * @date	02/06/2016
 ****************************************************************************/
#ifndef _SCHEDULER_C_
#define _SCHEDULER_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include <Scheduler.h>
#include "interrupts.h"
#include "System.h"

/****************************************************************************
 *  DEFINES
 ***************************************************************************/

/****************************************************************************
* PRIVATE VARIABLES
****************************************************************************/
uint32_t ticks_per_us;

/****************************************************************************
 *  EXTERN VARIABLES
 ****************************************************************************/
 extern const tSchedulerInstanceMap  SchedulerInstanceMap[];

 /****************************************************************************
 *  PRIVATE FUNCTIONS
 ****************************************************************************/
static void SchedulerExecuteSystemProcess(void);
void SchedulerUpdateTaskTime(int32_t initTimeTaskScheduler, int32_t finalTimeTaskScheduler);

/****************************************************************************
 *  PRIVATE DEFINES
 ****************************************************************************/

/****************************************************************************
 *    PUBLIC VARIABLE
 ****************************************************************************/
tSchedulerProcess schedulerProcess[NUM_OF_SCHED_TASKS];
tSchedulerContext schedulerContext[NUM_OF_SCHED_TASKS];
uint8_t ptrTask;


/*************************************************************************//**
 * @brief   Init process table
 * @param   None.
 * @return  None
 ****************************************************************************/
void SchedulerInit(void)
{
	uint8_t i;

	SchedulerResetProcessTable();

 	for (i = 0; i < NUM_OF_SCHED_TASKS; i++)
 	{
		if(SchedulerInstanceMap[i].InitProcess != NULL)
		{
			(*(SchedulerInstanceMap[i].InitProcess))();
		}
 	}
    
    systemGetCLkFreq(&ticks_per_us);
    ticks_per_us /= 1000000;
    
}

/*************************************************************************//**
 * @brief   Reset the Process Table with its initial values
 * @param   None.
 * @return  None
 ****************************************************************************/
void SchedulerResetProcessTable(void)
{
	uint8_t i;

	ptrTask	= 0;

 	for (i = 0; i < NUM_OF_SCHED_TASKS; i++)
 	{
		schedulerProcess[i].uiTimeProcessCounter	= 0;
		schedulerProcess[i].bEnabledSystemTask	= SchedulerInstanceMap[i].initState;
		schedulerProcess[i].bEnabledUrgentSystemTask = FALSE;

		schedulerContext[i].maxDelayProcessCtn = 0;
		schedulerContext[i].maximumTaskTime_us = 0;
		schedulerContext[i].averageTaskTime_us = 0;
		schedulerContext[i].currentTaskTime_us = 0;
		schedulerContext[i].cntTaskTime = 0;
		schedulerContext[i].userEnabled = TRUE;
 	}
}

/*************************************************************************//**
 * @brief   Search the next process to execute depending on its enable-disable
 *          state and the previous order carried out
 * @param   None.
 * @return  bool: TRUE -> if some process has been found.
 ****************************************************************************/
uint8_t SchedulerNextSystemProcess(void)
{
    bool taskExecuted	= FALSE;
    uint8_t i			= 0;

    do
	{
        i++;
        /* Increment the next pointer process */
        ptrTask++;
        /* If ptrTask is equal to final vector, then restart ptrTask */
		ptrTask %= NUM_OF_SCHED_TASKS;

		/* SystemTask with priority is Enable */
		/* or SystemTask is Enable and Time process counter is higher than time process then enable and execute the process task */
		if ( schedulerProcess[ptrTask].bEnabledUrgentSystemTask == TRUE ||
			 ( schedulerProcess[ptrTask].bEnabledSystemTask == TRUE  && schedulerProcess[ptrTask].uiTimeProcessCounter >= SchedulerInstanceMap[ptrTask].uiTimeProcess ) )
		{

			if ( schedulerProcess[ptrTask].bEnabledUrgentSystemTask == TRUE )
			{
				schedulerProcess[ptrTask].bEnabledUrgentSystemTask = FALSE;
			}
			else
			{
				schedulerProcess[ptrTask].uiTimeProcessCounter = 0;
			}

			taskExecuted = TRUE;

			if (schedulerContext[ptrTask].maxDelayProcessCtn < (schedulerProcess[ptrTask].uiTimeProcessCounter - SchedulerInstanceMap[ptrTask].uiTimeProcess))
			{
				schedulerContext[ptrTask].maxDelayProcessCtn = schedulerProcess[ptrTask].uiTimeProcessCounter - SchedulerInstanceMap[ptrTask].uiTimeProcess;
			}

			SchedulerExecuteSystemProcess();
        }
    }
	while ( ( taskExecuted == FALSE ) && ( i <= NUM_OF_SCHED_TASKS ) );

    return taskExecuted;
}

/*************************************************************************//**
 * @brief   Execute the "ptrTask" process
 * @param   None.
 * @return  None
 ****************************************************************************/
static void SchedulerExecuteSystemProcess(void)
{
	int32_t initTimeTaskScheduler, finalTimeTaskScheduler;

	/*	Get SystTick Value before Process Function */
	schedulerContext[ptrTask].currentTaskTime_us = 0;
	initTimeTaskScheduler =  SYSTEM_TICK;
	schedulerContext[ptrTask].taskActive = TRUE;

	/*	Execute the Process function*/
    (*(SchedulerInstanceMap[ptrTask].CallProcess))();

    /*	Get SystTick Value after Process Function and update the taskTime */
	schedulerContext[ptrTask].taskActive = FALSE;
    finalTimeTaskScheduler = SYSTEM_TICK;

    // Update uS of currentTaskTime_us
    if ( schedulerContext[ptrTask].userEnabled == TRUE )
    {
    	SchedulerUpdateTaskTime(initTimeTaskScheduler, finalTimeTaskScheduler);
    }
}


/*************************************************************************//**
 * @brief   Enable the "Process_Number" in the tasks pipe
 * 			This function will be executed in every interrupt of SystTick
 * 			"Put this function in HAL_SYSTICK_Callback()"
 * @param   None.
 * @return  None
 ****************************************************************************/
void SchedulerProcessesAutoEnable(void)
{
	uint8_t i;

	for (i = 0; i < NUM_OF_SCHED_TASKS; i++)
	{
		schedulerProcess[i].uiTimeProcessCounter++;

		// If currentTaskTime it's enable, update 1000uS=1mS
		if (schedulerContext[i].taskActive == TRUE && schedulerContext[i].userEnabled == TRUE )
		{
			schedulerContext[i].currentTaskTime_us += 1000;
		}
	}
}



/*************************************************************************//**
 * @brief  	Update average task time, maximum task time
 * @param   process, initTime, finalTime
 * @return  none
 ****************************************************************************/
void SchedulerUpdateTaskTime(int32_t initTimeTaskScheduler, int32_t finalTimeTaskScheduler)
{
	uint64_t averageTaskTemp;
	schedulerContext[ptrTask].currentTaskTime_us += ((initTimeTaskScheduler - finalTimeTaskScheduler)/(int32_t)(ticks_per_us));
	schedulerContext[ptrTask].cntTaskTime %= MAX_TASK_TIME_CNT;
	schedulerContext[ptrTask].cntTaskTime++;

	/* Calculate the average task time */
	averageTaskTemp = (uint64_t)(schedulerContext[ptrTask].averageTaskTime_us*(schedulerContext[ptrTask].cntTaskTime-1));
	schedulerContext[ptrTask].averageTaskTime_us = (uint32_t)(averageTaskTemp + schedulerContext[ptrTask].currentTaskTime_us) / schedulerContext[ptrTask].cntTaskTime;

    /* If current Task Time is higher than maximum, update maximum task time */
    if ( schedulerContext[ptrTask].currentTaskTime_us > schedulerContext[ptrTask].maximumTaskTime_us )
    {
    	schedulerContext[ptrTask].maximumTaskTime_us = schedulerContext[ptrTask].currentTaskTime_us;
    }

}


/*************************************************************************//**
 * @brief   Returns the maximum task time
 * @param   process.
 * @return  average task time
 ****************************************************************************/
uint32_t SchedulerGetMaximumTaskTime(tScheduler processId)
{
	return schedulerContext[processId].maximumTaskTime_us;
}


/*************************************************************************//**
 * @brief   Returns the average task time
 * @param   process.
 * @return  average task time
 ****************************************************************************/
uint32_t SchedulerGetAverageTaskTime(tScheduler processId)
{
	return schedulerContext[processId].averageTaskTime_us;
}


/*************************************************************************//**
 * @brief   Set taskTime enable of disable
 * @param   Process number that is going to be enabled or disabled.
 * @return  None
 ****************************************************************************/
void SchedSetTaskTime(tScheduler processId, bool state)
{
	schedulerContext[processId].userEnabled = state;
}

/*************************************************************************//**
 * @brief   Returns the maximum delay process
 * @param   Process .
 * @return  max process delay
 ****************************************************************************/
uint32_t SchedulerGetMaxDelayProcess(tScheduler processId)
{
	return schedulerContext[processId].maxDelayProcessCtn;
}


/*************************************************************************//**
 * @brief   Disable "processId"
 * @param   Process number that is going to be disabled.
 * @return  None
 ****************************************************************************/
void SchedulerDisableTask(tScheduler processId)
{
	schedulerProcess[processId].bEnabledSystemTask	= FALSE;
}


/*************************************************************************//**
 * @brief   Enable "processId"
 * @param   Process number that is going to be enabled.
 * @return  None
 ****************************************************************************/
void SchedulerEnableTask(tScheduler processId)
{
	schedulerProcess[processId].bEnabledSystemTask	= TRUE;
}

/*************************************************************************//**
 * @brief   Enable "processId"
 * @param   Process number that is going to be enabled.
 * @return  None
 ****************************************************************************/
void SchedulerEnableUrgentTask(tScheduler processId)
{
	schedulerProcess[processId].bEnabledUrgentSystemTask = TRUE;
}

void SchedulerDisableAllTask(void)
{
	uint8_t i;

	for (i = 0; i < NUM_OF_SCHED_TASKS; i++)
	{
		schedulerProcess[i].bEnabledSystemTask	= FALSE;
	}
}

#endif // _SCHEDULER_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
