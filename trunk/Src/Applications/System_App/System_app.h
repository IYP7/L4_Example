/*************************************************************************//**
 * @file System_app.h
 * 
 * @brief System Application include.
 * 
 * @author Rubén Rodríguez
 * 
 * @date 3/1/2012
 *
 ****************************************************************************/

#ifndef _SYSTEM_APP_H_
#define _SYSTEM_APP_H_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/
typedef enum
{
    POWER_STATE_NO_REQUEST = 0,

    POWER_STATE_START_SHUTDOWN,
	POWER_STATE_WAKE_UP,
    POWER_STATE_GO_TO_SLEEP,
    POWER_STATE_REBOOT,

	POWER_STATE_MAX_REQUESTS
} ePowerStateRequest;

/* Value of SREG_SYSTEM_STATE */
typedef enum
{
    SYSTEM_APP_INIT_SHELL = 0,
    SYSTEM_APP_INIT,
    SYSTEM_APP_ON,
    SYSTEM_APP_POWER_OFF,
    SYSTEM_APP_SLEPT,
    SYSTEM_APP_REBOOT,
    SYSTEM_APP_MAX_STATES
} eSystemStates;

/****************************************************************************
 *  PUBLIC FUCNTION PROTOTYPES
 ***************************************************************************/
void callSystemApp( void );
void initSystemApp( void );
void requestSystemAppPowerState( uint8_t powerState );
void requestSystemAppLaunchBootloader(void);
void requestSystemAppReset( void );

#endif /* _SYSTEM_APP_H_ */

/****************************************************************************
 *    End of file
 ***************************************************************************/
