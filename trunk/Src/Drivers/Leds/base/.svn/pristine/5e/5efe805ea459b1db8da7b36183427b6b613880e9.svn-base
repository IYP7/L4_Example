/*************************************************************************//**
 * @file  Leds.c
 *
 * @brief Leds functions
 *
 * @author Albert Serrallo
 *
 * @date 12/05/2016
 *
 ****************************************************************************/

#ifndef _LEDS_C_
#define _LEDS_C_

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
#include "Leds.h"

/****************************************************************************
 *  DEFINES
 ****************************************************************************/


/****************************************************************************
 *  EXTERN VARIABLES
 ****************************************************************************/
extern const tLedsInstanceMap ledsInstanceMap [NUMBER_OF_LEDS];
extern const tLedsBlinkingMap ledsBlinkingMap [NUM_OF_BLINK_LEDS];
extern const tLedsOnTimeMap ledsOnTimeMap [NUM_OF_ONTIME_LEDS];
extern const tLedsFadingMap ledsFadingMap [NUM_OF_FADES_LEDS];
extern const tLedsEffectMap ledsEffectMap[NUM_OF_LED_EFFECT];

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
tLedsContext ledsContext[NUMBER_OF_LEDS];


/****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/
eError LedsUpdateBlinking( tLeds led );
eError LedsUpdateFading( tLeds led, tFadingLeds fading);
eError LedsUpdateState(tLeds led);
eError LedsReset( tLeds led );

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/


/*************************************************************************//**
 * @brief  Initializes leds.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError LedsInit( void )
{
	eError result = RET_OK;
	tLeds led;

	for (led = 0; led < NUMBER_OF_LEDS; led++)
	{
		result = LedsReset(led);
		ledsContext[led].ledOutputState = LED_STATE_OFF;
		ledsContext[led].actualLedEffect = ledsInstanceMap[led].ledDefault;
		ledsContext[led].actualIndexType = ledsEffectMap[ledsInstanceMap[led].ledDefault].indexEffect;
		ledsContext[led].actualLedMode = ledsEffectMap[ledsInstanceMap[led].ledDefault].ledMode;

	}

	return result;
}



/*************************************************************************//**
 * @brief   Reset parameters of led.
 * @param	led.
 * @return  RET_OK.
 ****************************************************************************/
eError LedsReset( tLeds led )
{
	eError result = RET_OK;

	ledsContext[led].actualState = LED_STATE_OFF;
	ledsContext[led].actualTime = 0;
	ledsContext[led].actualPwm = ledsInstanceMap[led].pwmOnValue;
	ledsContext[led].actualNumBlinks = 0;
	ledsContext[led].actualStep = 0;

	return result;
}



/*************************************************************************//**
 * @brief  Starts leds.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError LedsStart( void )
{
	eError result = RET_OK;
	tLeds led;

	/* Init all leds Off */
	for (led = 0; led < NUMBER_OF_LEDS; led++)
	{
		LedsOff(led);
	}

	return result;
}

/*************************************************************************//**
 * @brief  Stops leds.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError  LedsStop( void )
{
	eError result = RET_OK;

	return result;
}

/*************************************************************************//**
 * @brief  Sleeps leds.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError  LedsSleep( void )
{
	eError result = RET_OK;

	result = LedsInit();

	return result;
}

/*************************************************************************//**
 * @brief  Wakes leds
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError  LedsWake( void )
{
	eError result = RET_OK;

	LedsStart();

	return result;
}


/*************************************************************************//**
 * @brief	Get Led effect
 * @param	led to set action
 * @param	ledEffect defined in ledsEffectMap
 * @return	RET_OK.
 ****************************************************************************/
eError LedsGetModeEffect( tLeds led, tLedsEffect *ledEffect )
{
	eError result = RET_OK;

	*ledEffect = ledsContext[led].actualLedEffect;

	return result;
}

/*************************************************************************//**
 * @brief	Set Led effect
 * @param	led to set action
 * @param	ledEffect defined in ledsEffectMap
 * @return	RET_OK.
 ****************************************************************************/
eError LedsSetModeEffect( tLeds led, tLedsEffect ledEffect )
{
	eError result = RET_OK;

	ledsContext[led].actualLedMode = ledsEffectMap[ledEffect].ledMode;
	ledsContext[led].actualIndexType = ledsEffectMap[ledEffect].indexEffect;
	ledsContext[led].actualLedEffect = ledEffect;
	ledsContext[led].actualState = LED_STATE_OFF;
	ledsContext[led].ledOutputState = LED_STATE_OFF;

	return result;
}


/*************************************************************************//**
 * @brief	Set Led State
 * @param	led to set action
 * @param	ledAction : LED_ON, LED_OFF
 * @return	RET_OK.
 ****************************************************************************/
eError LedsGetModeState( tLeds led, tLedState *ledState)
{
	eError result = RET_OK;

	*ledState = ledsContext[led].ledOutputState;

	return result;
}

/*************************************************************************//**
 * @brief	Sets Action led.
 * @param	led to set action
 * @param	ledAction : LED_ON, LED_OFF
 * @return	RET_OK.
 ****************************************************************************/
eError LedsSetModeState( tLeds led, tLedState ledState )
{
	eError result = RET_OK;

	ledsContext[led].ledOutputState = ledState;
	/* LED_ON reset the parameters of LED (numOfBlinks, pwm for fading, etc. */
	if ( ledState == LED_STATE_ON )
	{
		if (ledsContext[led].actualLedMode == LED_ON_OFF)
		{
			LedsOn(led);
		}
		else if ( ledsContext[led].actualLedMode == LED_ON_TIME)
		{
			LedsReset(led);
			LedsOn(led);
		}
		else
		{
			LedsReset(led);
			LedsOff(led);
		}
	}
	else
	{
		LedsOff(led);
	}

	return result;
}



/*************************************************************************//**
 * @brief  Turn on the led
 * @param	led
 * @return  None.
 ****************************************************************************/
eError LedsOn( tLeds led )
{
	eError result = RET_OK;

	ledsContext[led].actualState = LED_STATE_ON;

	/* Turn on led */
	if ( ledsInstanceMap[led].typeLed == LED_GPIO ) /* GPIO LED */
	{
		if (ledsInstanceMap[led].ledOnValue == LED_DIRECT)
		{
			WRITE_HREG( ledsInstanceMap[led].peripheral, ledsInstanceMap[led].hregValue, GPIO_HIGH);
		}
		else
		{
			WRITE_HREG( ledsInstanceMap[led].peripheral, ledsInstanceMap[led].hregValue, GPIO_LOW);
		}
	}
	else if ( ledsInstanceMap[led].typeLed == LED_PWM ) /* PWM LED */
	{
		if (ledsInstanceMap[led].ledOnValue == LED_DIRECT)
		{
			WRITE_HREG( ledsInstanceMap[led].peripheral, ledsInstanceMap[led].hregValue, ledsContext[led].actualPwm);
		}
		else
		{
			WRITE_HREG( ledsInstanceMap[led].peripheral, ledsInstanceMap[led].hregValue, 100 - ledsContext[led].actualPwm);
		}
	}

	return result;
}

/*************************************************************************//**
 * @brief  Turn off the led
 * @param	led
 * @return  None.
 ****************************************************************************/
eError  LedsOff( tLeds led )
{
	eError result = RET_OK;

	ledsContext[led].actualState = LED_STATE_OFF;

	/* Turn off led */
	if ( ledsInstanceMap[led].typeLed == LED_GPIO ) /* GPIO LED */
	{
		if (ledsInstanceMap[led].ledOnValue == LED_DIRECT)
		{
			WRITE_HREG( ledsInstanceMap[led].peripheral, ledsInstanceMap[led].hregValue, GPIO_LOW);
		}
		else
		{
			WRITE_HREG( ledsInstanceMap[led].peripheral, ledsInstanceMap[led].hregValue, GPIO_HIGH);
		}
	}

	else if ( ledsInstanceMap[led].typeLed == LED_PWM ) /* PWM LED */
	{
		if (ledsInstanceMap[led].ledOnValue == LED_DIRECT)
		{
			WRITE_HREG( ledsInstanceMap[led].peripheral, ledsInstanceMap[led].hregValue, 0);
		}
		else
		{
			WRITE_HREG( ledsInstanceMap[led].peripheral, ledsInstanceMap[led].hregValue, 100);
		}
	}


	return result;
}

/*************************************************************************//**
 * @brief	Update Blinking action
 * @param	led
 * @return  RET_OK or RET_FAIL
 ****************************************************************************/
eError LedsUpdateBlinking( tLeds led )
{
	eError result = RET_OK;
	if (  ledsBlinkingMap[ledsContext[led].actualIndexType].numBlinks == INFINITE_BLINK || ledsContext[led].actualNumBlinks < ledsBlinkingMap[ledsContext[led].actualIndexType].numBlinks )
	{
		if ( ledsContext[led].actualState == LED_STATE_ON )
		{
			result = LedsOff(led);

			// If specified number of blinks, decrement actualNumBlinks
			if ( ledsBlinkingMap[ledsContext[led].actualIndexType].numBlinks != INFINITE_BLINK )
			{
				ledsContext[led].actualNumBlinks++;
			}
		}
		else if ( ledsContext[led].actualState == LED_STATE_OFF )
		{
			result = LedsOn(led);
		}

	}
    else
    {
        if ( ledsBlinkingMap[ledsContext[led].actualIndexType].finalLedState == LED_STATE_ON &&
             ledsContext[led].actualState == LED_STATE_OFF)
        {
			LedsOn(led);
		}
        else if (ledsBlinkingMap[ledsContext[led].actualIndexType].finalLedState == LED_STATE_OFF &&
             ledsContext[led].actualState == LED_STATE_ON)
        {
			LedsOff(led);
		}
    }
    
	return result;
}


/*************************************************************************//**
 * @brief	Update Fading action
 * @param	led, fadingType
 * @return  RET_OK or RET_FAIL
 ****************************************************************************/
eError LedsUpdateFading( tLeds led, tFadingLeds fading)
{
	eError result = RET_OK;
	if ( ledsContext[led].actualStep >= ledsFadingMap[fading].maxValue)
	{
		if ( ledsContext[led].actualState == LED_STATE_ON )
		{
			ledsContext[led].actualState = LED_STATE_OFF;
		}
		else
		{
			ledsContext[led].actualState = LED_STATE_ON;
		}

		ledsContext[led].actualStep = ledsFadingMap[fading].minValue;
	}
	else
	{
		ledsContext[led].actualStep +=  ledsFadingMap[fading].step;
	}

	ledsContext[led].actualPwm = ledsContext[led].actualStep;

	// Update PWM Timer
	if ( ledsContext[led].actualState == LED_STATE_ON )
	{
		WRITE_HREG( ledsInstanceMap[led].peripheral, ledsInstanceMap[led].hregValue, 100-ledsContext[led].actualPwm);
	}
	else
	{
		WRITE_HREG( ledsInstanceMap[led].peripheral, ledsInstanceMap[led].hregValue, ledsContext[led].actualPwm);
	}


	return result;
}



/*************************************************************************//**
 * @brief  Led Scheduler periodical task execution
 * @param	None.
 * @return  None.
 ****************************************************************************/
void LedsTask( void )
{
	uint16_t nLed = 0;

	for ( nLed=0; nLed < NUMBER_OF_LEDS; nLed++)
	{
		if ( ledsContext[nLed].ledOutputState == LED_STATE_ON)
		{
			LedsUpdateState(nLed);
		}
	}
}


/*************************************************************************//**
 * @brief  Led Callback Function
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError LedsUpdateState(tLeds ledNumber)
{
	eError result = RET_OK;

	/* Gets actual time in milliseconds */
	uint32_t timeAux = ledsContext[ledNumber].actualTime*LEDS_DRIVER_SCHEDULER_CALLBACK_TIMER_MS;

	/* LED ACTION ON_OFF CALLBACK*/
	if ( ledsContext[ledNumber].actualLedMode == LED_ON_OFF)
	{
		if ( ledsContext[ledNumber].actualState == LED_STATE_ON )
		{
			LedsOn(ledNumber);
		}
		else
		{
			LedsOff(ledNumber);
		}
	}

	/* LED STATE ON TIME CALLBACK */
	else if ( ledsContext[ledNumber].actualLedMode == LED_ON_TIME)
	{
		ledsContext[ledNumber].actualTime++;
		if ( timeAux >= ledsOnTimeMap[ledsContext[ledNumber].actualIndexType].ledOnTime )
		{
			ledsContext[ledNumber].actualState = LED_STATE_OFF;
			LedsOff(ledNumber);
			ledsContext[ledNumber].actualTime  = 0;	/* Restart actual time */
		}
	}


	/* LED STATE BLINKING CALLBACK */
	else if ( ledsContext[ledNumber].actualLedMode == LED_BLINKING )
	{
		ledsContext[ledNumber].actualTime++;

		if ( ledsContext[ledNumber].actualState ==  LED_STATE_ON)
		{
			if ( timeAux >= ledsBlinkingMap[ledsContext[ledNumber].actualIndexType].timeOn &&
					ledsBlinkingMap[ledsContext[ledNumber].actualIndexType].timeOn != 0 )
			{
				LedsUpdateBlinking(ledNumber);
				ledsContext[ledNumber].actualTime = 0;
			}

		}
		else if ( ledsContext[ledNumber].actualState ==  LED_STATE_OFF)
		{
			if ( timeAux >= ledsBlinkingMap[ledsContext[ledNumber].actualIndexType].timeOff &&
					ledsBlinkingMap[ledsContext[ledNumber].actualIndexType].timeOff != 0)
			{
				LedsUpdateBlinking(ledNumber);
				ledsContext[ledNumber].actualTime = 0;
			}
		}
	}

	/* LED STATE FADING CALLBACK */
	else if ( ledsContext[ledNumber].actualLedMode == LED_FADING)
	{
		ledsContext[ledNumber].actualTime++;
		if ( timeAux >= ledsFadingMap[ledsContext[ledNumber].actualIndexType].timeStep)
		{
			LedsUpdateFading(ledNumber, (tFadingLeds)ledsContext[ledNumber].actualIndexType);
			ledsContext[ledNumber].actualTime = 0;
		}
	}

	return result;
}



#endif // _LEDS_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
