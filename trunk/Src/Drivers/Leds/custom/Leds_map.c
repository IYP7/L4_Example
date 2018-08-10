/***********************************************************************
 * @file    Leds_map.c
 *
 * @brief   Leds map control
 *
 * @author 	Albert Serrallo
 *
 * @date 	12/05/2016
 **********************************************************************/

#ifndef	_LEDS_MAP_C_
#define	_LEDS_MAP_C_

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "common.h"

/***********************************************************************
 * COMMON ARCHITECTURE INCLUDES
 **********************************************************************/
#include "hreg_dat.h"
#include "Leds.h"

/***********************************************************************
 * INCLUDES
 **********************************************************************/

/***********************************************************************
 * DEFINES
 **********************************************************************/

/***********************************************************************
 * TYPE DEFINITIONS
 **********************************************************************/


/*	LED INSTANCE MAP 																				*/
/*																									*/
/* 		typeLed			--> LED_GPIO or LED_PWM (tLedType)											*/
/* 		hregValue		--> Hreg of typeLed to read or write value									*/
/* 		peripheral		--> GPIO or TIMER instance depends of Led Type								*/
/*		pwmOnValue;		-->	PWM Duty cycle Value [0-100] , if LED_GPIO this variable is unused		*/
/* 		onStateValue	--> LED_DIRECT or LED_INVERSE												*/
/*		ledDefault		--> Default effect mode for led												*/

tLedsInstanceMap ledsInstanceMap[NUMBER_OF_LEDS] =
{
	/* LED_TEST_1 */	{ LED_GPIO,	HREG_GPIO_VALUE,	GPIO_LED_1, 			0,		LED_DIRECT,		LED_EFFECT_ON_OFF		},
	/* LED_TEST_2 */	{ LED_GPIO,	HREG_GPIO_VALUE,	GPIO_LED_2, 			0,		LED_DIRECT,		LED_EFFECT_BLINKING_1	},
#ifdef _TIMER_H
	/* LED_TEST_3 */	{ LED_PWM,	HREG_TIMER_VALUE,	TIM_PWM_OUTPUT_TEST, 	50,		LED_DIRECT,		LED_EFFECT_FADING_1		},
	/* LED_TEST_4 */	{ LED_PWM,	HREG_TIMER_VALUE,	TIM_PWM_OUTPUT_TEST, 	50,	 	LED_DIRECT,		LED_EFFECT_FADING_2		},
#endif // _TIMER_H
};


/*	LED	EFFECTS MAP 																				*/
/*																									*/
/* 		ledMode			--> LED_TYPE_ON_OFF, LED_TYPE_BLINKING or LED_TYPE_ON_TIME (tLedMode)		*/
/* 		indexEffect		--> Index to link the respective table led action effect map, 				*/
tLedsEffectMap ledsEffectMap[NUM_OF_LED_EFFECT] =
{
	/* LED_EFFECT_ON_OFF    */		{ LED_ON_OFF, 0		},

	/* LED_EFFECT_ONTIMER_1 */		{ LED_ON_TIME, LED_ONTIMER_1 },
	/* LED_EFFECT_ONTIMER_2 */		{ LED_ON_TIME, LED_ONTIMER_2 },

	/* LED_EFFECT_BLINKING_1 */		{ LED_BLINKING, LED_BLINK_1 },
	/* LED_EFFECT_BLINKING_2 */		{ LED_BLINKING, LED_BLINK_2 },

	/* LED_EFFECT_FADING_1 */		{ LED_FADING, LED_FADE_1 },
	/* LED_EFFECT_FADING_2 */		{ LED_FADING, LED_FADE_2 },
};


/*	LED BLINKING MAP 																	*/
/*																						*/
/* 		numBlinks		--> Number of blinks 											*/
/* 		timeOn			--> Blinking time ON											*/
/* 		timeOff 		--> Blinking time OFF											*/
tLedsBlinkingMap ledsBlinkingMap[NUM_OF_BLINK_LEDS] =
{
	/* LED_BLINK_1 */ { INFINITE_BLINK, 	300, 	1000,  LED_STATE_OFF },
	/* LED_BLINK_2 */ { 3,					300, 	 200,  LED_STATE_OFF },
};


/*	LED FADING MAP 																		*/
/*																						*/
/* 		step			--> Step of fade mode (in %) 									*/
/* 		timeStep		--> Time to next step of fade mode (in %) 						*/
/* 		maxValue		--> maximum value of fade mode ( default 100%) 					*/
/* 		minValude 		--> minimum value of fade mode ( default 0%) 					*/
tLedsFadingMap ledsFadingMap [NUM_OF_FADES_LEDS] =
{
	/* LED_FADE_1 */ { 5, 	100, 100, 	0 },
	/* LED_FADE_2 */ { 5,   500,  80,   20 },
};


/*	LED ON TIME MAP 																	*/
/*																						*/
/* 		ledOnTime		--> Led Time On before change state to Led off 					*/
tLedsOnTimeMap ledsOnTimeMap[NUM_OF_ONTIME_LEDS] =
{
	/* LED_ONTIMER_1 */ { 2000 },
	/* LED_ONTIMER_2 */ { 500 },
};





/***********************************************************************
 * PUBLIC FUCNTIONS
 **********************************************************************/

/***********************************************************************
 * PRIVATE FUCNTIONS
 **********************************************************************/

#endif	/*_LEDS_MAP_C_*/

/***********************************************************************
 * End of file
 **********************************************************************/
