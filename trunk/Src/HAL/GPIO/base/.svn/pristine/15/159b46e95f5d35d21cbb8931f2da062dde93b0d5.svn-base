/*****************************************************************************
 * @file
 * @addtogroup HAL
 * @{
 * @addtogroup GPIO
 * @{
 *
 * @brief 	GPIO control driver.
 *
 * @author 	FV4MMA2
 *
 * @date 	28/01/2016
 *
 ****************************************************************************/

#ifndef _GPIO_C_
#define _GPIO_C_

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
#include "GPIO.h"
#include "GPIO_map.h"

/****************************************************************************
 *  EXTERNAL VARIABLES
 ****************************************************************************/
extern const tGPIOInstanceMap GPIOInstanceMap[NUM_OF_GPIO];

/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/

/****************************************************************************
 *  PRIVATE FUNCTIONS DEFINITIONS
 ****************************************************************************/
void InitGPIOClock(tGPIO port);


/****************************************************************************
 *  PUBLIC FUNCTIONS
 ****************************************************************************/

/*****************************************************************************
 * @brief  	Initializes GPIO driver.
 * 		   	Initializes all GPIO declared on GPIO_map list
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError GPIOInit( void )
{
	eError 				success = RET_OK;
	uint8_t 			port;
	GPIO_InitTypeDef  	GPIO_InitStructure;

	for (port = 0; port < NUM_OF_GPIO; port++)
	{
		/* Enable GPIO clock of corresponding port */
		InitGPIOClock(port);

		/* Configure the GPIO parameters */
        GPIO_InitStructure.Pin =  	(uint32_t)GPIOInstanceMap[port].pin;
		GPIO_InitStructure.Mode = 	(uint32_t)GPIOInstanceMap[port].mode;
		GPIO_InitStructure.Speed = 	(uint32_t)GPIOInstanceMap[port].speed;
		GPIO_InitStructure.Pull = 	(uint32_t)GPIOInstanceMap[port].pupd;
		GPIO_InitStructure.Alternate = (uint32_t)GPIOInstanceMap[port].alternate;

		HAL_GPIO_Init(GPIOInstanceMap[port].port, &GPIO_InitStructure);

		/* Set GPIO initial value */
		if ( GPIOInstanceMap[port].initialValue != GPIO_DEFAULT )
		{
			success = GPIOWritePort(port, GPIOInstanceMap[port].initialValue);
		}
		/* No else needed */
	}

	return success;
}

/*************************************************************************//**
 * @brief  	Stops GPIO driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError GPIOStop( void )
{
	eError 	success = RET_OK;

	return success;
}

/*************************************************************************//**
 * @brief  	Starts GPIO driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError GPIOStart( void )
{
	eError 	success = RET_OK;

	return success;
}

/*************************************************************************//**
 * @brief  	Sleeps GPIO driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError GPIOSleep( void )
{
	eError 	success = RET_OK;

	return success;
}

/*************************************************************************//**
 * @brief  	Wakes up GPIO driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError GPIOWake( void )
{
	eError 	success = RET_OK;

	return success;
}

/*************************************************************************//**
 * @brief  	Writes GPIO port with desired value.
 * @param	port to write.
 * @param   value
 * @return  success.
 ****************************************************************************/
eError GPIOWritePort(tGPIO port, tGPIOValue value)
{
	eError 	success = RET_OK;

	if ( value == GPIO_DEFAULT )
	{
		success = RET_INVALID_PARAMETER;
		return success;
	}


	if ( value != GPIO_TOGGLE )
	{
		HAL_GPIO_WritePin(GPIOInstanceMap[port].port, GPIOInstanceMap[port].pin, value);
	}
	else
	{
		HAL_GPIO_TogglePin(GPIOInstanceMap[port].port, GPIOInstanceMap[port].pin);
	}

	return success;
}

/*************************************************************************//**
 * @brief  	Reads GPIO port.
 * @param	port to read.
 * @param   value
 * @return  success.
 ****************************************************************************/
eError GPIOReadPort(tGPIO port,  tGPIOValue *value)
{
	eError 	success = RET_OK;

	*value = HAL_GPIO_ReadPin(GPIOInstanceMap[port].port, GPIOInstanceMap[port].pin);

	return success;
}


/****************************************************************************
 *    PRIVATE FUNCTIONS
 ****************************************************************************/

/*************************************************************************//**
 * @brief  Enable GPIO clock.
 * @param	port to enable clock.
 * @return  None.
 ****************************************************************************/
void InitGPIOClock(tGPIO port)
{
	/* Enable the GPIO Clock */
#ifdef GPIOA
	if ( GPIOInstanceMap[port].port == GPIOA ){
		__GPIOA_CLK_ENABLE();
	}
#endif //GPIOA

#ifdef GPIOB
	if( GPIOInstanceMap[port].port == GPIOB ){
		__GPIOB_CLK_ENABLE();
	}
#endif //GPIOB

#ifdef GPIOC
	if( GPIOInstanceMap[port].port == GPIOC ){
		__GPIOC_CLK_ENABLE();
	}
#endif //GPIOC

#ifdef GPIOD
	if( GPIOInstanceMap[port].port == GPIOD ){
		__GPIOD_CLK_ENABLE();
	}
#endif //GPIOD

#ifdef GPIOE
	if( GPIOInstanceMap[port].port == GPIOE ){
		__GPIOE_CLK_ENABLE();
	}
#endif //GPIOE

#ifdef GPIOF
	if( GPIOInstanceMap[port].port == GPIOF ){
		__GPIOF_CLK_ENABLE();
	}
#endif //GPIOF

#ifdef GPIOG
	if( GPIOInstanceMap[port].port == GPIOG ){
		__GPIOG_CLK_ENABLE();
	}
#endif //GPIOG

#ifdef GPIOH
	if( GPIOInstanceMap[port].port == GPIOH ){
		__GPIOH_CLK_ENABLE();
	}
#endif //GPIOH

#ifdef GPIOI
	if( GPIOInstanceMap[port].port == GPIOI ){
		__GPIOI_CLK_ENABLE();
	}
#endif //GPIOI


}



#endif // _GPIO_C_

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
