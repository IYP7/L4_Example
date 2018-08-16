/***********************************************************************
 * @file interrupts_map.c
 * @ingroup HAL
 * @date 27/01/2016
 * @Author Dario Davide
 *
 * @addtogroup HAL
 * @{
 * @addtogroup INTERRUPTS
 * @{

 * @brief INTERRUPTS map declaration. To be defined for every project's needs
 *
 * @details
 * Define here the IRQ Channels configuration.
 * Note that :
 * - the map configuration has be defined in accordance with module driver related to IRQ
 * - For each IRQ Channel declared one IRQhandler function has to be implemented on interrupts_it files
 *
 * INTERRUPTS driver can re-map the IRQ Handler Call to a specific Driver Controller, for this :
 * - the HAL IRQ Handler has be implemented using the HAL IRQ callback
 * - in HAL IRQ callback could be added Module Function defined on user Driver
 *
 **********************************************************************/

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "Common.h"

/***********************************************************************
 * ARCHITECTURE INCLUDES
 **********************************************************************/
#include "hreg_dat.h"
#include "Scheduler.h"
#ifdef SW_TIMER_AVAILABLE
#include "SwTimer.h"
#endif // SW_TIMER_AVAILABLE

/***********************************************************************
 * INCLUDES
 **********************************************************************/
#include "interrupts.h"
#include "interrupts_map.h"

#ifdef MON_TEST_IRQ
#include "test_mw.h"
#endif
/***********************************************************************
 * DEFINES
 **********************************************************************/
/***********************************************************************
 * TYPE DEFINITIONS
 **********************************************************************/

/***********************************************************************
 * EXTERN VARIABLES
 **********************************************************************/

/***********************************************************************
 * PUBLIC VARIABLES
 **********************************************************************/


tInterruptsInstanceMap interruptsInstanceMap[MAX_IRQ] =
{
		/* IRQ_SYSTICK		*/	{SysTick_IRQn		, 	TICK_INT_PRIORITY,	INT_SUBPRIORITY_0,		IRQ_ENABLE	},
		/* IRQ_RTC,			*/	{RTC_WKUP_IRQn		, 	INT_PRIORITY_1   ,	INT_SUBPRIORITY_0,	    IRQ_DISABLE	},
		/* IRQ_EXTI0		*/	{EXTI0_IRQn			,	INT_PRIORITY_1   ,	INT_SUBPRIORITY_0,		IRQ_DISABLE	},
		/* IRQ_EXTI1		*/	{EXTI1_IRQn			,	INT_PRIORITY_1	 ,	INT_SUBPRIORITY_0,	    IRQ_DISABLE	},
		/* IRQ_EXTI9_5		*/	{EXTI9_5_IRQn		,	INT_PRIORITY_1	 ,	INT_SUBPRIORITY_0,		IRQ_DISABLE	},
		/* IRQ_ADC1			*/	{ADC1_IRQn			,	INT_PRIORITY_1	 ,	INT_SUBPRIORITY_0,		IRQ_DISABLE	},
		/* IRQ_TIM2			*/ 	{TIM2_IRQn			,	INT_PRIORITY_1	 ,	INT_SUBPRIORITY_0,		IRQ_DISABLE	},
		/* IRQ_TIM3			*/ 	{TIM3_IRQn			,	INT_PRIORITY_1	 ,	INT_SUBPRIORITY_0,		IRQ_DISABLE	},
		/* IRQ_USART1		*/	{USART1_IRQn		,	INT_PRIORITY_1	 ,	INT_SUBPRIORITY_0,		IRQ_DISABLE	},
		/* IRQ_USART2		*/	{USART2_IRQn		,	INT_PRIORITY_1	 ,	INT_SUBPRIORITY_0,		IRQ_DISABLE	},
		/* IRQ_I2c1  		*/	{I2C1_EV_IRQn       ,	INT_PRIORITY_1	 ,	INT_SUBPRIORITY_0,		IRQ_DISABLE },
		/* IRQ_I2c2  		*/	{I2C2_EV_IRQn       ,	INT_PRIORITY_1	 ,	INT_SUBPRIORITY_0,		IRQ_DISABLE },
		/* IRQ_SPI1  		*/	{SPI1_IRQn          ,	INT_PRIORITY_1	 ,	INT_SUBPRIORITY_0,		IRQ_DISABLE },
		/* IRQ_SPI2  		*/	{SPI2_IRQn          ,	INT_PRIORITY_1	 ,	INT_SUBPRIORITY_0,		IRQ_DISABLE },
};


/***********************************************************************
 * PRIVATE VARIABLES
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUCNTION PROTOTYPES
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUCNTIONS
 **********************************************************************/

/***********************************************************************
 * PRIVATE FUCNTIONS
 **********************************************************************/
/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
	HAL_IncTick();

	SchedulerProcessesAutoEnable();
#ifdef SW_TIMER_AVAILABLE
	SwTimerIrq();
#endif //SW_TIMER_AVAILABLE
}

/**
  * @brief  TimeStamp callback.
  * @param  hrtc: RTC handle
  * @retval None
  */
//void HAL_RTCEx_TimeStampEventCallback(RTC_HandleTypeDef *hrtc)
//{
//	  /* NOTE :the HAL_RTCEx_Tamper2EventCallback could be implemented in the user driver
//	   */
//
//}

/**
  * @brief  Tamper 1 callback.
  * @param  hrtc: RTC handle
  * @retval None
  */
//void HAL_RTCEx_Tamper1EventCallback(RTC_HandleTypeDef *hrtc)
//{
//	  /* NOTE :the HAL_RTCEx_Tamper2EventCallback could be implemented in the user driver
//	   */
//
//}

/**
  * @brief  Tamper 2 callback.
  * @param  hrtc: RTC handle
  * @retval None
  */
//void HAL_RTCEx_Tamper2EventCallback(RTC_HandleTypeDef *hrtc)
//{
//  /* NOTE :the HAL_RTCEx_Tamper2EventCallback could be implemented in the user file
//   */
//}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion, and
  *         you can add your own implementation.
  * @retval None
  */
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
//{
//	/*TODO: "example" call to Driver Function*/
//}

#ifdef TIMER_USES_INTERRUPTS

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	timerDriverPEIRQHandler(htim);
}

/**
  * @brief  Input Capture callback in non blocking mode
  * @param  htim : TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	timerDriverICIRQHandler(htim);
}

#endif //TIMER_USES_INTERRUPTS

/**
  * @brief UART1 callback
  * @param None
  * @retval None
  */
void HAL_UART1_Callback(void)
{
#ifdef IS_UART1
   uartDriverMainIRQHandler(UART_1);
#endif
}

/**
  * @brief UART2 callback
  * @param None
  * @retval None
  */
void HAL_UART2_Callback(void)
{
#ifdef IS_UART2
	uartDriverMainIRQHandler(UART_2);
#endif
}


void HAL_I2C1_Callback(void)
{
#ifdef IS_I2C1
	i2cDriverMainIRQHandler(I2C_1);
#endif
}

void HAL_I2C2_Callback(void)
{
#ifdef IS_I2C2
	i2cDriverMainIRQHandler(I2C_2);
#endif
}

void HAL_SPI1_Callback(void)
{
#ifdef IS_SPI1
    spiDriverMainIRQHandler(SPI_1);
#endif
}

void HAL_SPI2_Callback(void)
{
#ifdef IS_SPI2
    spiDriverMainIRQHandler(SPI_2);
#endif
}

/****************************************************************************
 *    End of file
 *    }@}@
 ***************************************************************************/
