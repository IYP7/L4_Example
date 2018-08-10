/*************************************************************************//**
 * @STUB No functionality implemented FULLY
 *
 * @file System.h
 *
 * @brief Microcontroller specific driver Header - stm32F4xx.
 *
 * @author Jordi Castells
 * @author Albert Serrallo
 *
 * @date 10/02/2015
 *
 ****************************************************************************/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

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

/****************************************************************************
 * DEFINES
 ****************************************************************************/
#define FIRST_DATA		0x5A5A
#define HSE_RTC_DIVISOR		(2)

#define I_WINDOW_MODE_DEACTIVATE	(820)
#define W_WINDOW_MODE_DEACTIVATE	(44)
#define IWDG_WINDOW_DISABLE			0xFFF

#define IS_POWER_MODE(powerMode)	(((powerMode) == SYSTEM_EXECUTE_BOOTLOADER) || \
                                	((powerMode) == SYSTEM_EXECUTE_STANDBY) || \
                                	((powerMode) == SYSTEM_EXECUTE_SLEEP) || \
									((powerMode) == SYSTEM_EXECUTE_RESET))

#define IS_WATCHDOG(wdg)			(((wdg) == W_WATCHDOG) || ((wdg) == I_WATCHDOG))

#define SYSTEM_TICK SysTick->VAL

/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/
/* Value of HREG_SYSTEM_POWER_MODE */
typedef enum eSystemPowerModes
{
	SYSTEM_EXECUTE_BOOTLOADER,
	SYSTEM_EXECUTE_STANDBY,
	SYSTEM_EXECUTE_SLEEP,
	SYSTEM_EXECUTE_RESET,

	SYSTEM_MAX_POWER_MODES
} tSystemPowerModes;

typedef enum ePowerUpReasons
{
	SYSTEM_FIRST_POWER_UP,
	SYSTEM_RESET_POWER_UP,
	SYSTEM_RESET_WATCHDOG,
	SYSTEM_RESET_SOFTWARE,
	SYSTEM_BOR_RESET,
	SYSTEM_POR_RESET,
	SYSTEM_LOW_POWER_RESET,
	SYSTEM_RESET_WINDOW_WATCHDOG,

	SYSTEM_MAX_POWER_UP_REASONS
} tPowerUpReasons;

typedef enum eSystemImages
{
	SYSTEM_IMAGE_A,
	SYSTEM_IMAGE_B,

	SYSTEM_MAX_IMAGES = SYSTEM_IMAGE_B
} tSystemImages;

typedef enum eWatchdog
{
	W_WATCHDOG,
	I_WATCHDOG
} tWatchdog;

typedef struct sWatchdogStruct
{
	uint16_t mSecs_UntilReset;
	uint16_t mSecs_LowWindow;

} tWatchdogStruct;


typedef struct sSystemInstanceMap
{
	/* RCC OSC Config*/
	uint32_t OscillatorType;
	uint32_t HSEState;
	uint32_t LSEState;
	uint32_t HSIState;
	uint32_t HSICalibrationValue;
	uint32_t LSIState;

	/* RCC OSC: PLL Config */
	uint32_t PLLState;
	uint32_t PLLSource;
	uint32_t PLLM;	  		/**< @brief Division Factor for PLL VCO input clock */
	uint32_t PLLN;			/**< @brief Multiplication Factor for PLL VCO output clock */
	uint32_t PLLP;			/**< @brief Division factor for main system clock */
	uint32_t PLLQ;			/**< @brief Division factor for OTG, FS, SDIO, RNG clocks */
	uint32_t PLLR;			/**< @brief Division factor for I2S, SAI clocks or Output clock*/

	/* RCC CLOCK Config */
	uint32_t SYSCLKSource;
	uint32_t AHBCLKDivider;
	uint32_t APB1CLKDivider;
	uint32_t APB2CLKDivider;

	/* RTC clock selection */
	uint32_t RTCClockSelection;
} tSystemInstanceMap;

/****************************************************************************
 *    PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************/
eError stm32F4Init( void );
eError stm32F4Stop( void );
eError stm32F4Start( void );
eError stm32F4Sleep( void );
eError stm32F4Wake( void );
eError stm32F4SetPowerMode( tSystemPowerModes powerMode );
tPowerUpReasons checkPowerUpReason( void );

eError RTCSetTime (uint32_t time);
eError RTCGetTime (uint32_t* time);
eError RTCSetDate (uint32_t date);
eError RTCGetDate (uint32_t* date);

eError getAPB1Clock (uint32_t *APBClock);
eError watchdogStart( void );
eError systemGetCLkFreq(uint32_t *ClockFrequency);
void clearWatchdog(tWatchdog watchdog);
void systemPreConfiguration( void );

void systemSoftReset(void);
void systemJumpToBoot(void);

#endif // _SYSTEM_H_

/****************************************************************************
 *    End of file
 ***************************************************************************/
