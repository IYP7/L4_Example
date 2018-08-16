/*****************************************************************************
 * @file 	GPIO_map.c
 *
 * @brief 	 GPIO map control.
 *
 * @author 	FV4MMA2
 *
 * @date 	28/01/2016
 *
 ****************************************************************************/
#ifndef _GPIO_MAP_C_
#define _GPIO_MAP_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "GPIO.h"

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/

const tGPIOInstanceMap GPIOInstanceMap[NUM_OF_GPIO] =
{
		/* {Port, GPIO_pins, GPIO_mode, GPIO_speed, GPIO_pull, GPIO_AF, GPIO_Init_Value }
		 *
		 * Port 			--> GPIOA, GPIOB, GPIOC, GPIOD, GPIOH
		 * GPIO_pins  		--> GPIO_PIN_X
		 * GPIO_mode 		--> GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_MODE_OUTPUT_OD, GPIO_MODE_AF_PP,
		 * 						GPIO_MODE_AF_OD, GPIO_MODE_ANALOG
		 * GPIO_speed 		--> GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_HIGH
		 * GPIO_pull 		--> GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
		 * GPIO_AF		-->
		 * GPIO_Init_Value 	--> GPIO_LOW, GPIO_HIGH, GPIO_TOGGLE, GPIO_DEFAULT        */

		/* Leds mounted on STM32L0-discovery */
		/* GPIO_LED_2               */ { GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_HIGH, GPIO_NOPULL, GPIO_AF_NONE, GPIO_DEFAULT },

		/* Button mounted on STM32L0-discovery */
		/* GPIO_BUTTON_B1           */ { GPIOC, GPIO_PIN_13,GPIO_MODE_IT_RISING,GPIO_SPEED_HIGH, GPIO_NOPULL, GPIO_AF_NONE, GPIO_DEFAULT },	

		/* GPIO_PWM         	  */ { GPIOA, GPIO_PIN_0, GPIO_MODE_AF_PP, GPIO_SPEED_HIGH, GPIO_NOPULL, GPIO_AF1_TIM1, GPIO_DEFAULT },
		/* GPIO_PWM_2         	  */ { GPIOA, GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_SPEED_HIGH, GPIO_NOPULL, GPIO_AF1_TIM1, GPIO_DEFAULT },

		/* External Buttons for Buttons driver test */
		/* GPIO_BUTTON_B2           */ { GPIOB, GPIO_PIN_5, GPIO_MODE_INPUT,GPIO_SPEED_HIGH, GPIO_NOPULL, GPIO_AF_NONE, GPIO_DEFAULT },
		/* GPIO_BUTTON_B3           */ { GPIOB, GPIO_PIN_4, GPIO_MODE_INPUT,GPIO_SPEED_HIGH, GPIO_NOPULL, GPIO_AF_NONE, GPIO_DEFAULT },
		
		/*external Leds for Leds driver test */
		/* GPIO_LED_1          	  */ { GPIOB, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_HIGH, GPIO_NOPULL, GPIO_AF_NONE, GPIO_DEFAULT },

		/* UART2 for ST-NUCLEO-F030R8*/
		/* TX                       */ { GPIOA, GPIO_PIN_2 , GPIO_MODE_AF_PP, GPIO_SPEED_HIGH, GPIO_PULLUP, GPIO_AF7_USART2, GPIO_DEFAULT },
		/* RX                       */ { GPIOA, GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_SPEED_HIGH, GPIO_PULLUP, GPIO_AF7_USART2, GPIO_DEFAULT },
		/* UART1 for ST-NUCLEO-F030R8*/
		///* TX                       */ { GPIOA, GPIO_PIN_9 , GPIO_MODE_AF_PP, GPIO_SPEED_HIGH, GPIO_PULLUP, GPIO_AF7_USART1, GPIO_DEFAULT },
		///* RX                       */ { GPIOA, GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_SPEED_HIGH, GPIO_PULLUP, GPIO_AF7_USART1, GPIO_DEFAULT },


};

#endif // _GPIO_MAP_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
