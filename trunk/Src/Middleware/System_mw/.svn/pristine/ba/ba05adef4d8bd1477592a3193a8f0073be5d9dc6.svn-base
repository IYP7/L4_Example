/*************************************************************************//**
 * @file System_app.c
 *
 * @brief System Middleware Definition.
 *
 * This file is providing the master middleware control interface.
 *
 * @author Rubén Rodríguez
 *
 * @date 3/1/2012
 *
 ****************************************************************************/

#ifndef _SYSTEM_MW_C_
#define _SYSTEM_MW_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/
#include "hreg_dat.h"
#include "sreg_dat.h"

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "System_app.h"
#include "System_mw.h"
#include "shell.h"
#include "Pipedevice.h"

/****************************************************************************
 *  PRIVATE VARIABLES
 ***************************************************************************/


/****************************************************************************
 *  PUBLIC FUNCTIONS
 ***************************************************************************/

/*************************************************************************//**
 * @brief   Initialization of the system Middleware module.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
void initSystemMw( void )
{
	/* Initialise the HREG layer */
	initHregAll();

	/* Initialise pipe Device */
    pipeDeviceInit();

    /* Start HREG layer */
	startHregGroupAll();

	/* Start pipe Device */
    pipeDeviceStart();
}


/*************************************************************************//**
 * @brief   Call to System middleware task.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
void callSystemMwTask( void )
{
	uint8_t    value;

	value = READ_SREG(SREG_SYSTEM_MW_NOTIF);

	switch (value)
	{
		case NOTIF_SYSTEM_MW_POWER_OFF:
			/* Debug info */
			printShellString((int8_t *)"System OFF ...", VERBOSE_GENERAL);
			/* Stop all drivers */
			stopHregGroupAll();
			/* Clear */
			WRITE_SREG(SREG_SYSTEM_MW_NOTIF, NOTIF_SYSTEM_MW_NO_REQUEST);
			break;

		case NOTIF_SYSTEM_MW_BOOTLOADER:
			/* Debug info */
			printShellString((int8_t *)"System BOOT ...", VERBOSE_GENERAL);
			/* Notify driver level about the bootloader process */
			WRITE_HREG(UNIQUE_INSTANCE, HREG_SYSTEM_POWER_MODE, NOTIF_SYSTEM_MW_BOOTLOADER);
			/* Clear */
			WRITE_SREG(SREG_SYSTEM_MW_NOTIF, NOTIF_SYSTEM_MW_NO_REQUEST);
			break;

		case NOTIF_SYSTEM_MW_SLEEP:
			/* Debug info */
			printShellString((int8_t *)"System SLEEP ...", VERBOSE_GENERAL);
			/* Sleep all HREG groups */
			sleepHregGroupAll();
			/* Clear */
			WRITE_SREG(SREG_SYSTEM_MW_NOTIF, NOTIF_SYSTEM_MW_NO_REQUEST);

			/* Execute Sleep routine*/
			WRITE_HREG(UNIQUE_INSTANCE, HREG_SYSTEM_POWER_MODE, NOTIF_SYSTEM_MW_SLEEP);

			/* Wake up all HREG groups */
			wakeHregGroupAll();
			/* Wake Up condition */
			requestSystemAppPowerState(POWER_STATE_WAKE_UP);
			break;

		case NOTIF_SYSTEM_MW_RESET:
			/* Debug info */
			printShellString((int8_t *)"System RESET ...", VERBOSE_GENERAL);
			/* Notify driver level about the reset process */
			WRITE_HREG(UNIQUE_INSTANCE, HREG_SYSTEM_POWER_MODE, NOTIF_SYSTEM_MW_RESET);
			/* Clear */
			WRITE_SREG(SREG_SYSTEM_MW_NOTIF, NOTIF_SYSTEM_MW_NO_REQUEST);
			break;

		default:
			break;
	}
}

/*************************************************************************//**
 * @brief   Activates/Deactivates power areas.
 *
 * @param	Module to change power state.
 * @param   New power state
 *
 * @return  None.
 *
 ****************************************************************************/
void SystemSetPower( tPowerModules module, tPowerModes state )
{
	switch(module)
	{
		case SYSTEM_POWER_MAIN:
//			WRITE_HREG(HREG_GPIO_PORT, SW1_CONTROL_GPIO);
			break;

		default:
			return;
	}

//	WRITE_HREG(HREG_GPIO_VALUE, state);
}

#endif /* _SYSTEM_MW_C_ */

/****************************************************************************
 *    End of file
 ****************************************************************************/
