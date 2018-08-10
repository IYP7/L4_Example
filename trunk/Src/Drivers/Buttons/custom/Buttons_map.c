/*****************************************************************************
 * @file 	Buttons_map.c
 *
 * @brief 	Buttons map control.
 *
 * @author 	Albert Serrallo
 *
 * @date 	28/04/2016
 *
 ****************************************************************************/
#ifndef	_BUTTONS_MAP_C_
#define	_BUTTONS_MAP_C_

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
#include "Buttons.h"

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/


/*	BUTTON INSTANCE MAP 																					*/
/*																											*/
/* 		buttonType				--> BUTTON_TYPE_ADC, BUTTON_TYPE_TIMER or BUTTON_TYPE_GPIO (tButtonType)	*/
/* 		indexType		 		--> Index to link the respective table button type							*/
/* 		expireTime				--> Max Time to reset the flag of Button , expireTime=0 -> No expire Time	*/
/* 		processCallBackFunction --> CallBackFunction														*/

const tButtonsInstanceMap buttonsInstanceMap [NUM_OF_BUTTONS] =
{
#if defined(MON_BUTTON_GPIO)
	/* BUTTON_INC_TIME */		{ BUTTON_TYPE_GPIO,		BUTTON_GPIO_TEST_1,	 	5000, 	NULL	},
	/* BUTTON_DEC_TIME */		{ BUTTON_TYPE_GPIO,		BUTTON_GPIO_TEST_2, 	5000, 	NULL 	},
	/* BUTTON_RESET_MODE*/  	{ BUTTON_TYPE_GPIO,		BUTTON_GPIO_TEST_RST, 	10000, 	NULL 	},
	/* BUTTON_RELEASE_TEST */	{ BUTTON_TYPE_GPIO,		BUTTON_GPIO_RELEASE,	0,		NULL 	},
#endif	//MON_BUTTON_GPIO

#if defined(MON_BUTTON_TIMER)
	/* BUTTON_ENCODER_INC */	{ BUTTON_TYPE_TIMER,	BUTTON_TIMER_INC,	 	5000,	NULL 	},
	/* BUTTON_ENCODER_DEC */	{ BUTTON_TYPE_TIMER,	BUTTON_TIMER_DEC,	 	5000,	NULL 	},
#endif //MON_BUTTON_TIMER

#if defined(MON_BUTTON_ADC)
	/* BUTTON_ADC_TEST */		{ BUTTON_TYPE_ADC,		BUTTON_ADC_1,	 		5000,	NULL 	},
#endif //MON_BUTTON_ADC
};


/*	BUTTON TYPE PERIPHERAL GPIO 																									*/
/*																																	*/
/* 		periphGPIO[NUM_MAX_BUTTONS] --> GPIO_BUTTON_1, GPIO_BUTTON_2 defined and configured (eGPIOPort -> GPIO_map.h )				*/
/* 								* NULL_BUTTON = No more GPIO associated with button 												*/
/* 		hregPtrValue		--> HREG_GPIO_VALUE	, value of gpio																		*/
/* 		minPushTime			--> Min Time in milliseconds to consider it correct Push												*/
/* 		maxPushTime			--> If maxPushTime = 0 then , Button is configured as NonRelease mode									*/
/* 		pushedValue			--> State of pushed value GPIO, GPIO_LOW or GPIO_HIGH													*/
#if defined(MON_BUTTON_GPIO)
const tButtonsPDGpioMap buttonsPDGpioMap [NUM_OF_GPIO_BUTTONS] =
{
		/* BUTTON_GPIO_TEST_1 */	{ {GPIO_BUTTON_B1,	NULL_BUTTON,	NULL_BUTTON},	HREG_GPIO_VALUE,	100,	1000,	GPIO_LOW },
		/* BUTTON_GPIO_TEST_2 */	{ {GPIO_BUTTON_B2,	NULL_BUTTON,	NULL_BUTTON},	HREG_GPIO_VALUE, 	100,	1000,	GPIO_LOW },
		/* BUTTON_GPIO_TEST_RST */	{ {GPIO_BUTTON_B1, 	GPIO_BUTTON_B2,	NULL_BUTTON}, 	HREG_GPIO_VALUE,    1000,	3000,	GPIO_LOW },
		/* BUTTON_GPIO_RELEASE */	{ {GPIO_BUTTON_B3,	NULL_BUTTON,	NULL_BUTTON},	HREG_GPIO_VALUE,	1000,	0,		GPIO_LOW },
};
#endif //MON_BUTTON_GPIO


/*	BUTTON TYPE PERIPHERAL TIMER 															*/
/* 		hregPtrValue		--> HREG_TIM_DUTY, Value of the Timer							*/
/* 		timerValue			--> TIMER_INCREMENT or TIMER_DECREMENT							*/
/* 								Increment = positive timer value ( 300 -> 1500 )			*/
/* 								Decrement = negative timer value ( 200 -> 5 )				*/
/* 		filterEnc			--> To increment button number need X timer steps				*/
#ifdef _TIMER_H_
#if defined(MON_BUTTON_TIMER)
const tButtonsPDTimerMap buttonsPDTimerMap [NUM_OF_TIMER_BUTTONS] =
{
		/* BUTTON_TIMER_INC */	{TIM_ENCODER_TEST,	HREG_TIMER_VALUE,	TIMER_INCREMENT,    1 },
		/* BUTTON_TIMER_DEC */	{TIM_ENCODER_TEST,	HREG_TIMER_VALUE,	TIMER_DECREMENT,    1 },
};
#endif //MON_BUTTON_TIMER
#endif //_TIMER_H_

/*	BUTTON TYPE PERIPHERAL ADC																*/
/*																							*/
/* periphADC 			--> ADC_1 ( eADCCommon -> ADC_map.h )								*/
/* groupADC 			--> ADC_GROUP_1 ( eADCGroup -> ADC_map.h )							*/
/* hregPtrValue			--> HREG_ADC_VALUE, Value of the ADC								*/
/* hregPtrGroup			--> HREG_ADC_GROUP, Group of the ADC								*/
/* ADCValue				--> Value in % to consider event available							*/
#ifdef _ADC_H_
#if defined(MON_BUTTON_ADC)
const tButtonsPDAdcMap buttonsPDAdcMap [NUM_OF_ADC_BUTTONS] =
{
		/* BUTTON_ADC_1 */	{	ADC_5V, 	ADC_GROUP_1, HREG_ADC_VALUE, HREG_ADC_GROUP,	50	},
};
#endif //MON_BUTTON_ADC
#endif //_ADC_H_


#endif // _BUTTONS_MAP_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
