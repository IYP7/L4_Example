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
//#include "shell.h"

#include "GPIO.h"
#include "uart.h"
#include "Flash.h"
#include "system.h"
/****************************************************************************
*  PRIVATE VARIABLES
****************************************************************************/
uint8_t requestPowerState;  /**< keeps power state request to be analized during system app task */

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

    uint32_t time, date;
	uint8_t hours, min, sec, year, month, day;

	RTCGetTime(&time);
	RTCGetDate(&date);
	uint8_t tmp = ':';

	hours = (time >> 24) & 0xFF;
	min = (time >> 16) & 0xFF;
	sec = (time >> 8) & 0xFF;
	year = (date >> 24) & 0xFF;
	year += '0';
	month = (date >> 16) & 0xFF;
	month += '0';
	day = (date >> 8) & 0xFF;
	day += '0';

	uartDriverSetBufferSize(UART_2, 1);
	if(hours<10)
	{
		hours+= '0';
		uartDriverWrite(UART_2, &hours);
	}
	else
	{
		tmp = hours/10;
		tmp += '0';
		uartDriverWrite(UART_2, &tmp);
		tmp = hours%10;
		tmp += '0';
		uartDriverWrite(UART_2, &tmp);
	}
	tmp = ':';
	uartDriverWrite(UART_2, &tmp);

	if(min<10)
	{
		min+= '0';
		uartDriverWrite(UART_2, &min);
	}
	else
	{
		tmp = min/10;
		tmp += '0';
		uartDriverWrite(UART_2, &tmp);
		tmp = min%10;
		tmp += '0';
		uartDriverWrite(UART_2, &tmp);
	}
	tmp = ':';
	uartDriverWrite(UART_2, &tmp);

	if(sec<10)
	{
		sec+= '0';
		uartDriverWrite(UART_2, &sec);
	}
	else
	{
		tmp = sec/10;
		tmp += '0';
		uartDriverWrite(UART_2, &tmp);
		tmp = sec%10;
		tmp += '0';
		uartDriverWrite(UART_2, &tmp);
	}
	tmp = '\n';
	uartDriverWrite(UART_2, &tmp);
	uartDriverWrite(UART_2, &day);
	tmp = '/';
	uartDriverWrite(UART_2, &tmp);
	uartDriverWrite(UART_2, &month);
	uartDriverWrite(UART_2, &tmp);
	uartDriverWrite(UART_2, &year);
	tmp = '\n';
	uartDriverWrite(UART_2, &tmp);
	uartDriverWrite(UART_2, &tmp);
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

#endif /* _SYSTEM_APP_C_ */

/****************************************************************************
 *    End of file
 ***************************************************************************/
