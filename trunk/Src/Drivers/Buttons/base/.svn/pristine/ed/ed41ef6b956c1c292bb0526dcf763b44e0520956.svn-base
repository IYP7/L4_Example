/*****************************************************************************
 * @file
 * @addtogroup Drivers
 * @{
 * @addtogroup Buttons
 * @{
 *
 * @brief 	Buttons driver.
 *
 * @author 	Albert Serrallo
 *
 * @date 	28/04/2016
 *
 ****************************************************************************/

#ifndef _BUTTONS_C_
#define _BUTTONS_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"
#include "hreg_dat.h"


/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "Buttons.h"
#include "Buttons_map.h"

/****************************************************************************
 *  DEFINES
 ***************************************************************************/
#define BUTTONS_TO_CHECK	2

/****************************************************************************
 *  EXTERNAL VARIABLES
 ****************************************************************************/
extern const tButtonsInstanceMap buttonsInstanceMap[NUM_OF_BUTTONS];
#if defined(MON_BUTTON_GPIO)
extern const tButtonsPDGpioMap buttonsPDGpioMap[NUM_OF_GPIO_BUTTONS];
#endif
#if defined(MON_BUTTON_TIMER)
extern const tButtonsPDTimerMap buttonsPDTimerMap[NUM_OF_TIMER_BUTTONS];
#endif
#if defined(MON_BUTTON_ADC)
extern const tButtonsPDAdcMap buttonsPDAdcMap[NUM_OF_ADC_BUTTONS];
#endif

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
tButtonsContext buttonsContext[NUM_OF_BUTTONS];
#if defined(MON_BUTTON_GPIO)
tButtonsGPIOContext buttonsGPIOContext[NUM_OF_GPIO_BUTTONS];
#endif
#if defined(MON_BUTTON_TIMER)
tButtonsTimerContext buttonsTimerContext[NUM_OF_TIMER_BUTTONS];
#endif
#if defined(MON_BUTTON_ADC)
tButtonsADCContext buttonsADCContext[NUM_OF_ADC_BUTTONS];
#endif

/****************************************************************************
 *  PRIVATE FUNCTIONS DEFINITIONS
 ****************************************************************************/
eError ButtonsCheckPeripheralValue( tButtons buttonNumber );
eError ButtonsCheckExpiredTime( tButtons buttonNumber );

#if defined(MON_BUTTON_GPIO)
eError ButtonsCheckGPIOValue( tPDGpioButtons gpioButton );
eError ButtonsChangeStateGPIOButtons( tPDGpioButtons gpioButton, tButtonFSM buttonFSM );
eError ButtonsFSMGpioCheck( tButtons buttonNumber );
#endif

#if defined(MON_BUTTON_TIMER)
eError ButtonsCheckTimerValue( tPDTimerButtons timerButton,  tButtons button);
#endif

#if defined(MON_BUTTON_ADC)
eError ButtonsCheckADCValue( tPDAdcButtons adcButton, tButtons button );
#endif

/****************************************************************************
 *  PUBLIC FUNCTIONS
 ****************************************************************************/


/*************************************************************************//**
 * @brief  Initializes button.
 *
 * @param	None.
 *
 * @return  RET_OK or RET_FAIL.
 *
 ****************************************************************************/
eError ButtonsInit( void )
{
	eError result = RET_OK;
	uint16_t button;

	for (button = 0; button < NUM_OF_BUTTONS; button++)
	{
		buttonsContext[button].actualExpireTime = 0;
		buttonsContext[button].countPress = 0;
	}


	return result;
}

/*************************************************************************//**
 * @brief  Starts button.
 *
 * @param	None.
 *
 * @return  RET_OK or RET_FAIL.
 *
 ****************************************************************************/
eError ButtonsStart( void )
{
	eError result = RET_OK;
	uint16_t nPeriphBtn, nBtn;

	/* Init gpioContext*/
	for (nPeriphBtn = 0; nPeriphBtn < NUM_OF_GPIO_BUTTONS; nPeriphBtn++)
	{
		buttonsGPIOContext[nPeriphBtn].buttonState = BUTTON_STATE_NOT_PRESSED;
		buttonsGPIOContext[nPeriphBtn].actualFSM = BUTTON_FSM_WAITING_PRESS;
		buttonsGPIOContext[nPeriphBtn].actualTime = 0;
		buttonsGPIOContext[nPeriphBtn].numButtons = 0;
		buttonsGPIOContext[nPeriphBtn].nonReleaseAction = 0;
		/* Calculate the number of valid buttons */
		for ( nBtn = 0; nBtn < NUM_MAX_BUTTONS; nBtn++ )
		{
			if ( buttonsPDGpioMap[nPeriphBtn].periphGPIO[nBtn] != NULL_BUTTON)
			{
				buttonsGPIOContext[nPeriphBtn].numButtons++;
			}
		}
	}
#if defined(MON_BUTTON_TIMER)
	/* Get Timer Resolution and Init TimerContext*/
	for (nPeriphBtn = 0; nPeriphBtn < NUM_OF_TIMER_BUTTONS; nPeriphBtn++)
	{
		buttonsTimerContext[nPeriphBtn].lastTimerValue = 0;
	}
#endif

#if defined(MON_BUTTON_ADC)
	/* Get ADC Resolution and Init ADCContext */
	for (nPeriphBtn = 0; nPeriphBtn < NUM_OF_ADC_BUTTONS; nPeriphBtn++)
	{
		buttonsADCContext[nPeriphBtn].lastAdcValue = 0;
	}
#endif

	return result;
}

/*************************************************************************//**
 * @brief  Stops button.
 *
 * @param	None.
 *
 * @return  RET_OK or RET_FAIL.
 *
 ****************************************************************************/
eError  ButtonsStop( void )
{
	eError result = RET_OK;

	return result;
}

/*************************************************************************//**
 * @brief  Sleeps button.
 *
 * @param	None.
 *
 * @return  RET_OK or RET_FAIL.
 *
 ****************************************************************************/
eError  ButtonsSleep( void )
{
	eError result = RET_OK;

	ButtonsStop();

	return result;
}

/*************************************************************************//**
 * @brief  Wakes button
 *
 * @param	None.
 *
 * @return  RET_OK or RET_FAIL.
 *
 ****************************************************************************/
eError  ButtonsWake( void )
{
	eError result = RET_OK;

	ButtonsStart();

	return result;
}


/*************************************************************************//**
 * @brief  Get Button State Function.
 *
 * @param	button	-> Button instance
 *
 * @return  button state:  BUTTON_PRESSED, BUTTON_NO_PRESSED,
 *
 ****************************************************************************/
tButtonAction ButtonsGetState(tButtons button, uint8_t *countPress)
{
	if ( buttonsContext[button].countPress > 0 )
	{
		buttonsContext[button].countPress--;
		*countPress = buttonsContext[button].countPress;
		return BUTTON_EVENT_AVAILABLE;
	}
	*countPress = 0;
	return BUTTON_NO_EVENT;
}



/*************************************************************************//**
 * @brief  Callback Function
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
void ButtonsTask( void )
{
	static tButtons buttonNumber = 0;
	uint16_t nBtn = 0;

	/* Update actualTime and ExpireTime */
	while ( nBtn < NUM_OF_BUTTONS )
	{
		if ( buttonsInstanceMap[nBtn].buttonType == BUTTON_TYPE_GPIO )
		{
			buttonsGPIOContext[buttonsInstanceMap[nBtn].indexType].actualTime++;
		}

		if ( buttonsContext[nBtn].countPress > 0 && buttonsInstanceMap[nBtn].expireTime != 0 )
		{
			buttonsContext[nBtn].actualExpireTime++;
		}
		nBtn++;
	}
	
	nBtn = 0;
	/* Sequence to Check Buttons */
	while ( nBtn < BUTTONS_TO_CHECK )
	{
		ButtonsCheckPeripheralValue( buttonNumber );
		ButtonsCheckExpiredTime ( buttonNumber );
		buttonNumber++;

		/* Reset buttonNumber */
		if (buttonNumber == NUM_OF_BUTTONS)
		{
			buttonNumber = 0;
		}
		nBtn++;
	}

}



/*************************************************************************//**
 * @brief  Checks the peripheral button Value
 *
 * @param	None
 *
 * @return  RET_OK or RET_FAIL.
 *
 ****************************************************************************/
eError ButtonsCheckPeripheralValue( tButtons buttonNumber )
{
	eError result = RET_OK;
	uint16_t nPeriphBtn;
	
	nPeriphBtn =  buttonsInstanceMap[buttonNumber].indexType;

	/* GPIO TYPE */
	if ( buttonsInstanceMap[buttonNumber].buttonType == BUTTON_TYPE_GPIO )
	{
#if defined(MON_BUTTON_GPIO)
		ButtonsCheckGPIOValue(nPeriphBtn);
		result = ButtonsFSMGpioCheck( buttonNumber );
#endif
	}

#if defined(MON_BUTTON_TIMER)
	/* TIMER TYPE */
	else if ( buttonsInstanceMap[buttonNumber].buttonType == BUTTON_TYPE_TIMER )
	{
		ButtonsCheckTimerValue(nPeriphBtn, buttonNumber);
	}
#endif

#if defined(MON_BUTTON_ADC)
	/* ADC TYPE */
	else if ( buttonsInstanceMap[buttonNumber].buttonType == BUTTON_TYPE_ADC )
	{
		ButtonsCheckADCValue(nPeriphBtn, buttonNumber);
	}
#endif
	return result;
}


#if defined(MON_BUTTON_TIMER)
/*************************************************************************//**
 * @brief  Checks the peripheral TIMER Value
 *
 * @param	timerButton
 *
 * @return  RET_OK or RET_FAIL.
 *
 ****************************************************************************/
eError ButtonsCheckTimerValue( tPDTimerButtons timerButton,  tButtons button)
{
	eError result = RET_OK;
	tBool btnTimerAction = FALSE;
	uint8_t btnTim;
	int32_t timerValue;

	/* Get from Timer value and turns of encoder int32_t casting for GetValue reasons */
	timerValue = (int32_t)TimerGetValue (buttonsPDTimerMap[timerButton].periphTimer);
	timerValue /= buttonsPDTimerMap[timerButton].filterEnc ;

	if ( (timerValue > buttonsTimerContext[timerButton].lastTimerValue) && (buttonsPDTimerMap[timerButton].timerValue == TIMER_INCREMENT) )
	{
		/* Button Callback, NO increment countPress*/
		if ( buttonsInstanceMap[button].processCallBackFunction != NULL )
		{
			buttonsInstanceMap[button].processCallBackFunction(button);
		}
		/* Button Callback NULL -> Increment count Press */
		else
		{
			buttonsContext[button].countPress++;
		}

		btnTimerAction = TRUE;
	}

	else if ( (timerValue < buttonsTimerContext[timerButton].lastTimerValue) && (buttonsPDTimerMap[timerButton].timerValue == TIMER_DECREMENT) )
	{
		/* Button Callback, NO increment countPress*/
		if ( buttonsInstanceMap[button].processCallBackFunction != NULL )
		{
			buttonsInstanceMap[button].processCallBackFunction(button);
		}
		/* Button Callback NULL -> Increment count Press */
		else
		{
			buttonsContext[button].countPress++;
		}

		btnTimerAction = TRUE;
	}


	if ( btnTimerAction == TRUE )
	{
		/* we need to update the lastTimeValue for all button timer with same instance timer */
		for ( btnTim=0; btnTim < NUM_OF_TIMER_BUTTONS; btnTim++ )
		{
			if ( buttonsPDTimerMap[btnTim].periphTimer == buttonsPDTimerMap[timerButton].periphTimer )
			{
				buttonsTimerContext[btnTim].lastTimerValue = timerValue;
			}
		}
		buttonsContext[button].actualExpireTime = 0;
		buttonsGPIOContext[timerButton].actualTime = 0;
	}

	return result;
}
#endif

#if defined(MON_BUTTON_GPIO)
/*************************************************************************//**
 * @brief  Checks the peripheral GPIO Value
 * @param	gpioButton
 * @return  RET_OK or RET_FAIL.
 ****************************************************************************/
eError ButtonsCheckGPIOValue( tPDGpioButtons gpioButton )
{
	eError result = RET_OK;
	tGPIOValue buttonState;
	uint16_t nBtn = 0;
	tBool btnFound = FALSE;

	while ((nBtn < NUM_MAX_BUTTONS) && (btnFound == FALSE))
	{
		/* If gpioButton is NULL_BUTTON , this gpioButton is the last gpioButton */
		if ( buttonsPDGpioMap[gpioButton].periphGPIO[nBtn] != NULL_BUTTON )
		{
			/* Read the gpioButton value */
			result = READ_HREG(buttonsPDGpioMap[gpioButton].periphGPIO[nBtn], buttonsPDGpioMap[gpioButton].hregPtrValue, &buttonState);
			if ( buttonsPDGpioMap[gpioButton].pushedValue == buttonState )
			{
				buttonsGPIOContext[gpioButton].buttonState = BUTTON_STATE_PRESSED;
			}
			else
			{
				buttonsGPIOContext[gpioButton].buttonState = BUTTON_STATE_NOT_PRESSED;
				btnFound = TRUE;
			}
		}
		else
		{
			btnFound = TRUE;
		}
		nBtn++;
	}

	return result;
}

/*************************************************************************//**
 * @brief  Change the GPIO buttons FSM State
 *
 * @param	gpioButton
 *
 * @return  RET_OK or RET_FAIL.
 *
 ****************************************************************************/
eError ButtonsChangeStateGPIOButtons( tPDGpioButtons gpioButton, tButtonFSM buttonFSM )
{
	eError result = RET_OK;
	uint16_t pBtn, nBtn;

	// Inhibit flag for other buttons with same index
	for ( pBtn=0; pBtn < NUM_OF_GPIO_BUTTONS; pBtn++)
	{
		for ( nBtn=0; nBtn < NUM_MAX_BUTTONS; nBtn++)
		{
			if ((pBtn != gpioButton) &&  (buttonsPDGpioMap[pBtn].periphGPIO[0] == buttonsPDGpioMap[gpioButton].periphGPIO[nBtn]))
			{
				buttonsGPIOContext[pBtn].actualFSM = buttonFSM;
			}
		}
	}

	return result;
}

#endif

#if defined(MON_BUTTON_ADC)
/*************************************************************************//**
 * @brief  Checks the peripheral ADC Value
 *
 * @param	adcButton, button
 *
 * @return  RET_OK or RET_FAIL.
 *
 ****************************************************************************/
eError ButtonsCheckADCValue( tPDAdcButtons adcButton, tButtons button )
{
	eError result = RET_OK;
	uint32_t adcValue, numChannels;

	result = READ_HREG(buttonsPDAdcMap[adcButton].groupADC,  buttonsPDAdcMap[adcButton].hregPtrGroup, &numChannels);
	result = READ_HREG(buttonsPDAdcMap[adcButton].periphADC, buttonsPDAdcMap[adcButton].hregPtrValue, &adcValue);
	if ( adcValue != buttonsADCContext[adcButton].lastAdcValue )
	{
		if ( adcValue >= buttonsPDAdcMap[adcButton].ADCValue )
		{
			buttonsContext[button].countPress++;
			buttonsContext[button].actualExpireTime = 0;
		}
	}

	return result;
}
#endif


/*************************************************************************//**
 * @brief   Update Expired Time of Buttons
 *
 * @param	button
 *
 * @return  RET_OK or RET_FAIL.
 *
 ****************************************************************************/
eError ButtonsCheckExpiredTime( tButtons buttonNumber )
{
	eError result = RET_OK;

	if (buttonsContext[buttonNumber].countPress > 0 && buttonsInstanceMap[buttonNumber].expireTime != 0 )
	{
		/* Time expired ? */
		if ((buttonsContext[buttonNumber].actualExpireTime*BUTTONS_SCHEDULER_CALLBACK_TIMER_MS) >=  buttonsInstanceMap[buttonNumber].expireTime )
		{
			/* Reset the flag and time of Button */
			buttonsContext[buttonNumber].countPress = 0;
			buttonsContext[buttonNumber].actualExpireTime = 0;

			if ( buttonsInstanceMap[buttonNumber].buttonType == BUTTON_TYPE_GPIO )
			{
				buttonsGPIOContext[buttonsInstanceMap[buttonNumber].indexType].nonReleaseAction = 0;
				buttonsGPIOContext[buttonsInstanceMap[buttonNumber].indexType].actualFSM = BUTTON_FSM_WAITING_PRESS;

				if ( buttonsGPIOContext[buttonsInstanceMap[buttonNumber].indexType].numButtons > 1 )
				{
					ButtonsChangeStateGPIOButtons(buttonsInstanceMap[buttonNumber].indexType, BUTTON_FSM_WAITING_PRESS);
				}
			}
		}
	}
	else
	{
		buttonsContext[buttonNumber].actualExpireTime = 0;
	}

	return result;
}


#if defined(MON_BUTTON_GPIO)
/*************************************************************************//**
 * @brief  Checks the button GPIO STATE
 * @param	button -> Button instance
 * @return  RET_OK or RET_FAIL.
 ****************************************************************************/
eError ButtonsFSMGpioCheck( tButtons buttonNumber )
{
	eError result = RET_OK;
	uint32_t time;
	uint16_t nPeriphBtn;

	nPeriphBtn =  buttonsInstanceMap[buttonNumber].indexType;

	switch ( buttonsGPIOContext[nPeriphBtn].actualFSM )
	{

	case BUTTON_FSM_WAITING_PRESS:
		if ( buttonsGPIOContext[nPeriphBtn].buttonState == BUTTON_STATE_PRESSED )
		{
			buttonsGPIOContext[nPeriphBtn].actualFSM = BUTTON_FSM_PRESS;
			buttonsGPIOContext[nPeriphBtn].actualTime = 0;

			/* Inhibit other index instances */
			if ( buttonsGPIOContext[nPeriphBtn].numButtons > 1 )
			{
				ButtonsChangeStateGPIOButtons(nPeriphBtn, BUTTON_FSM_USED);
			}
		}
		break;


	case BUTTON_FSM_PRESS:
		time = buttonsGPIOContext[nPeriphBtn].actualTime*BUTTONS_SCHEDULER_CALLBACK_TIMER_MS;

		/* Button activated by release */
		if ( buttonsPDGpioMap[nPeriphBtn].maxPushTime > 0 )
		{
			if ((buttonsGPIOContext[nPeriphBtn].buttonState == BUTTON_STATE_NOT_PRESSED) && ( time < buttonsPDGpioMap[nPeriphBtn].maxPushTime)
					&& ( time >= buttonsPDGpioMap[nPeriphBtn].minPushTime))
			{
				buttonsContext[buttonNumber].actualExpireTime = 0;
				buttonsGPIOContext[nPeriphBtn].actualFSM = BUTTON_FSM_WAITING_PRESS;

				/* Remove the Inhibit state */
				if ( buttonsGPIOContext[nPeriphBtn].numButtons > 1 )
				{
					ButtonsChangeStateGPIOButtons(nPeriphBtn, BUTTON_FSM_WAITING_PRESS);
				}

				/* Button Callback, NO increment countPress*/
				if ( buttonsInstanceMap[buttonNumber].processCallBackFunction != NULL )
				{
					buttonsInstanceMap[buttonNumber].processCallBackFunction(buttonNumber);
				}
				/* Button Callback NULL -> Increment count Press */
				else
				{
					buttonsContext[buttonNumber].countPress++;
				}
			}

			else if ((buttonsGPIOContext[nPeriphBtn].buttonState == BUTTON_STATE_NOT_PRESSED) && ( time < buttonsPDGpioMap[nPeriphBtn].minPushTime))
			{
				buttonsGPIOContext[nPeriphBtn].actualFSM = BUTTON_FSM_WAITING_PRESS;

				/* Remove the Inibit state */
				if ( buttonsGPIOContext[nPeriphBtn].numButtons > 1 )
				{
					ButtonsChangeStateGPIOButtons(nPeriphBtn, BUTTON_FSM_WAITING_PRESS);
				}

			}

			else if ((buttonsGPIOContext[nPeriphBtn].buttonState == BUTTON_STATE_NOT_PRESSED) && ( time > buttonsPDGpioMap[nPeriphBtn].maxPushTime))
			{
				buttonsGPIOContext[nPeriphBtn].actualFSM = BUTTON_FSM_WAITING_PRESS;

				/* Remove the Inibit state */
				if ( buttonsGPIOContext[nPeriphBtn].numButtons > 1 )
				{
					ButtonsChangeStateGPIOButtons(nPeriphBtn, BUTTON_FSM_WAITING_PRESS);
				}
			}
		}

		/* Button activated by NON-release */
		else
		{
			if ((buttonsGPIOContext[nPeriphBtn].buttonState == BUTTON_STATE_PRESSED) && ( time >= buttonsPDGpioMap[nPeriphBtn].minPushTime)
					&& buttonsGPIOContext[nPeriphBtn].nonReleaseAction == 0 )
			{
				buttonsContext[buttonNumber].actualExpireTime = 0;
				buttonsGPIOContext[nPeriphBtn].nonReleaseAction = 1;

				/* Button Callback, no increment countPress*/
				if ( buttonsInstanceMap[buttonNumber].processCallBackFunction != NULL )
				{
					buttonsInstanceMap[buttonNumber].processCallBackFunction(buttonNumber);
				}

				/* Button Callback NULL -> Increment count Press */
				else
				{
					buttonsContext[buttonNumber].countPress++;
				}
			}

			else if ( buttonsGPIOContext[nPeriphBtn].buttonState == BUTTON_STATE_NOT_PRESSED )
			{
				buttonsGPIOContext[nPeriphBtn].actualFSM = BUTTON_FSM_WAITING_PRESS;
				buttonsGPIOContext[nPeriphBtn].nonReleaseAction = 0;
				if ( buttonsGPIOContext[nPeriphBtn].numButtons > 1 )
				{
					ButtonsChangeStateGPIOButtons(nPeriphBtn, BUTTON_FSM_WAITING_PRESS);
				}
			}
		}

		break;

	case BUTTON_FSM_USED:
		break;

	}

	return result;
}

#endif


#endif // _BUTTONS_C_

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
