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
		 * Port 			--> GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOH
		 * GPIO_pins  		--> GPIO_PIN_X
		 * GPIO_mode 		--> GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_MODE_OUTPUT_OD, GPIO_MODE_AF_PP,
		 * 						GPIO_MODE_AF_OD, GPIO_MODE_ANALOG, GPIO_MODE_ANALOG_ADC_CONTROL
		 * GPIO_speed 		--> GPIO_SPEED_FREQ_LOW, GPIO_SPEED_FREQ_MEDIUM, GPIO_SPEED_FREQ_HIGH, GPIO_SPEED_FREQ_VERY_HIGH
		 * GPIO_pull 		--> GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
		 * GPIO_AF		-->
		 * GPIO_Init_Value 	--> GPIO_LOW, GPIO_HIGH, GPIO_TOGGLE, GPIO_DEFAULT        */

		/* Leds mounted on STM32L0-discovery */
		/* GPIO_LED_2               	*/ { GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL, GPIO_AF_NONE, GPIO_DEFAULT },

		/* Button mounted on STM32L0-discovery */
		/* GPIO_BUTTON_B1          		*/ { GPIOC, GPIO_PIN_13,GPIO_MODE_IT_RISING,GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL, GPIO_AF_NONE, GPIO_DEFAULT },

		/* GPIO_PWM         	  		*/ { GPIOA, GPIO_PIN_0, GPIO_MODE_AF_PP, GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL, GPIO_AF1_TIM1, GPIO_DEFAULT },
		/* GPIO_PWM_2         	  		*/ { GPIOA, GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL, GPIO_AF1_TIM1, GPIO_DEFAULT },

		/* External Buttons for Buttons driver test */
		/* GPIO_BUTTON_B2          		*/ { GPIOB, GPIO_PIN_5, GPIO_MODE_INPUT,GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL, GPIO_AF_NONE, GPIO_DEFAULT },
		/* GPIO_BUTTON_B3           	*/ { GPIOB, GPIO_PIN_4, GPIO_MODE_INPUT,GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL, GPIO_AF_NONE, GPIO_DEFAULT },
		
		/*external Leds for Leds driver test */
		/* GPIO_LED_1          	  		*/ { GPIOB, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_HIGH, GPIO_NOPULL, GPIO_AF_NONE, GPIO_DEFAULT },

		/* UART2 for ST-NUCLEO-F030R8*/
			/* TX                       */ { GPIOA, GPIO_PIN_2 , GPIO_MODE_AF_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP, GPIO_AF7_USART2, GPIO_DEFAULT },
			/* RX                       */ { GPIOA, GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP, GPIO_AF7_USART2, GPIO_DEFAULT },
		/* UART1 for ST-NUCLEO-F030R8*/
		///* TX                       */ { GPIOA, GPIO_PIN_9 , GPIO_MODE_AF_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP, GPIO_AF7_USART1, GPIO_DEFAULT },
		///* RX                       */ { GPIOA, GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_SPEED_FREQ_HIGH, GPIO_PULLUP, GPIO_AF7_USART1, GPIO_DEFAULT },

		/* SPI1 */
			/* GPIO_SPI1_MISO 			*/ { GPIOA, GPIO_PIN_6,  GPIO_MODE_AF_PP,	  	GPIO_SPEED_FREQ_LOW, GPIO_NOPULL, GPIO_AF5_SPI1, GPIO_DEFAULT },
			/* GPIO_SPI1_MOSI			*/ { GPIOA, GPIO_PIN_7,  GPIO_MODE_AF_PP,	 	GPIO_SPEED_FREQ_LOW, GPIO_NOPULL, GPIO_AF5_SPI1, GPIO_DEFAULT },
			/* GPIO_SPI1_CLK/SCK		*/ { GPIOA, GPIO_PIN_1,  GPIO_MODE_AF_PP,	  	GPIO_SPEED_FREQ_LOW, GPIO_PULLDOWN, GPIO_AF5_SPI1, GPIO_DEFAULT },
//			/* GPIO_SPI1_NSS			*/ { GPIOA, GPIO_PIN_4,  GPIO_MODE_INPUT_PP, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL, GPIO_AF5_SPI1, GPIO_DEFAULT },

		/* SPI3 */
//			/* GPIO_SPI3_MISO 			*/ { GPIOC, GPIO_PIN_12,  GPIO_MODE_AF_PP,	  	GPIO_SPEED_FREQ_LOW, GPIO_NOPULL, GPIO_AF6_SPI3, GPIO_DEFAULT },
//			/* GPIO_SPI3_MOSI			*/ { GPIOC, GPIO_PIN_11,  GPIO_MODE_AF_PP,	 	GPIO_SPEED_FREQ_LOW, GPIO_NOPULL, GPIO_AF6_SPI3, GPIO_DEFAULT },
//			/* GPIO_SPI3_CLK/SCK		*/ { GPIOC, GPIO_PIN_10,  GPIO_MODE_AF_PP,	  	GPIO_SPEED_FREQ_LOW, GPIO_PULLDOWN, GPIO_AF6_SPI3, GPIO_DEFAULT },
//			/* GPIO_SPI3_NSS			*/ { GPIOA, GPIO_PIN_15,  GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL, GPIO_AF6_SPI3, GPIO_DEFAULT },

		/* SDIO - SD */
			/* GPIO_SDIO_D0 			*/ {GPIOC, GPIO_PIN_8, GPIO_MODE_AF_PP, GPIO_SPEED_FAST, GPIO_PULLUP, GPIO_AF12_SDIO, GPIO_DEFAULT},
			/* GPIO_SDIO_D1 			*/ {GPIOC, GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_SPEED_FAST, GPIO_PULLUP, GPIO_AF12_SDIO, GPIO_DEFAULT},
			/* GPIO_SDIO_D2 			*/ {GPIOC, GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_SPEED_FAST, GPIO_PULLUP, GPIO_AF12_SDIO, GPIO_DEFAULT},
			/* GPIO_SDIO_D3 			*/ {GPIOC, GPIO_PIN_11, GPIO_MODE_AF_PP, GPIO_SPEED_FAST, GPIO_PULLUP, GPIO_AF12_SDIO, GPIO_DEFAULT},
			/* GPIO_SDIO_CK 			*/ {GPIOC, GPIO_PIN_12, GPIO_MODE_AF_PP, GPIO_SPEED_FAST, GPIO_NOPULL, GPIO_AF12_SDIO, GPIO_DEFAULT},
			/* GPIO_SDIO_CMD 			*/ {GPIOD, GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_SPEED_FAST, GPIO_PULLUP, GPIO_AF12_SDIO, GPIO_DEFAULT},

			/* GPIO_DRAIN_CURRENT 		*/ {GPIOA, GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_HIGH, GPIO_NOPULL, GPIO_AF_NONE, GPIO_HIGH}




};

#endif // _GPIO_MAP_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
