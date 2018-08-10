/*****************************************************************************
 * @file 	Buttons_map.h
 *
 * @brief 	Buttons Map Header.
 *
 * @author 	Albert Serrallo
 *
 * @date 	28/04/2016
 *
 ****************************************************************************/
#ifndef _BUTTONS_MAP_H_
#define _BUTTONS_MAP_H_

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
/* Time to check the Buttons in Scheduler */
#define BUTTONS_SCHEDULER_CALLBACK_TIMER_MS 	10

#ifdef _GPIO_H_
/* define MON_BUTTON_GPIO for use button with gpio */
#define MON_BUTTON_GPIO
#endif //_GPIO_H_

#ifdef _TIMER_H_
/* define MON_BUTTON_TIMER for use button with timer */
#define MON_BUTTON_TIMER
#endif //_TIMER_H_

#ifdef _ADC_H_
/* define MON_BUTTON_ADC for use button with adc */
#define MON_BUTTON_ADC
#endif //_ADC_H_


/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

/* Instance Buttons */
typedef enum eButtons
{
#if defined(MON_BUTTON_GPIO)
	BUTTON_INC_TIME,
	BUTTON_DEC_TIME,
	BUTTON_RESET_MODE,
	BUTTON_RELEASE_TEST,
#endif	//MON_BUTTON_GPIO

#ifdef MON_BUTTON_TIMER
	BUTTON_ENCODER_INC,
	BUTTON_ENCODER_DEC,
#endif //MON_BUTTON_TIMER

#ifdef MON_BUTTON_ADC
	BUTTON_ADC_TEST,
#endif //MON_BUTTON_ADC
    NUM_OF_BUTTONS,
} tButtons;


#if defined(MON_BUTTON_GPIO)
typedef enum ePDGpioButtons
{
	BUTTON_GPIO_TEST_1,
	BUTTON_GPIO_TEST_2,
	BUTTON_GPIO_TEST_RST,
	BUTTON_GPIO_RELEASE,

	NUM_OF_GPIO_BUTTONS,
} tPDGpioButtons;
#endif


#if defined(MON_BUTTON_TIMER)
typedef enum ePDTimerButtons
{
	BUTTON_TIMER_INC,
	BUTTON_TIMER_DEC,

	NUM_OF_TIMER_BUTTONS,
}tPDTimerButtons;
#endif


#if defined(MON_BUTTON_ADC)
typedef enum ePDAdcButtons
{
	BUTTON_ADC_1,

	NUM_OF_ADC_BUTTONS,
}tPDAdcButtons;
#endif



#endif // _GPIO_MAP_H_

/****************************************************************************
 *    End of file
 ***************************************************************************/
