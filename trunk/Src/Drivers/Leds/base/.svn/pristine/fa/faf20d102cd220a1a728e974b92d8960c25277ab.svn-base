/*************************************************************************//**
 * @file Leds.h
 *
 * @brief LEDS
 *
 * @author Albert Serrallo
 *
 * @date 12/05/2016
 *
 ****************************************************************************/

 #ifndef _LEDS_H_
 #define _LEDS_H_

 /****************************************************************************
  *  COMMON INCLUDE
  ***************************************************************************/
 #include "common.h"

 /****************************************************************************
  *  INCLUDES
  ***************************************************************************/
 #include "leds_map.h"

 /****************************************************************************
  *  DEFINES
  ****************************************************************************/
#define INFINITE_BLINK	0

 /****************************************************************************
  *  TYPES DEFINITION
  ****************************************************************************/

typedef enum eLedMode
{
	LED_ON_OFF,
	LED_BLINKING,
	LED_ON_TIME,
	LED_FADING,
}tLedMode;


typedef enum eLedOnValue
{
	LED_DIRECT,
	LED_INVERSE,
}tLedOnValue;

typedef enum eLedType
{
	LED_GPIO,
	LED_PWM,
}tLedType;

typedef enum eLedState
{
	LED_STATE_NOT_INITIALIZED,
	LED_STATE_ON,
	LED_STATE_OFF,

	LED_NUM_OF_STATES
}tLedState;


typedef struct sLedsInstanceMap
{
	tLedType		typeLed;		/* Led Type : LED_GPIO or LED_PWM 					*/
	const HREG*		hregValue;		/* Hreg of typeLed to read or write value			*/
	uint16_t		peripheral;		/* GPIO or TIMER instance depends of Led Type		*/
	uint8_t			pwmOnValue;		/* PWM Duty cycle Value [0-100] 					*/
	tLedOnValue		ledOnValue;		/* On value: LED_DIRECT or LED_INVERSE 				*/
	tLedsEffect		ledDefault;		/* Default Effect for LED			 				*/
} tLedsInstanceMap;


typedef struct sLedsEffectMap
{
	tLedMode		ledMode;	/* Init Mode on Led									*/
	uint16_t		indexEffect;	/* Index to link the respective table led type		*/
} tLedsEffectMap;


typedef struct sLedsBlinkingMap
{
	uint16_t		numBlinks;		/* Number of blinks */
	uint32_t		timeOn;			/* Time Led On in mS */
	uint32_t		timeOff;		/* Time Led Off in mS */
    tLedState       finalLedState;  /* Final Led STATE    */
} tLedsBlinkingMap;

typedef struct sLedsOnTimeMap
{
	uint32_t		ledOnTime;  	/* Led Time On before change state to Led off */
} tLedsOnTimeMap;


typedef struct sLedsFadingMap
{
	uint8_t			step;		/* Step of fade mode (in %) 			  		*/
	uint16_t		timeStep;	/* time to next Step (in mS)			  		*/
	uint8_t			maxValue;	/* maximum value of fade mode ( default 100%) 	*/
	uint8_t			minValue;	/* minimum value of fade mode ( default 100%) 	*/
} tLedsFadingMap;


typedef struct sLedsContext
{
	tLedState		ledOutputState;
	tLedsEffect		actualLedEffect;
	uint16_t		actualIndexType;
	tLedMode		actualLedMode;
	uint32_t		actualTime;
	tLedState		actualState;
	uint8_t			actualStep;		/* Actual step for fading mode */
	uint8_t			actualPwm;
	uint16_t		actualNumBlinks;
} tLedsContext;


 /****************************************************************************
  *    VARIABLES
  ****************************************************************************/

  /****************************************************************************
  *    PRIVATE FUNCTIONS
  ****************************************************************************/

 /****************************************************************************
  *    PUBLIC FUNCTIONS
  ****************************************************************************/
 eError LedsInit( void );
 eError LedsStop( void );
 eError LedsStart( void );
 eError LedsSleep( void );
 eError LedsWake( void );

 eError LedsSetModeEffect( tLeds led, tLedsEffect ledEffect );
 eError LedsGetModeEffect( tLeds led, tLedsEffect *ledEffect );
 eError LedsSetModeState( tLeds led, tLedState ledState );
 eError LedsGetModeState( tLeds led, tLedState *ledState);

 eError LedsOn( tLeds led );
 eError LedsOff( tLeds led );

 void LedsTask( void );



 #endif // _LEDS_H_

 /****************************************************************************
  *    End of file
  ***************************************************************************/
