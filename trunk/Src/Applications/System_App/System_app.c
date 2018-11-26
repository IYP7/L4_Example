/*************************************************************************//**
 * @file System_app.c
 * 
 * @brief System Application Definition.
 * 
 * This file is providing the master application control interface. 
 * 
 * @author Rubén Rodríguez
 * 
 * @date 3/1/2012
 *
 ****************************************************************************/

#ifndef _SYSTEM_APP_C_
#define _SYSTEM_APP_C_

/****************************************************************************
*  COMMON INCLUDE
****************************************************************************/
#include "common.h"

/****************************************************************************
*  ARCHITECTURE INCLUDES
****************************************************************************/
//#include "sreg_dat.h"

/****************************************************************************
*  INCLUDES
****************************************************************************/
#include "System_mw.h"
#include "System_app.h"

#include "GPIO.h"
#include "VirtualEEPROM.h"
/****************************************************************************
*  PRIVATE VARIABLES
****************************************************************************/
uint8_t requestPowerState;  /**< keeps power state request to be analized during system app task */
uint8_t x = 0;
/****************************************************************************
*  PUBLIC FUCNTIONS
****************************************************************************/

/*************************************************************************//**
 * @brief   Initialization of the system application module.
 * 
 * @param	None
 * 
 * @return  None  
 *
 * @details Initializes HREG and SREG layers.
 * @details Initializes all applications
 ****************************************************************************/
void initSystemApp( void )
{
    requestPowerState = POWER_STATE_NO_REQUEST;

    initSregAll();    /* Initialise the SREG layer */
}


/*************************************************************************//**
 * @brief   Executes the system application task.
 * 
 * @param	None
 * 
 * @return  None  
 *
 * @details depending on current state, executes active applications.
 * @details calls system middleware task
 ****************************************************************************/
void callSystemApp( void )
{
    uint8_t systemState;

    /* HAL TEST */
    // GPIO
    GPIOWritePort(GPIO_LED_1, GPIO_TOGGLE);

    // VirtualEEPROM

    /* END HAL TEST */

    /* System state machine */
    systemState = READ_SREG(SREG_SYSTEM_STATE);
    //systemState = SYSTEM_APP_INIT_SHELL;
    switch ( systemState )
    {
		case SYSTEM_APP_INIT_SHELL:
			/* Initialise SHELLSTACK*/
			systemState = SYSTEM_APP_INIT;
			break;
        
		case SYSTEM_APP_INIT:
			/* Application init dispather */
			AtCommandStart();
			systemState = SYSTEM_APP_ON;
            break;
		
		case SYSTEM_APP_POWER_OFF:
			/* Application power off dispather */

            break;

        case SYSTEM_APP_SLEPT:
			/* Application sleep dispather */

			/* Power Sequence control */
            if ( requestPowerState == POWER_STATE_WAKE_UP ) 
            {
				/* Wake up */
                systemState = SYSTEM_APP_ON;
                requestPowerState = POWER_STATE_NO_REQUEST;
			} 
            else 
            {
				/* Keep same state */
			}
            break;

        case SYSTEM_APP_ON:
			/* Application dispatcher */
			/* Power Sequence control */
            if ( requestPowerState == POWER_STATE_START_SHUTDOWN )
            {
                /* System shall do SHUTDOWN */
				/* Execute system Mw power down. This will power off all drivers */
                WRITE_SREG(SREG_SYSTEM_MW_NOTIF, NOTIF_SYSTEM_MW_POWER_OFF);
                
                /* Power off */
                systemState = SYSTEM_APP_POWER_OFF;
            }
            else if ( requestPowerState == POWER_STATE_GO_TO_SLEEP )
            {
                /* Execute system Mw sleep sequence. This will disable all drivers */
                WRITE_SREG(SREG_SYSTEM_MW_NOTIF, NOTIF_SYSTEM_MW_SLEEP);
                
                /* Slept */
                systemState = SYSTEM_APP_SLEPT;
            }
            else if ( requestPowerState == POWER_STATE_REBOOT )
            {
                systemState = SYSTEM_APP_REBOOT;
				WRITE_SREG(SREG_SYSTEM_MW_NOTIF, NOTIF_SYSTEM_MW_RESET);
            }
            
            /* Reset power state request */
            requestPowerState = POWER_STATE_NO_REQUEST;
            break;
			
		default:
		   /* Do nothing */
			break;
        
    }
    
    /* Update system state */
    WRITE_SREG(SREG_SYSTEM_STATE, systemState);
}

/*************************************************************************//**
 * @brief   Receives a request to change power state.
 * 
 * @param	new power state.
 * 
 * @return  None  
 ****************************************************************************/
void requestSystemAppPowerState( uint8_t powerState )
{
    requestPowerState = powerState;
}

/*************************************************************************//**
 * @brief   Receives a request to start bootloader.
 * 
 * @param	None
 * 
 * @return  None  
 ****************************************************************************/
void requestSystemAppLaunchBootloader( void )
{	
	uint8_t systemState;

    WRITE_SREG(SREG_SYSTEM_MW_NOTIF, NOTIF_SYSTEM_MW_BOOTLOADER);
    systemState = SYSTEM_APP_REBOOT;

	/* Update system state */
    WRITE_SREG(SREG_SYSTEM_STATE, systemState);
}

/*************************************************************************//**
 * @brief   Receives a request to reset.
 * 
 * @param	None
 * 
 * @return  None  
 ****************************************************************************/
void requestSystemAppReset( void )
{	
	uint8_t systemState;

    WRITE_SREG(SREG_SYSTEM_MW_NOTIF, NOTIF_SYSTEM_MW_RESET);
    systemState = SYSTEM_APP_REBOOT;

	/* Update system state */
    WRITE_SREG(SREG_SYSTEM_STATE, systemState);
}

void ATLTECallback(tAtResponseID eventID, uint8_t *buffer, uint16_t length)
{
//	uartDriverSetBufferSize((tUart)UART_2, 6);
//	uartDriverWrite((tUart)UART_2, (uint8_t*)"LTE: ");
//	uartDriverSetBufferSize((tUart)UART_2, length);
//	uartDriverWrite((tUart)UART_2, (uint8_t*)buffer);
}

void ATGPSCallback(tAtResponseID eventID, uint8_t *buffer, uint16_t length)
{
//	uartDriverSetBufferSize((tUart)UART_2, 6);
//	uartDriverWrite((tUart)UART_2, (uint8_t*)"GPS: ");
//	uartDriverSetBufferSize((tUart)UART_2, length);
//	uartDriverWrite((tUart)UART_2, (uint8_t*)buffer);
}

#endif /* _SYSTEM_APP_C_ */

/****************************************************************************
 *    End of file
 ***************************************************************************/
