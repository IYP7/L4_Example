/*************************************************************************//**
 * @file main.c
 *
 * @brief The main function is where a program starts execution.
 *
 * It is responsible for the high-level organization of the program's
 * functionality.
 *
 * @author Rubén Rodríguez
 *
 * @date 3/1/2012
 *
 ****************************************************************************/

#ifndef _MAIN_C_
#define _MAIN_C_


/****************************************************************************
*  COMMON INCLUDE
****************************************************************************/
#include "common.h"

/****************************************************************************
*  ARCHITECTURE INCLUDES
****************************************************************************/

/****************************************************************************
*  INCLUDES
****************************************************************************/
#include "main.h"
#include "Scheduler.h"
#include "system.h"
#include "interrupts.h"
#include "swtimer.h"
/*************************************************************************//**
 * @brief   Embedded application start routine.
 *
 * @param	None
 *
 * @return  None
 *
 * @details Initializes applications.
 * @details Call system application to execute active applications.
 ****************************************************************************/
int  main()
{
	/* Call to initial specific platform configuration */
    systemPreConfiguration();

	/* Applications Initialization */
    SchedulerInit();

    stm32L4Init();
    interruptsInit();
    interruptsStart();
    SwTimerInit();
    while (TRUE)
    {
        SchedulerNextSystemProcess();
    }

    return 0;
}

#endif /* _MAIN_C_ */

/****************************************************************************
 *    End of file
 ***************************************************************************/
