/**************************************************************************//**
 * @file 	ADC.h
 * @addtogroup HAL
 * @{
 * @addtogroup Adc
 * @{

 * @brief 	ADC functionality Header.
 *
 * @author 	FV4MMA
 *
 * @date 	03/02/2016
 *
 ****************************************************************************/

#ifndef _ADC_H_
#define _ADC_H_

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
#include "ADC_map.h"
#define MAX_NUMB_GROUP 	(NUM_OF_ADC * NUM_OF_ADC)
/****************************************************************************
 * DEFINES
 ****************************************************************************/
#if defined(STM32L053xx)
#define NUMBER_OF_CHANNELS 18
#else
#define NUMBER_OF_CHANNELS 19
#endif
#define NUM_OF_ADC_GROUP NUM_OF_ADC_DEVICES
/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/
typedef struct sADCPeripheralMap
{
	ADC_TypeDef* 	adc;
	uint32_t 		ClockPrescaler;
	uint32_t 		resolution;
	uint32_t 		dataAlign;
	uint8_t			dmaIndex;

} tADCPeripheralMap;



typedef struct sADCInstanceMap
{
	tADCCommon 	adc;
	uint32_t 	channel;
	uint8_t		rank;
	uint32_t 	samplingTime;

} tADCInstanceMap;

typedef struct sAdcDmaMap
{
	uint32_t 				DMA_Channel;
	DMA_Channel_TypeDef		*DMA_Stream;
	uint32_t *				DMA_buffer;
	uint16_t				DMA_BufferSize;
	void  (*DMA_TransferComplete)(uint16_t * buffer);
	uint8_t					DMA_nChannels;
	uint8_t					DMA_nCov;
} tAdcDmaMap;

typedef enum eAdcCallback
{
	FULL_TRASFER =0,
	HALF_TRASFER,
	ERROR_TRASFER,
	OUT_OF_WINDOWS,

}tAdcCallBack;
/****************************************************************************
 *    PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************/
eError ADCInit( void );
eError ADCStop( void );
eError ADCStart( void );
eError ADCSleep( void );
eError ADCWake( void );

eError ADCGetChannelConversion(tADC ADCIndex);
eError ADCGetValue( tADC ADCdevice, uint32_t *value);
eError ADCGetResolution( tADCCommon ADCconversor, uint32_t *resolution);
void adcDriverHandler(ADC_HandleTypeDef* AdcHandle,tAdcCallBack type);
uint8_t ADCGetnRank ( tADC channel);


#endif // _ADC_H_

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
