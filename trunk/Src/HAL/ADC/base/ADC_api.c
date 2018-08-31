/***********************************************************************
 * @file	ADC_api.c
 *
 * @brief	ADC Hardware registers
 *
 * @author	Albert Serrallo
 *
 * @date	2016-03-11
 *
 **********************************************************************/
#ifndef	_ADC_API_C_
#define	_ADC_API_C_

/***********************************************************************
 * PRIVATE INCLUDES
 **********************************************************************/
#include "ADC.h"


/***********************************************************************
 * PUBLIC ARCHITECTURE INCLUDES
 **********************************************************************/
#include "Hreg.h"
#include "Hreg_api.h"

/***********************************************************************
 * PRIVATE DEFINES
 **********************************************************************/

/***********************************************************************
 * PRIVATE TYPES
 **********************************************************************/

/***********************************************************************
 * PRIVATE VARIABLES DEFINITIONS
 **********************************************************************/

/***********************************************************************
 * PRIVATE FUNCTIONS DECLARATIONS
 **********************************************************************/

/***********************************************************************
 * PRIVATE FUNCTIONS DEFINITIONS
 **********************************************************************/

/***********************************************************************
 * PUBLIC VARIABLES DEFINITIONS
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUNCTIONS DEFINITIONS
 **********************************************************************/


/***********************************************************************
 * @brief	API for init Adc.
 *
 **********************************************************************/
__weak eError initHregADC( void )
{
	eError result = RET_OK;

	result = ADCInit();

	return result;
}

/***********************************************************************
 * @brief	API for start Adc.
 *
 **********************************************************************/
__weak eError startHregADC( void )
{
	eError result = RET_OK;

	result = ADCStart();

	return result;
}

/***********************************************************************
 * @brief	API for stop Adc.
 *
 **********************************************************************/
__weak eError stopHregADC( void )
{
	eError result = RET_OK;

	result = ADCStop();

	return result;
}

/***********************************************************************
 * @brief	API for sleep Adc.
 *
 **********************************************************************/
__weak eError sleepHregADC( void )
{
	eError result = RET_OK;

	result = ADCSleep();

	return result;
}

/***********************************************************************
 * @brief	API for wake Adc.
 *
 **********************************************************************/
__weak eError wakeHregADC( void )
{
	eError result = RET_OK;

	result = ADCWake();

	return result;
}

/***********************************************************************
 * @brief	On Read function for Adc value.
 *
 **********************************************************************/
__weak eError onReadHregADCValue( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	result = ADCGetValue((tADC)instance, (uint32_t*) param1);

	return result;
}

/***********************************************************************
 * @brief	On Read function for Adc resolution.
 *
 **********************************************************************/
__weak eError onReadHregADCResolution( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	result = ADCGetResolution((tADCCommon)instance, (uint32_t*) param1);

	return result;
}

/***********************************************************************
 * @brief	On Read function for Adc group.
 *
 **********************************************************************/
__weak eError onReadHregADCGroup( uint8_t instance, const HREG* hregPtr, void* param1 )
{
	eError result = RET_OK;

	result = ADCGetChannelConversion((tADC)instance);

	*(uint8_t *)param1 = 1;

	return result;
}



#endif	/*_ADC_API_C_*/


