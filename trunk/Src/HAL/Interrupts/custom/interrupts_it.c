/*****************************************************************************
 * @file 	interrupts_it.c
 * @ingroup HAL
 * @date	27/01/2016
 * @Author  Dario Davide
 * @addtogroup HAL
 * @{
 * @addtogroup INTERRUPTS
 * @{
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * The interrupt service routine could be configured using the HAL_Library
 * service IRQ routine, or/and user service IRQ routine. For this, the Handler IRQ routine
 * related to IRQ Channel has to included the call to :
 *
 *  - user routine :        HAL IRQ callback (defined on iterrupts_map)
 *
 *  - HAL_library routine : HAL Library IRQ routine (defined on STM3_xx_HAL_Library)
 *
 *******************************************************************************/

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "common.h"

/***********************************************************************
 * ARCHITECTURE INCLUDES
 **********************************************************************/
#include "Hreg_api.h"
/***********************************************************************
 * INCLUDES
 **********************************************************************/
#include "interrupts_it.h"
#include "interrupts_map.h"

/***********************************************************************
 * DEFINES
 **********************************************************************/

/***********************************************************************
 * TYPE DEFINITIONS
 **********************************************************************/

/***********************************************************************
 * EXTERN VARIABLES
 **********************************************************************/
/* RTC handler declared in "main.c or DriverRtc.c" file */
/*TODO: extern RTC_HandleTypeDef RtcHandle; */
/* ADC handler declared in "main.c" or DriverRtc.c" file */
/*TODO: extern ADC_HandleTypeDef    AdcHandle; */
/* TIMX handler declared in "main.c" or DriverRtc.c" file */
#ifdef TIMER_USES_INTERRUPTS
extern TIM_HandleTypeDef    sTIMHandle[NUM_OF_PERIPHERAL_TIMERS];
extern tTimerInstanceMap 	TimerInstanceMap[NUM_OF_TIMERS];
#endif //TIMER_USES_INTERRUPTS


/***********************************************************************
 * PUBLIC VARIABLES
 **********************************************************************/

/***********************************************************************
 * PRIVATE VARIABLES
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUCNTION PROTOTYPES
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUCNTIONS
 **********************************************************************/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}


/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void) {

	HAL_SYSTICK_IRQHandler();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/


/**
  * @brief  This function handles RTC Auto wake-up interrupt request.
  * @param  None
  * @retval None
  */
void RTC_WKUP_IRQHandler(void)
{
	/*TODO :1)RtcHandle could be defined in the user file  */
	//HAL_RTCEx_TamperTimeStampIRQHandler(&RtcHandle);
}


/**
 * @brief  This function handles external line 0 and 1 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI0_IRQHandler(void)
{
	/*TODO : the HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_X) could be commented for GPIO_PIN unused */
	//HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief  This function handles external line 0 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI1_IRQHandler(void)
{
	/*TODO : the HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_X) could be commented for GPIO_PIN unused */
	//HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI9_5_IRQHandler(void)
{
	/*TODO : the HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_X) could be commented for GPIO_PIN unused */
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);

}

/**
* @brief  This function handles COMP interrupt request.
* @param  None
* @retval None
*/
void ADC_IRQHandler(void)
{
  /*TODO :1)AdcHandle could be defined in the user file  */
   //HAL_ADC_IRQHandler(&AdcHandle);
}



/**
  * @brief  This function handles UART1 interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA
  *         used for USART data transmission
  */
void USART1_IRQHandler(void)
{
	HAL_UART1_Callback();
}
/**
  * @brief  This function handles UART2 interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA
  *         used for USART data transmission
  */

void USART2_IRQHandler(void)
{
	HAL_UART2_Callback();
}

/**
  * @brief  This function handles I2C1 interrupt request.
  * @param  None
  * @retval None
  */
void I2C1_EV_IRQHandler(void)
{
	HAL_I2C1_Callback();
}

/**
  * @brief  This function handles I2C1 interrupt request.
  * @param  None
  * @retval None
  */
void I2C2_EV_IRQHandler(void)
{
	HAL_I2C2_Callback();
}

/**
  * @brief  This function handles SPI2 interrupt request.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{
    HAL_SPI1_Callback();
}

/**
  * @brief  This function handles SPI2 interrupt request.
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void)
{
    HAL_SPI2_Callback();
}

void SPI3_IRQHandler(void)
{
    HAL_SPI3_Callback();
}
/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
#ifdef TIM2_IRQ_ENABLE
	HAL_TIM_IRQHandler(&sTIMHandle[TimerInstanceMap[TIM2_IRQ_INSTANCE].timerDevice]);
#endif
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
#ifdef TIM3_IRQ_ENABLE
	HAL_TIM_IRQHandler(&sTIMHandle[TimerInstanceMap[TIM3_IRQ_INSTANCE].timerDevice]);
#endif
}

/****************************************************************************
 *    End of file
 *    }@}@
 ***************************************************************************/
