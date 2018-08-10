/*************************************************************************//**
 * @file Scheduler.h
 *
 * @brief
 *
 * @author Marc Bernabé
 *
 * @date
 *
 ****************************************************************************/

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
*  ARCHITECTURE INCLUDE
***************************************************************************/

/****************************************************************************
*  INCLUDES
***************************************************************************/
#include "Scheduler_map.h"

/****************************************************************************
* DEFINES
****************************************************************************/
#define MAX_TASK_TIME_CNT 0xFFFFFFFF

/****************************************************************************
*  TYPE DEFINITIONS
****************************************************************************/
typedef struct sSchedulerProcess
{
	uint16_t uiTimeProcessCounter		: 14;
	uint8_t bEnabledSystemTask			: 1;
	uint8_t bEnabledUrgentSystemTask    : 1;
}tSchedulerProcess;

typedef struct sSchedulerInstanceMap
{
	void (*InitProcess)();
	void (*CallProcess)();
	uint16_t uiTimeProcess;
    bool initState;
} tSchedulerInstanceMap;


typedef struct sSchedulerContext
{
	uint32_t 	averageTaskTime_us;
	int32_t 	currentTaskTime_us;
	uint32_t 	maximumTaskTime_us;
	uint16_t	maxDelayProcessCtn;
	uint32_t	cntTaskTime;
	bool		taskActive;
	bool		userEnabled;
}tSchedulerContext;


/****************************************************************************
 *    PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************/
void SchedulerInit(void);
void SchedulerResetProcessTable(void); 							// Erase all process table
uint8_t SchedulerNextSystemProcess(void); 						// In the main loop
void SchedulerProcessesAutoEnable(void); 						// In the timer ingterrupt every 1ms
void SchedulerDisableTask(tScheduler processId);
void SchedulerEnableTask(tScheduler processId);
void SchedulerEnableUrgentTask(tScheduler processId);
void SchedulerDisableAllTask(void);

uint32_t SchedulerGetAverageTaskTime(tScheduler processId);
uint32_t SchedulerGetMaximumTaskTime(tScheduler processId);
uint32_t SchedulerGetMaxDelayProcess(tScheduler processId);
void SchedulerSetTaskProfileState(tScheduler processId, bool state);

#endif // _SCHEDULER_H

/****************************************************************************
 *    End of file
 ***************************************************************************/


