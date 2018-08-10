/***********************************************************************
 * @file	Leds_map.h
 *
 * @brief	Leds map declaration
 *
 * @author Albert Serrallo
 *
 * @date 12/05/2016
 **********************************************************************/

#ifndef	_LEDS_MAP_H_
#define	_LEDS_MAP_H_

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "common.h"

/***********************************************************************
 * COMMON ARCHITECTURE INCLUDES
 **********************************************************************/

/***********************************************************************
 * INCLUDES
 **********************************************************************/

/***********************************************************************
 * DEFINES
 **********************************************************************/
#define LEDS_DRIVER_SCHEDULER_CALLBACK_TIMER_MS	 (5)



/***********************************************************************
 * TYPE DEFINITIONS
 **********************************************************************/

typedef enum eLeds
{
	LED_TEST_1,
	LED_TEST_2,
#ifdef _TIMER_H
	LED_TEST_3,
	LED_TEST_4,
#endif // _TIMER_H

    NUMBER_OF_LEDS,
} tLeds;

typedef enum eLedsEffect
{
	LED_EFFECT_ON_OFF,

	LED_EFFECT_ONTIMER_1,
	LED_EFFECT_ONTIMER_2,

	LED_EFFECT_BLINKING_1,
	LED_EFFECT_BLINKING_2,

	LED_EFFECT_FADING_1,
	LED_EFFECT_FADING_2,

    NUM_OF_LED_EFFECT,
} tLedsEffect;



typedef enum eBlinkLeds
{
	LED_BLINK_1,
	LED_BLINK_2,

	NUM_OF_BLINK_LEDS,
}tBlinkLeds;

typedef enum eOnTimeLeds
{
	LED_ONTIMER_1,
	LED_ONTIMER_2,

	NUM_OF_ONTIME_LEDS,
}tOnTimeLeds;

typedef enum eFadingLeds
{
	LED_FADE_1,
	LED_FADE_2,

	NUM_OF_FADES_LEDS,
}tFadingLeds;



/***********************************************************************
 * PUBLIC FUCNTION PROTOTYPES
 **********************************************************************/

#endif	/*_LEDS_MAP_H_*/

/***********************************************************************
 * End of file
 **********************************************************************/
