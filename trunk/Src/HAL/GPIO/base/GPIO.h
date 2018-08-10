/*****************************************************************************
 * @file
 * @addtogroup HAL
 * @{
 * @addtogroup GPIO
 * @{

 *
 * @brief 	GPIO functionality Header.
 *
 * @author 	FV4MMA2
 *
 * @date 	28/01/2016
 *
 ****************************************************************************/
#ifndef _GPIO_H_
#define _GPIO_H_

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
#include "GPIO_map.h"

/****************************************************************************
 * DEFINES
 ****************************************************************************/
#define GPIO_AF_NONE		0xFF

/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

typedef enum eGPIOValue
{
	GPIO_LOW,
	GPIO_HIGH,
	GPIO_TOGGLE,
	GPIO_DEFAULT
} tGPIOValue;

typedef struct sGPIOInstanceMap
{
	GPIO_TypeDef*	  	port;
	uint16_t 			pin;
	uint32_t 			mode;
	uint8_t				speed;
	uint8_t    			pupd;
	uint8_t				alternate;
	tGPIOValue			initialValue;
} tGPIOInstanceMap;


/****************************************************************************
 *    PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************/
eError GPIOInit( void );
eError GPIOStop( void );
eError GPIOStart( void );
eError GPIOSleep( void );
eError GPIOWake( void );
eError GPIOWritePort(tGPIO port, tGPIOValue value);
eError GPIOReadPort(tGPIO port,  tGPIOValue *value);

#endif // _GPIO_H_

/****************************************************************************
 *    End of file
 ***************************************************************************/
