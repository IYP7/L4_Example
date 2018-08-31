
/*****************************************************************************
 * @file 	ADC_map.c
 *
 * @brief 	ADC map declaration
 *
 * @author 	FV4MMA
 *
 * @date 	03/02/2016
 *
 ****************************************************************************/

#ifndef	_ADC_MAP_C_
#define	_ADC_MAP_C_

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "Common.h"

/***********************************************************************
 * COMMON ARCHITECTURE INCLUDES
 **********************************************************************/
#include "Hreg_dat.h"
/***********************************************************************
 * INCLUDES
 **********************************************************************/
#include "ADC.h"
#include "ADC_map.h"

/***********************************************************************
 * DEFINES
 **********************************************************************/

/***********************************************************************
 * TYPE DEFINITIONS
 **********************************************************************/

/***********************************************************************
 * EXTERN VARIABLES
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUCNTION PROTOTYPES
 **********************************************************************/
__weak void halfBridgePushCurrent(uint16_t *buffer)
{
	UNUSED(buffer);
}

/***********************************************************************
 * PUBLIC VARIABLES
 **********************************************************************/

__IO uint16_t uhADCxConvertedValue[100];

tADCPeripheralMap ADCPeripheralMap[NUM_OF_ADC] =
{
		/* {ADC,Prescaler Resolution, DataAlign, SamplingTime, activeChannels}
		 *  ADC,			--> AD,
		 *  Resolution		--> ADC_RESOLUTION_12B, ADC_RESOLUTION_10B, ADC_RESOLUTION_8B, ADC_RESOLUTION_6B,
		 *  DataAlign		--> ADC_DATAALIGN_RIGHT, ADC_DATAALIGN_LEFT,

		 *  					*/

#ifdef IS_ADC_1
	/* ADC_1 */	{ ADC1,ADC_CLOCKPRESCALER_PCLK_DIV2, ADC_RESOLUTION_12B, ADC_DATAALIGN_RIGHT, ADC_DMA_1 },
#endif
#ifdef IS_ADC_2
	/* ADC_2 */	{ ADC2,ADC_CLOCKPRESCALER_PCLK_DIV2, ADC_RESOLUTION_12B, ADC_DATAALIGN_RIGHT, ADC_POLLING }
#endif

};

const tADCInstanceMap ADCInstanceMap[NUM_OF_ADC_DEVICES] =  {
		/* {ADC, Channel, GPIO_mode, GPIO_speed, GPIO_pull, GPIO_AF, GPIO_Init_Value }
		 *
		 *  ADC,			--> ADC_1,
		 *  Channel			--> ADC_CHANNEL_0, ADC_CHANNEL_1, ... , ADC_CHANNEL_15, ADC_CHANNEL_TEMPSENSOR ,ADC_CHANNEL_VREFINT
		 *  Rank			--> [1..16]
		 *  samplingTime	--> ADC_SAMPLETIME_2CYCLES_5, ADC_SAMPLETIME_6CYCLES_5, ADC_SAMPLETIME_12CYCLES_5, ADC_SAMPLETIME_24CYCLES_5,
		 *  					ADC_SAMPLETIME_47CYCLES_5, ADC_SAMPLETIME_92CYCLES_5, ADC_SAMPLETIME_247CYCLES_5, ADC_SAMPLETIME_640CYCLES_5
		 *  					*/

#ifdef IS_ADC_2
		/* ADC_ANALOG_1 */		{ ADC_2, ADC_CHANNEL_14, 	  	1 ,ADC_SAMPLETIME_92CYCLES_5 },
		/* ADC_ANALOG_2 */		{ ADC_2, ADC_CHANNEL_15, 	  	1 ,ADC_SAMPLETIME_92CYCLES_5 },
		/* ADC_MCU_OC_1 */		{ ADC_2, ADC_CHANNEL_8, 	  	1 ,ADC_SAMPLETIME_92CYCLES_5 },
		/* ADC_MCU_OC_2 */		{ ADC_2, ADC_CHANNEL_9, 	  	1 ,ADC_SAMPLETIME_92CYCLES_5 },
		/* ADC_VREFINT */		{ ADC_2, ADC_CHANNEL_VREFINT, 	1 ,ADC_SAMPLETIME_92CYCLES_5 },
#endif
#ifdef IS_ADC_1
		/* ADC_BLDC1_CUR1 */	{ ADC_1, ADC_CHANNEL_10, 		1 ,ADC_SAMPLETIME_92CYCLES_5 },
		/* ADC_BLDC1_CUR2 */	{ ADC_1, ADC_CHANNEL_11, 		2 ,ADC_SAMPLETIME_92CYCLES_5 },
		/* ADC_BLDC1_CUR3 */	{ ADC_1, ADC_CHANNEL_12, 		3 ,ADC_SAMPLETIME_92CYCLES_5 },
		/* ADC_BLDC2_CUR1 */	{ ADC_1, ADC_CHANNEL_13, 		4 ,ADC_SAMPLETIME_92CYCLES_5 },
		/* ADC_BLDC2_CUR2 */	{ ADC_1, ADC_CHANNEL_4, 		5 ,ADC_SAMPLETIME_92CYCLES_5 },
		/* ADC_BLDC2_CUR3 */	{ ADC_1, ADC_CHANNEL_6, 		6 ,ADC_SAMPLETIME_92CYCLES_5 },
#endif


};


tAdcDmaMap AdcDmaMap[NUM_OF_ADC_DMA] =  {

		/*{DMA CHANNEL, DMA STREAM , IRQ , callback function , buffer, bufferSize}*/

		/* ADC_DMA_1 */{DMA_CHANNEL_0,DMA2_Stream0,(uint32_t*)&uhADCxConvertedValue,60,NULL,6}
};


/***********************************************************************
 * PRIVATE VARIABLES
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUCNTIONS
 **********************************************************************/

/***********************************************************************
 * PRIVATE FUCNTIONS
 **********************************************************************/

#endif	/*_ADC_MAP_C_*/

/***********************************************************************
 * End of file
 **********************************************************************/
