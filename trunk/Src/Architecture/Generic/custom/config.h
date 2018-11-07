/*************************************************************************//**
 * @file config.h
 *
 * @brief Define current configuration of the project.
 *
 * @author Rub�n Rodr�guez
 *
 * @date 3/1/2012
 *
 ****************************************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_

/****************************************************************************
 *  INCLUDES
 ****************************************************************************/
#include "stm32l4xx_hal.h"  /*ST HAL libraries*/

/****************************************************************************
 *  DEFINES
 ****************************************************************************/
 /* If bootloader is present system driver will reallocate interrupt vector table */
 /*  This define affect system HAL driver and has flash HAL driver as dependency */
//#define BOOTLOADER_PRESENT

//--START MONTSENY AUTOGENERATED 

#define UART_AVAILABLE
#define INTERRUPTS_AVAILABLE 
//#define FLASH_AVAILABLE
#define SYSTEM_AVAILABLE 
//#define SPI_AVAILABLE
#define GPIO_AVAILABLE
#define DMA_AVAILABLE
//#define BUTTONS_AVAILABLE
//#define PIPE_DEVICE_AVAILABLE
//#define LEDS_AVAILABLE
#define SW_TIMER_AVAILABLE 
//#define VIRTUAL_EEPROM_AVAILABLE
//#define EEPROM_HREG_SUPPORT
//#define EEPROM_SREG_SUPPORT
#define SYSTEM_MW_AVAILABLE 
//#define SHELL_AVAILABLE
//#define STATE_MACHINE_AVAILABLE
#define GENERIC_AVAILABLE 
#define SREG_AVAILABLE 
//#define HREG_AVAILABLE
#define SCHEDULER_AVAILABLE 
#define SYSTEM__APP_AVAILABLE 
#define MAIN_AVAILABLE 
#define BASIC_AVAILABLE 


#define STM32L4
//--END MONTSENY AUTOGENERATED

/* Watchdog Config */
#define WINDOW_WATCHDOG			FALSE
#define INDEPENDENT_WATCHDOG	FALSE
#define WATCHDOG_TESTMODE 		FALSE

/* RTC Test Config */
#define RTC_CALIBRATION_TEST 	FALSE

/* State machine Config */
#define PROTOCOL_DEVICE_INSTANCE 	0
#define FSM_PARAMETERS 				0

#define PIPE_LOG                PIPE_1
//#define UART_LOG                UART_1

#define TIMER_PERIOD			1		/* In ms*/

#endif /* _CONFIG_H_ */

/****************************************************************************
 *    End of file
 ****************************************************************************/
