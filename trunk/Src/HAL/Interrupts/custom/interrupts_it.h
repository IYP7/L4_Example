/*****************************************************************************
 * @file interrupts_it.h
 * @addtogroup HAL
 * @{
 * @addtogroup INTERRUPTS
 * @{
 *
 * @brief HAL interrupts_it Header.
 *
 * @author Dario Davide
 *
 * @date 25/01/2016
 *
 ****************************************************************************/

#ifndef INTERRUPTS_IT_H_
#define INTERRUPTS_IT_H_

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void RTC_WKUP_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void ADC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void I2C2_EV_IRQHandler(void);

#endif /* INTERRUPTS_IT_H_ */
