/*************************************************************************//**
 * @file main.c
 *
 * @brief The main function is where a program starts execution.
 *
 * It is responsible for the high-level organization of the program's
 * functionality.
 *
 * @author Rub�n Rodr�guez
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
#include "GPIO.h"

#include "uart.h"
#include "flash.h"
#include "spi.h"
#include "DMA.h"
#include "sd.h"
#include "PipeDevice.h"
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

    /* HAL TEST */
    GPIOInit();
    stm32L4Init();

    interruptsInit();
    interruptsStart();

    spiInit();
    spiStart();

    uartInit();
    uartStart();

    FlashInit();
    SwTimerInit();
    pipeDeviceInit();
/*    dmaInit();
    dmaStart()
    sdInit();
    sdStart();*/
    /*\ HAL TEST */

	/* Applications Initialization */
    SchedulerInit();

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
