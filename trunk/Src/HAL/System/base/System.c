/*************************************************************************//**
 * @file System.c
 *
 * @brief Microcontroller specific driver - stm32F4xx.
 *
 * @author Gonzalo Serrano
 * @author Albert Serrallo
 *
 * @date 10/02/2016
 *
 ****************************************************************************/

#ifndef _SYSTEM_C_
#define _SYSTEM_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/
#include "hreg_dat.h"

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "System.h"
#include "System_map.h"

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#define FLASH_INITIAL_ADDRESS (0x08000000)
#define BOOT_INITIAL_ADDRESS  (0x08000000)
#define BOOT_ENTRY_ADDRESS    (0x08000004)

#ifdef MONTSENY_FLASH_MARK
const uint32_t montseny_mark[4] __attribute__((__section__(".montseny_mark"))) = { 0xFFFFFFFF,
		0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};
#endif
/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
tPowerUpReasons powerUpReason = SYSTEM_FIRST_POWER_UP;
RTC_HandleTypeDef RTCHandler;

#if (WINDOW_WATCHDOG == 1)
WWDG_HandleTypeDef WWDGHandler;
uint8_t WWDGCounterValue;
#endif
#if (INDEPENDENT_WATCHDOG == 1)
IWDG_HandleTypeDef IWDGHandler;
uint16_t IWDGCounterValue;
#endif

typedef void (*pFunction)(void);

/****************************************************************************
 *  EXTERN VARIABLES
 ****************************************************************************/
//TODO:
extern const tWatchdogStruct WWDGMap;
extern const tWatchdogStruct IWDGMap;
extern const tSystemInstanceMap SystemMap;

/****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/
static void storePowerUpReason( void );
static eError watchdogInit( void );

#if (INDEPENDENT_WATCHDOG == 1)
static eError IWDGInit( void );
static eError IWDGStart( void );
#endif
#if (WINDOW_WATCHDOG == 1)
static eError WWDGInit( void );
static eError WWDGStart( void );
#endif


static eError SystemClock_Config(void);
#ifdef MON_RTC_ENABLED
static void RTCInit( void );
static eError RTCSetInitDateTime( void );
#endif
/*************************************************************************//**
 * @brief  Init stm32F4 driver.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
eError stm32F4Init( void )
{
	eError result = RET_OK;
	/* If WATCHDOG_TESTMODE is activated window watchdog and independent watchdog	*/
	/* counter is frozen when the MCU is halted for testing purpose					*/
#if (WATCHDOG_TESTMODE == 1)
	__HAL_RCC_DBGMCU_CLK_ENABLE();
	__HAL_DBGMCU_FREEZE_WWDG();
	__HAL_DBGMCU_FREEZE_IWDG();
#endif

	  /* Configure Flash prefetch */
	//HAL_Init();
	#if (PREFETCH_ENABLE != 0)
	__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	#endif /* PREFETCH_ENABLE */

	storePowerUpReason();

	result = SystemClock_Config();
	if (result == RET_OK)
		{
#if defined(MON_RTC_ENABLED)
			RTCInit();
#endif
			result = watchdogInit();
		}

	return result;
}

/*************************************************************************//**
 * @brief  Starts stm32F4 driver.
 *
 * @param	None.
 *
 * @return  eError.
 *
 ****************************************************************************/
eError stm32F4Start( void )
{
	eError result = RET_OK;
	WRITE_HREG(UNIQUE_INSTANCE, HREG_SYSTEM_POWER_UP_REASON, powerUpReason);

	return result;
}


/*************************************************************************//**
 * @brief  Stops stm32F4 driver.
 *
 * @param	None.
 *
 * @return  eError
 *
 ****************************************************************************/
eError stm32F4Stop( void )
{
	eError result = RET_OK;
	result = stm32F4SetPowerMode(SYSTEM_EXECUTE_STANDBY);
	return result;
}


/*************************************************************************//**
 * @brief  Sleeps stm32F4 driver.
 *
 * @param	None.
 *
 * @return  eError
 *
 ****************************************************************************/
eError stm32F4Sleep( void )
{
	eError result = RET_OK;
	result = stm32F4SetPowerMode(SYSTEM_EXECUTE_SLEEP);

	return result;
}

/*************************************************************************//**
 * @brief  Wakes up stm32F4 driver.
 *
 * @param	None.
 *
 * @return  eError.
 *
 ****************************************************************************/
eError stm32F4Wake( void )
{
	eError result = RET_OK;
	return result;
}

/*************************************************************************//**
 * @brief  Check power up reason.
 *
 * @param	None.
 *
 * @return  ePowerUpReasons
 *
 ****************************************************************************/
tPowerUpReasons checkPowerUpReason( void )
{
	return powerUpReason;
}

/*************************************************************************//**
 * @brief  Store the power up reason flags in a global variable.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
static void storePowerUpReason( void )
{
	/* Check if the StandBy flag is set */
	if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		/* Clear StandBy flag */
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
//
//		/* Wait for RTC APB registers synchronization (needed after start-up from Reset)*/
//		HAL_RTC_WaitForSynchro(&RTCHandler);
//		/* No need to configure the RTC as the RTC config(clock source, enable,
//		   prescaler,...) are kept after wake-up from STANDBY */

		powerUpReason = SYSTEM_RESET_POWER_UP;
	}
	else
	{
		if ( __HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET )
		{
			powerUpReason = SYSTEM_RESET_WINDOW_WATCHDOG;
		}
		else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
		{
			powerUpReason = SYSTEM_RESET_WATCHDOG;
		}
		else if ( __HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) != RESET )
		{
			powerUpReason = SYSTEM_RESET_SOFTWARE;
		}
		else if ( __HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET )
		{
			powerUpReason = SYSTEM_RESET_POWER_UP;
		}
		else if ( __HAL_RCC_GET_FLAG(RCC_FLAG_BORRST) != RESET )
		{
			powerUpReason = SYSTEM_BOR_RESET;
		}
		else if ( __HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET )
		{
			powerUpReason = SYSTEM_POR_RESET;
		}
		else if ( __HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST) != RESET )
		{
			powerUpReason = SYSTEM_LOW_POWER_RESET;
		}
		else
		{
			powerUpReason = SYSTEM_FIRST_POWER_UP;
		}
	}

	__HAL_RCC_CLEAR_RESET_FLAGS();
}

/*************************************************************************//**
 * @brief  Initialize all configured watchdogs.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
static eError watchdogInit( void )
{
	eError result = RET_OK;

#if (INDEPENDENT_WATCHDOG == 1)
	result = IWDGInit();
#endif
#if (WINDOW_WATCHDOG == 1)
	if (result == RET_OK)
	{
		result = WWDGInit();
	}
#endif
	return result;
}

/*************************************************************************//**
 * @brief  Starts all configured watchdogs.
 * 			The watchdogs must be started when the system is running normally,
 * 			Do not start in the "start phase" because could be any module that
 * 			need some delays triggering the watchdog reset.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
eError watchdogStart( void )
{
	eError result = RET_OK;
#if (INDEPENDENT_WATCHDOG == 1)
	result = IWDGStart();
#endif
#if (WINDOW_WATCHDOG == 1)
	result = WWDGStart();
#endif
	return result;
}

#if (INDEPENDENT_WATCHDOG == 1)
/*************************************************************************//**
 * @brief  Initialize independent watchdog.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
static eError IWDGInit( void )
{
	uint32_t auxValue;
	uint32_t auxWindow;
	uint16_t reloadVal;
	eError result = RET_OK;

	auxValue = LSI_VALUE;
	auxValue /= 8; //IWDG_PRESCALER_8
	if (IWDGMap.mSecs_LowWindow == IWDG_WINDOW_DISABLE)
	{
		IWDGHandler.Init.Window = IWDG_WINDOW_DISABLE;
	}
	else
	{
		auxWindow = auxValue * IWDGMap.mSecs_LowWindow;
		auxWindow /= 1000; //sec to mSec
		IWDGHandler.Init.Window = auxWindow;
	}
	auxValue *= IWDGMap.mSecs_UntilReset;
	reloadVal = auxValue / 1000; //sec to mSec

	IWDGHandler.Instance = IWDG;
	IWDGHandler.Init.Prescaler = IWDG_PRESCALER_8;
	IWDGHandler.Init.Reload = reloadVal;
	if (HAL_IWDG_Init(&IWDGHandler) != HAL_OK)
	{
		result = RET_FAIL;
	}
	return result;
}
/*************************************************************************//**
 * @brief  Start independent watchdog.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
static eError IWDGStart( void )
{
	eError result = RET_OK;

	clearWatchdog(I_WATCHDOG);

	HAL_IWDG_Start(&IWDGHandler);

	return result;
}
#endif

#if (WINDOW_WATCHDOG == 1)
/*************************************************************************//**
 * @brief  Initialize Window watchdog.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
static eError WWDGInit( void )
{
	uint32_t auxValue;
	uint32_t auxWindow;
	uint32_t apb1Clock;
	eError result = RET_OK;

	__HAL_RCC_WWDG_CLK_ENABLE();

	result = getAPB1Clock(&apb1Clock);

	if (result == RET_OK)
	{
		auxValue = apb1Clock;
		auxValue /= 4096; // Value corresponding to internal WWDG divider
		auxValue /= 8; //WWDG_PRESCALER_8

		/* Window config*/
		auxWindow = auxValue * WWDGMap.mSecs_LowWindow;
		auxWindow /= 1000; //sec to mSec
		auxWindow += 0x40; // from 0x40 to 0x80;
		if (auxWindow > 0x7F) //Maximum value
		{
			auxWindow = 0x7F;
		}

		/* Counter config */
		auxValue = auxValue * WWDGMap.mSecs_UntilReset;
		auxValue /= 1000; //sec to mSec
		WWDGCounterValue = auxValue + 0x40;
		if (WWDGCounterValue > 0x7F) //Maximum value
		{
			WWDGCounterValue = 0x7F;
		}

		WWDGHandler.Instance = WWDG;
		WWDGHandler.Init.Prescaler = WWDG_PRESCALER_8;
		WWDGHandler.Init.Window  = auxWindow;
		WWDGHandler.Init.Counter = WWDGCounterValue;
		if (HAL_WWDG_Init(&WWDGHandler) != HAL_OK)
		{
			result = RET_FAIL;
		}
	}
	else
	{
		result = RET_FAIL;
	}

	return result;
}

/*************************************************************************//**
 * @brief  Start window watchdog.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
static eError WWDGStart( void )
{
	eError result = RET_OK;

	clearWatchdog(W_WATCHDOG);

	HAL_WWDG_Start(&WWDGHandler);

	return result;
}
#endif

/*************************************************************************//**
 * @brief  get APB1 clock frequency needed to calculate
 * 				watchdogs in milliseconds instead clock cycles
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
eError getAPB1Clock (uint32_t *APBClock)
{
	eError result = RET_OK;

	if (APBClock != NULL)
	{
		uint32_t HCLK_Prescaler = RCC->CFGR & RCC_CFGR_HPRE;
		uint32_t APB1_Prescaler = RCC->CFGR & RCC_CFGR_PPRE1;
		uint32_t APB2_Prescaler = RCC->CFGR & RCC_CFGR_PPRE2;

		switch (HCLK_Prescaler)
		{
		case RCC_CFGR_HPRE_DIV1:
			*APBClock = SystemCoreClock;
			break;
		case RCC_CFGR_HPRE_DIV2:
			*APBClock = SystemCoreClock/2;
			break;
		case RCC_CFGR_HPRE_DIV4:
			*APBClock = SystemCoreClock/4;
			break;
		case RCC_CFGR_HPRE_DIV8:
			*APBClock = SystemCoreClock/8;
			break;
		case RCC_CFGR_HPRE_DIV16:
			*APBClock = SystemCoreClock/16;
			break;
		case RCC_CFGR_HPRE_DIV64:
			*APBClock = SystemCoreClock/64;
			break;
		case RCC_CFGR_HPRE_DIV128:
			*APBClock = SystemCoreClock/128;
			break;
		case RCC_CFGR_HPRE_DIV256:
			*APBClock = SystemCoreClock/256;
			break;
		case RCC_CFGR_HPRE_DIV512:
			*APBClock = SystemCoreClock/512;
			break;
		default:
			result = RET_FAIL;
			break;
		}

		switch(APB1_Prescaler)
		{
		case RCC_CFGR_PPRE1_DIV1:
			/* Do nothing */
			break;
		case RCC_CFGR_PPRE1_DIV2:
			*APBClock /= 2;
			break;
		case RCC_CFGR_PPRE1_DIV4:
			*APBClock /= 4;
			break;
		case RCC_CFGR_PPRE1_DIV8:
			*APBClock /= 8;
			break;
		case RCC_CFGR_PPRE1_DIV16:
			*APBClock /= 16;
			break;
		}

		switch(APB2_Prescaler)
		{
		case RCC_CFGR_PPRE2_DIV1:
			/* Do nothing */
			break;
		case RCC_CFGR_PPRE2_DIV2:
			*APBClock /= 2;
			break;
		case RCC_CFGR_PPRE2_DIV4:
			*APBClock /= 4;
			break;
		case RCC_CFGR_PPRE2_DIV8:
			*APBClock /= 8;
			break;
		case RCC_CFGR_PPRE2_DIV16:
			*APBClock /= 16;
			break;
		}
	}
	else
	{
		result = RET_FAIL;
	}

	return result;
}

#if defined(MON_RTC_ENABLED)
/*************************************************************************//**
 * @brief  Init RTC module.
 * 			RTC Backup is not present in F4 value line microcontrollers (STM32F4x0),
 * 			ie, the RTC cannot maintains the date and time after VCC disconnection.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
static void RTCInit( void )
{
	uint32_t aux;

	/* Enable PWR clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* Enable RTC peripheral */
	__HAL_RCC_RTC_ENABLE();

	/* Enables access to the backup domain (RTC registers, 		*/
	/* RTC backup data registers when present). 				*/
	HAL_PWR_EnableBkUpAccess();

	RTCHandler.Instance = RTC;
	RTCHandler.Init.HourFormat = RTC_HOURFORMAT_24;
	RTCHandler.Init.AsynchPrediv = 127;

	/* Calculate divisor to achieve 1Hz RTC */
	if (SystemMap.RTCClockSelection == RCC_RTCCLKSOURCE_LSE)
	{
		aux = LSE_VALUE;
	}
	else if (SystemMap.RTCClockSelection == RCC_RTCCLKSOURCE_LSI) //40 KHz
	{
		aux = LSI_VALUE;
	}
	else
	{
		uint32_t temp = SystemMap.RTCClockSelection & 0xFFFF000U;
		temp = temp / 0x10000U;
		aux = HSE_VALUE/temp;
	}

	aux /= (RTCHandler.Init.AsynchPrediv + 1);

	RTCHandler.Init.SynchPrediv = aux;
	RTCHandler.Init.OutPut = RTC_OUTPUT_DISABLE;
	RTCHandler.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RTCHandler.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	HAL_RTC_Init(&RTCHandler);

	HAL_RTC_WaitForSynchro(&RTCHandler);

	RTCSetInitDateTime();

}

/*************************************************************************//**
 * @brief   Sets the initial date and time and if it is present writes
 * 			the backup register in order to maintains the date and time at every reset
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
eError RTCSetInitDateTime( void )
{
	eError result = RET_OK;

	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	sTime.Hours = 0x0;
	sTime.Minutes = 0x0;
	sTime.Seconds = 0x0;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (RTCHandler.Instance != NULL) //RTC initialized
	{
		if (HAL_RTC_SetTime(&RTCHandler, &sTime, FORMAT_BCD) != HAL_OK)
		{
			result = RET_FAIL;
		}
		if (result == RET_OK)
		{
			sDate.WeekDay = RTC_WEEKDAY_MONDAY;
			sDate.Month = RTC_MONTH_JANUARY;
			sDate.Date = 0x1;
			sDate.Year = 0x0;
			if (HAL_RTC_SetDate(&RTCHandler, &sDate, FORMAT_BCD) != HAL_OK)
			{
				result = RET_FAIL;
			}
			else
			{

#if (RTC_CALIBRATION_TEST == 1)
		HAL_RTCEx_SetCalibrationOutPut(&hrtc, RTC_CALIBOUTPUT_1HZ);
#endif
			}
		}
	}
	return result;
}

/*************************************************************************//**
 * @brief	RTC Set time function
 *
 * @param	uint32.
 *
 * @return  None.
 *
 ****************************************************************************/
eError RTCSetTime (uint32_t time)
{
	eError result = RET_OK;
	RTC_TimeTypeDef sTime;

	if (RTCHandler.Instance != NULL)
		//RTC is initialized
	{
		sTime.Hours 	= (uint8_t)(time >> 24);
		sTime.Minutes 	= (uint8_t)(time >> 16);
		sTime.Seconds	= (uint8_t)(time >> 8);
		sTime.SubSeconds = (uint8_t)(time);

		if (HAL_RTC_SetTime(&RTCHandler, &sTime, FORMAT_BIN) != HAL_OK)
		{
			result = RET_FAIL;
		}
	}
	else
	{
		result = RET_FAIL;
	}

	return result;
}

/*************************************************************************//**
 * @brief
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
eError RTCGetTime (uint32_t* time)
{
	eError result = RET_OK;

	RTC_TimeTypeDef sTime;

	if ((RTCHandler.Instance != NULL) && (time != NULL))
		//RTC is initialized and time pointer is valid
	{
		if (HAL_RTC_GetTime(&RTCHandler, &sTime, FORMAT_BIN) == HAL_OK)
		{
			*time = MON_MAKELONG_Byte(sTime.Hours, sTime.Minutes, sTime.Seconds, (uint8_t)sTime.SubSeconds);
		}
		else
		{
			result = RET_FAIL;
		}
	}
	return result;
}

/*************************************************************************//**
 * @brief
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
eError RTCSetDate (uint32_t date)
{
	eError result = RET_OK;
	RTC_DateTypeDef sDate;

	if (RTCHandler.Instance != NULL)
		//RTC is initialized
	{
		sDate.Year 	= (uint8_t)(date >> 24);
		sDate.Month 	= (uint8_t)(date >> 16);
		sDate.Date	= (uint8_t)(date >> 8);
		sDate.WeekDay = (uint8_t)(date);

		if (HAL_RTC_SetDate(&RTCHandler, &sDate, FORMAT_BIN) != HAL_OK)
		{
			result = RET_FAIL;
		}
	}
	else
	{
		result = RET_FAIL;
	}
	return result;
}


/*************************************************************************//**
 * @brief
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError RTCGetDate (uint32_t *date)
{
	eError result = RET_OK;

	RTC_DateTypeDef sDate;
	if ((RTCHandler.Instance != NULL) && (date != NULL))
		//RTC is initialized and time pointer is valid
	{
		if (HAL_RTC_GetDate(&RTCHandler, &sDate, FORMAT_BIN) == HAL_OK)
		{
			*date = MON_MAKELONG_Byte(sDate.Year, sDate.Month, sDate.Date, sDate.WeekDay);
		}
		else
		{
			result = RET_FAIL;
		}
	}
	else
	{
		result = RET_FAIL;
	}
	return result;
}
#endif



/*************************************************************************//**
 * @brief  Processes requests of state change for microcontroller.
 *
 * @param	New command to be applied.
 *
 * @return  result.
 *
 ****************************************************************************/
eError stm32F4SetPowerMode( tSystemPowerModes powerMode )
{
	eError result = RET_OK;
	if (IS_POWER_MODE(powerMode))
	{
		switch ( powerMode )
		{
		case SYSTEM_EXECUTE_BOOTLOADER:

			break;

		case SYSTEM_EXECUTE_STANDBY:

			/* Enable WKUP pin  */
			__PWR_CLK_ENABLE();
			/* Clear wake up flag */
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
			/* Enable Wake up pin 1 interrupt */
			HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
			/* Enter Standby mode */
			HAL_PWR_EnterSTANDBYMode();
			while (1)
			{}

			break;

		case SYSTEM_EXECUTE_SLEEP:

			//gpioWakeUp = GPIO_UART_UPDATE_RX;
			//initWakeUpInterruption(GPIO_USB_INSERTED);
			//initWakeUpInterruption(gpioWakeUp);

			/* Disable Signal Generator interrupt */
			//HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
			//disableSysticInterrupt();


			//HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

			//enableSysticInterrupt();
			//deInitWakeUpInterruption(gpioWakeUp);
			/* Enable Signal Generator interrupt */
			//HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

			break;

		case SYSTEM_EXECUTE_RESET:
			NVIC_SystemReset();
			break;

		default:
			break;
		}

	}
	return result;
}

/*************************************************************************//**
 * @brief  System Clock Configuration: It is generated by STCubeMx tool
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
static eError SystemClock_Config(void)
{

	eError result = RET_OK;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
#if defined(MON_RTC_ENABLED)
	RCC_PeriphCLKInitTypeDef PeriphClkInit;
#endif

	RCC_OscInitStruct.OscillatorType = SystemMap.OscillatorType;
	RCC_OscInitStruct.HSEState	= SystemMap.HSEState;
	RCC_OscInitStruct.HSICalibrationValue = SystemMap.HSICalibrationValue;
	RCC_OscInitStruct.HSIState 	= SystemMap.HSIState;
	RCC_OscInitStruct.LSEState = SystemMap.LSEState;
	RCC_OscInitStruct.LSIState = SystemMap.LSIState;

	RCC_OscInitStruct.PLL.PLLState 	= SystemMap.PLLState;
	RCC_OscInitStruct.PLL.PLLSource = SystemMap.PLLSource;
	RCC_OscInitStruct.PLL.PLLM 		= SystemMap.PLLM;
	RCC_OscInitStruct.PLL.PLLN		= SystemMap.PLLN;
	RCC_OscInitStruct.PLL.PLLP		= SystemMap.PLLP;
	RCC_OscInitStruct.PLL.PLLQ		= SystemMap.PLLQ;

#if defined(STM32F410Tx) || defined(STM32F410Cx) || defined(STM32F410Rx) || defined(STM32F446xx) || defined(STM32F469xx) ||\
    defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx) || defined(STM32F412Rx) || defined(STM32F412Cx)
	RCC_OscInitStruct.PLL.PLLR		= SystemMap.PLLR;
#endif

#if (INDEPENDENT_WATCHDOG == 1)//Assure that LSI is configured ON (needed by IWDG)
	RCC_OscInitStruct.OscillatorType |= RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.LSIState |= RCC_LSI_ON;
#endif

	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource 	 = SystemMap.SYSCLKSource;
	RCC_ClkInitStruct.AHBCLKDivider  = SystemMap.AHBCLKDivider;
	RCC_ClkInitStruct.APB1CLKDivider = SystemMap.APB1CLKDivider;
	RCC_ClkInitStruct.APB2CLKDivider = SystemMap.APB2CLKDivider;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

#if defined(MON_RTC_ENABLED)
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInit.RTCClockSelection = SystemMap.RTCClockSelection;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
#endif

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	__HAL_RCC_SYSCFG_CLK_ENABLE();

	return result;

}



/*************************************************************************//**
 * @brief   Get the System CLK Frequency
 * @param   ClockFrequency.
 * @return  eError.
 ****************************************************************************/
eError systemGetCLkFreq(uint32_t *ClockFrequency)
{
    eError result = RET_OK;

    *ClockFrequency = HAL_RCC_GetHCLKFreq();

    return result;
}



/*************************************************************************//**
 * @brief  Refreshes watchdog
 * @param	None.
 * @return  None.
 ****************************************************************************/
void clearWatchdog(tWatchdog watchdog)
{
	if IS_WATCHDOG(watchdog)
	{
	#if (WINDOW_WATCHDOG == 1)
		if (watchdog == W_WATCHDOG)
		{
			/* Refresh WWDG */
			HAL_WWDG_Refresh(&WWDGHandler, WWDGCounterValue);
		}
	#endif
	#if (INDEPENDENT_WATCHDOG == 1)
		if (watchdog == I_WATCHDOG)
		{
			/* Refresh IWDG */
			HAL_IWDG_Refresh(&IWDGHandler);
		}
	#endif
	}
}

/*************************************************************************//**
 * @brief Remap vector table if necessary
 * @detail cortex-m0 has a static-address vector table. If the Firmware
 * is configured to reside in a different direction than the default
 * one we must remap that vector table to fit in 0x0000 0000
 * From the programming manual:
 *  - Copy the vector table from the Flash (mapped at the base of the application
 *    load address) to the base address of the SRAM at 0x2000 0000.
 *  - Remap SRAM at address 0x0000 0000, using SYSCFG configuration register 1.
 *  - Then once an interrupt occurs, the Cortex ® -M0 processor will fetch the
 *    interrupt handler start address from the relocated vector table in SRAM,
 *    then it will jump to execute the interrupt handler located in the Flash
 * @param	None.
 * @return  None.
 ****************************************************************************/
void systemPreConfiguration( void )
{

#ifdef BOOTLOADER_PRESENT
	tFlash currentFlashArea;
    uint32_t FWStartAddress = 0;

	if ( FlashActualInstance(&currentFlashArea) == RET_OK )
	{
		FWStartAddress = FlashGetStartAddress(currentFlashArea);
		if ( FWStartAddress != FLASH_INITIAL_ADDRESS )
		{
		    SCB->VTOR = FWStartAddress;
		}
	}
#endif

}

/*************************************************************************//**
 * @brief Perform a soft reset of the system.
 * @param	None.
 * @return  None.
 ****************************************************************************/
void systemSoftReset(void)
{
    NVIC_SystemReset();
}

/*************************************************************************//**
 * @brief  Remaps memory and jumps to boot position. If there's a bootloader
 * that's what will be running. Otherwise just restarts the application
 * @param	None.
 * @return  None.
 ****************************************************************************/
void systemJumpToBoot(void)
{
    pFunction bootEntry;
    uint32_t bootStack;

    __disable_irq();

    bootStack = (uint32_t)  *(volatile unsigned int*) BOOT_INITIAL_ADDRESS;
    bootEntry = (pFunction) *(volatile unsigned int*) BOOT_ENTRY_ADDRESS;

    __set_MSP(bootStack);

    bootEntry();
}

#endif // _SYSTEM_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
