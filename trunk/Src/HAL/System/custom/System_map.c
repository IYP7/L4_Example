/*************************************************************************//**
 * @file System_map.c
 *
 * @brief System map control.
 *
 * @author Gonzalo Serrano
 * @author Albert Serrallo
 *
 * @date 10/02/2016
 *
 ****************************************************************************/

#ifndef _SYSTEM_MAP_C_
#define _SYSTEM_MAP_C_


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
#include "System.h"

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
const tSystemInstanceMap SystemMap = {

		/*------------------------------------------------------------------------------*/
		/* IMPORTANT INFORMATION: 														*/
		/* 	The easiest way to perform the clock configuration is using the				*/
		/*	STM32CubeMX software following the next steps: 								*/
		/* 	- Click New project and select the microcontroller used						*/
		/*	- Choose the minimum pinout used in the Pinout tab							*/
		/*	- Configure all clocks in the Clock Configuration tab 						*/
		/*	- Generate the code (CTRL + SHIFT + G)										*/
		/*	- Search the SystemClock_Config() function in the generated code.			*/
		/*	- Paste the configuration in this map structure								*/
		/*------------------------------------------------------------------------------*/


		/* OscillatorType 	--> RCC_OSCILLATORTYPE_NONE | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI |
		 * 						RCC_OSCILLATORTYPE_LSE	| RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_MSI |
		 * 					    RCC_OSCILLATORTYPE_HSI48
		 *
		 * HSEState  		-->	RCC_HSE_OFF, RCC_HSE_ON, RCC_HSE_BYPASS
		 * LSEState 		--> RCC_LSE_OFF, RCC_LSE_ON, RCC_LSE_BYPASS
		 * HSIState			--> RCC_HSI_OFF, RCC_HSI_ON, RCC_HSI_DIV4
		 * HSICalibration	--> 0x00 to 0x1F - Default: 0x10
		 * LSIState 		--> RCC_LSI_OFF, RCC_LSI_ON
		 *
		 * PLLState			--> RCC_PLL_NONE, RCC_PLL_OFF or RCC_PLL_ON
		 * PLLSource		--> RCC_PLLSOURCE_HSI, RCC_PLLSOURCE_HSE
		 * PLLM 			--> 1 - 16
		 * PLLN				--> 1 - 350
		 * PLLP 			--> 1 - 16
		 * PLLQ				--> 1, 2, 4, 8, 16
		 * PLLR				--> 1, 2, 4, 8
		 *
		 * SYSCLKSource		--> RCC_SYSCLKSOURCE_MSI, RCC_SYSCLKSOURCE_HSI, RCC_SYSCLKSOURCE_HSE, RCC_SYSCLKSOURCE_PLLCLK
		 * AHBCLKDivider	--> RCC_SYSCLK_DIV1, RCC_SYSCLK_DIV2, RCC_SYSCLK_DIV4, ..., RCC_SYSCLK_DIV512
		 * APB1CLKDivider	--> RCC_HCLK_DIV1, RCC_HCLK_DIV2, RCC_HCLK_DIV4, RCC_HCLK_DIV8, RCC_HCLK_DIV16
		 * APB2CLKDivider	--> RCC_HCLK_DIV1, RCC_HCLK_DIV2, RCC_HCLK_DIV4, RCC_HCLK_DIV8, RCC_HCLK_DIV16
		 *
		 * RTCClockSelection	-->RCC_RTCCLKSOURCE_HSE_DIV2, RCC_RTCCLKSOURCE_HSE_DIV4, RCC_RTCCLKSOURCE_LSI,RCC_RTCCLKSOURCE_LSE
		 */

		RCC_OSCILLATORTYPE_HSI,
		RCC_HSE_OFF,
		RCC_LSE_OFF,
		RCC_HSI_ON,
		0x10,
		RCC_LSI_ON,

		RCC_PLL_ON,
		RCC_PLLSOURCE_HSI,
		8,	// PLLM
		168, // PLLN
		RCC_PLLP_DIV2, // PLLP
		4, // PLLQ
		2, // PLLR

		RCC_SYSCLKSOURCE_PLLCLK,
		RCC_SYSCLK_DIV1,
		RCC_HCLK_DIV4,
		RCC_HCLK_DIV2,

		RCC_RTCCLKSOURCE_LSI

};


const tWatchdogStruct IWDGMap =  {

	/* All calculations are based in the Reference Manual: DM00091010 rev.3 pag. 470 	*/
	/* mSecs_UntilReset	--> Range in milli-seconds [1 - 820]							*/
	/* mSecs_LowWindow*	--> Range in milli-seconds [1 - 820]							*/
	/*  	*Choose IWDG_WINDOW_DISABLE value if you want to deactivate the window	*/

		500,  //ms
		IWDG_WINDOW_DISABLE	//ms

};
const tWatchdogStruct WWDGMap =  {

	/* All calculations are based in the Reference Manual: DM00091010 rev.3 pag. 479	*/
	/* mSecs_UntilReset	--> Range in milli-seconds [1 to 44]							*/
	/* mSecs_LowWindow*	--> Range in milli-seconds [1 to 44]							*/
	/*  	*Choose W_WINDOW_MODE_DEACTIVATE value if you want to deactivate the window	*/

		44, //ms
		W_WINDOW_MODE_DEACTIVATE	//ms
};



#endif // _SYSTEM_MAP_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
