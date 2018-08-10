/*****************************************************************************
 * @file
 * @addtogroup Drivers
 * @{
 * @addtogroup Buttons
 * @{

 *
 * @brief 	Buttons functionality Header.
 *
 * @author 	Albert Serrallo
 *
 * @date 	28/04/2016
 *
 ****************************************************************************/
#ifndef _BUTTONS_H_
#define _BUTTONS_H_

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
#include "Buttons_map.h"

/****************************************************************************
 * DEFINES
 ****************************************************************************/
#define NULL_BUTTON	0xFF

#define NUM_MAX_BUTTONS 3

#define MAX_TIME_OVERFLOW 100

/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

typedef enum eButtonsAction
{
	BUTTON_NO_EVENT,
	BUTTON_EVENT_AVAILABLE
}tButtonAction;


typedef enum eButtonsState
{
	BUTTON_STATE_NOT_PRESSED,
	BUTTON_STATE_PRESSED
}tButtonState;


typedef enum eButtonsTimerState
{
	TIMER_INCREMENT,
	TIMER_DECREMENT
}tButtonsTimerValue;


typedef enum eButtonsStateMachine
{
	BUTTON_FSM_WAITING_PRESS,
	BUTTON_FSM_PRESS,
	BUTTON_FSM_USED
} tButtonFSM;


typedef enum eButtonType
{
	BUTTON_TYPE_ADC,
	BUTTON_TYPE_GPIO,
	BUTTON_TYPE_TIMER,

	NUM_OF_BUTTON_TYPE
}tButtonType;


typedef struct sButtonsPDGpioMap
{
	uint16_t		periphGPIO[NUM_MAX_BUTTONS];	/**< @brief Peripheral GPIO  */
	const HREG* 	hregPtrValue;					/**< @brief hreg associated to device instance */
	uint32_t		minPushTime;					/**< @brief minimum Time to consider correct push */
	uint32_t 		maxPushTime;					/**< @brief maximum Time to consider correct push */
	uint16_t		pushedValue;					/**< @brief Value of the Push GPIO Value */
}tButtonsPDGpioMap;

typedef struct sButtonsPDTimerMap
{
	uint16_t			periphTimer;			/**< @brief Peripheral Timer  */
	const HREG* 		hregPtrValue;			/**< @brief hreg associated to device instance */
	tButtonsTimerValue	timerValue;				/**< @brief Value for event available */
	uint8_t				filterEnc;					/**< @brief Num of filter for the encoder  */
}tButtonsPDTimerMap;

typedef struct sButtonsPDAdcMap
{
	uint16_t			periphADC;				/**< @brief Peripheral ADC  */
	uint16_t			groupADC;
	const HREG* 		hregPtrValue;			/**< @brief hreg associated to device instance */
	const HREG* 		hregPtrGroup;			/**< @brief hreg associated to device instance */
	uint8_t				ADCValue;				/**< @brief Value in % */
}tButtonsPDAdcMap;

typedef struct sButtonsInstanceMap
{
	uint16_t 		buttonType;
	uint16_t		indexType;
	uint32_t		expireTime;
	void 			(*processCallBackFunction)(uint8_t nButton);
} tButtonsInstanceMap;



typedef struct sButtonsContext
{
	uint16_t			actualExpireTime;	/**< @brief Time passed after button release*/
	uint8_t				countPress;			/**< @brief Number of times button was pressed */
}tButtonsContext;

typedef struct SButtonsGPIOContext
{
	tButtonState	buttonState;
	tButtonFSM		actualFSM;			/**< @brief Actual Button State Machine */
	uint16_t		actualTime;			/**< @brief Time passed after button pressed */
	uint16_t		numButtons;
	uint8_t			nonReleaseAction;		/**< @brief Button non-release action */
}tButtonsGPIOContext;

typedef struct SButtonsTimerContext
{
	uint32_t		timerResolution;
	int32_t			lastTimerValue;
}tButtonsTimerContext;

typedef struct SButtonsADCContext
{
	uint32_t		adcResolution;
	uint32_t		lastAdcValue;
}tButtonsADCContext;

/****************************************************************************
 *    PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************/
eError ButtonsInit( void );
eError ButtonsStop( void );
eError ButtonsStart( void );
eError ButtonsSleep( void );
eError ButtonsWake( void );

tButtonAction ButtonsGetState(tButtons button, uint8_t *countPress);
void ButtonsTask( void );

#endif // _BUTTON_H_

/****************************************************************************
 *    End of file
 ***************************************************************************/
