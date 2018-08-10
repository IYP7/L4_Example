/*************************************************************************//**
 * @file System_app.c
 * 
 * @brief System Middleware Header Definition.
 * 
 * @author Rubén Rodríguez
 * 
 * @date 3/1/2012
 *
 ****************************************************************************/

#ifndef _SYSTEM_MW_H_
#define _SYSTEM_MW_H_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

typedef enum ePowerModes
{
	POWER_OFF,
	POWER_ON
} tPowerModes;


typedef enum ePowerModules
{
	SYSTEM_POWER_MAIN,

	NUMBER_OF_POWER_MODULES
} tPowerModules;

/* Values of SREG_SYSTEM_MW_NOTIF */
typedef enum
{
	NOTIF_SYSTEM_MW_NO_REQUEST = 0x00,
	NOTIF_SYSTEM_MW_SLEEP,
	NOTIF_SYSTEM_MW_BOOTLOADER,
	NOTIF_SYSTEM_MW_RESET,

	NOTIF_SYSTEM_MW_POWER_UP,
	NOTIF_SYSTEM_MW_POWER_UP_CHARGING,
	NOTIF_SYSTEM_MW_POWER_SAVING,
	NOTIF_SYSTEM_MW_POWER_SAVING_CHARGING,
	NOTIF_SYSTEM_MW_STANDBY,
	NOTIF_SYSTEM_MW_STANDBY_CHARGING,

	NOTIF_SYSTEM_MW_POWER_OFF,

	NOTIF_SYSTEM_MW_MAX_NOTIF
} eMWNotifications;

/****************************************************************************
 *  PUBLIC FUCNTION PROTOTYPES
 ***************************************************************************/
void initSystemMw( void );
void callSystemMwTask( void );
void SystemSetPower( tPowerModules module, tPowerModes state );

#endif /* _SYSTEM_MW_H_ */

/****************************************************************************
 *    End of file
 ***************************************************************************/
