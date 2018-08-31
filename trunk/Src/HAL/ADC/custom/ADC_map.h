/*****************************************************************************
 * @file 	ADC_map.h
 *
 * @brief 	ADC map declaration
 *
 * @author 	FV4MMA
 *
 * @date 	03/02/2016
 *
 ****************************************************************************/

#ifndef	_ADC_MAP_H_
#define	_ADC_MAP_H_

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "Common.h"

/***********************************************************************
 * COMMON ARCHITECTURE INCLUDES
 **********************************************************************/

/***********************************************************************
 * INCLUDES
 **********************************************************************/
#define IS_ADC_1



/***********************************************************************
 * DEFINES
 **********************************************************************/

#define ADC_TO_DMA2_STREAM0 ADC_1
typedef enum eADCCommon
{
#ifdef IS_ADC_1
	ADC_1 = 0,
#endif
#ifdef IS_ADC_2
	ADC_2,
#endif
#ifdef IS_ADC_3
	ADC_3,
#endif
	NUM_OF_ADC
} tADCCommon;


typedef enum eADC
{
#ifdef IS_ADC_2
	ADC_ANALOG_1,
	ADC_ANALOG_2,

	ADC_MCU_OC_1,
	ADC_MCU_OC_2,
	ADC_VREFINT,
#endif
#ifdef IS_ADC_1
	ADC_BLDC1_CUR1,
	ADC_BLDC1_CUR2,
	ADC_BLDC1_CUR3,
	ADC_BLDC2_CUR1,
	ADC_BLDC2_CUR2,
	ADC_BLDC2_CUR3,
#endif

	NUM_OF_ADC_DEVICES

} tADC;


typedef enum eAdcDma
{
	ADC_DMA_1 = 0,
	NUM_OF_ADC_DMA,
	ADC_POLLING = 0xFF,

}tAdcDma;
/***********************************************************************
 * TYPE DEFINITIONS
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUCNTION PROTOTYPES
 **********************************************************************/

#endif	/*_ADC_MAP_H_*/

/***********************************************************************
 * End of file
 **********************************************************************/
