/*****************************************************************************
 * @file 	GPIO_map.h
 *
 * @brief 	GPIO functionality Header.
 *
 * @author 	FV4MMA2
 *
 * @date 	28/01/2016
 *
 ****************************************************************************/
#ifndef _GPIO_MAP_H_
#define _GPIO_MAP_H_

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

/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

typedef enum eGPIO
{
	GPIO_LED_2,
	GPIO_BUTTON_B1,
	GPIO_PWM,
	GPIO_PWM_2,
	
	GPIO_BUTTON_B2,
	GPIO_BUTTON_B3,
	
	GPIO_LED_1,

    GPIO_UART2_TX,
    GPIO_UART2_RX,



    GPIO_SPI1_MISO,
    GPIO_SPI1_MOSI,
    GPIO_SPI1_SCK,


	GPIO_SPI3_MISO,
	GPIO_SPI3_MOSI,
	GPIO_SPI3_SCK,





    NUM_OF_GPIO
} tGPIO;

#endif // _GPIO_MAP_H_

/****************************************************************************
 *    End of file
 ***************************************************************************/
