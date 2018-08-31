/*****************************************************************************
 * @file 	ADC.c
 * @addtogroup HAL
 * @{
 * @addtogroup Adc
 * @{
 *
 * @brief 	ADC map control.
 *
 * @author 	Albert Serrallo
 *
 * @date 	24/08/2016
 *
 ****************************************************************************/

#ifndef _ADC_C_
#define _ADC_C_

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
#include "ADC.h"
/****************************************************************************
 *  EXTERN VARIABLES
 ****************************************************************************/
extern tADCPeripheralMap ADCPeripheralMap[NUM_OF_ADC];
extern tADCInstanceMap 	ADCInstanceMap[NUM_OF_ADC_DEVICES];
extern tAdcDmaMap AdcDmaMap[NUM_OF_ADC_DMA];
extern __IO uint16_t uhADCxConvertedValue[100];
extern DMA_HandleTypeDef  DMAHandleStructure[NUM_OF_ADC_DMA];
/****************************************************************************
 *  PRIVATE VARIABLES
 ****************************************************************************/
ADC_HandleTypeDef  ADCHandleStructure[NUM_OF_ADC];
static uint32_t ADCValueChannel[NUM_OF_ADC_DEVICES];
//static uint8_t dmaRankToChannel[NUM_OF_ADC][NUM_OF_ADC_DEVICES];


/****************************************************************************
 *  PRIVATE FUNCTIONS
 ****************************************************************************/

static HAL_StatusTypeDef initDMA(uint8_t index);
static uint8_t getNumberOfRanks(uint8_t index);

/****************************************************************************
 *    PRIVATE DEFINES
 -***************************************************************************/
#define CONVERT_TIMEOUT	(10) //milliseconds


/****************************************************************************
 * @brief  	Init ADC driver.
 * @param   None.
 * @return  None.
 ****************************************************************************/
eError ADCInit( void )
{
	eError success 	= RET_OK;

	return success;
}

/****************************************************************************
 * @brief  	Starts ADC driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError ADCStart( void )
{
	eError 	success 		= RET_OK;
	uint8_t commonIndex=0;

	HAL_StatusTypeDef status = HAL_OK;;


	/* Initializes the ADC according to parameters specified in map */
	while (commonIndex<NUM_OF_ADC && status == HAL_OK)
	{
		/* Enable ADC clock */
#ifdef IS_ADC_1
		if (((uint32_t)ADCPeripheralMap[commonIndex].adc & (uint32_t)ADC1_BASE) == ADC1_BASE)
		{
			__HAL_RCC_ADC_CLK_ENABLE();
		}
#endif
#ifdef IS_ADC_2
		if (((uint32_t)ADCPeripheralMap[commonIndex].adc & (uint32_t)ADC2_BASE) == ADC2_BASE)
		{
			__HAL_RCC_ADC_CLK_ENABLE();
		}
#endif
#ifdef IS_ADC_3
		if (((uint32_t)ADCPeripheralMap[commonIndex].adc & (uint32_t)ADC3_BASE) == ADC3_BASE)
		{
			__HAL_RCC_ADC_CLK_ENABLE();
		}
#endif

		/*##-1- Configure the ADC peripheral #######################################*/
		ADCHandleStructure[commonIndex].Instance = ADCPeripheralMap[commonIndex].adc;

		ADCHandleStructure[commonIndex].Init.ClockPrescaler 		= ADCPeripheralMap[commonIndex].ClockPrescaler;
		ADCHandleStructure[commonIndex].Init.Resolution 			= ADCPeripheralMap[commonIndex].resolution;
		ADCHandleStructure[commonIndex].Init.DataAlign 				= ADCPeripheralMap[commonIndex].dataAlign;

		ADCHandleStructure[commonIndex].Init.ScanConvMode 			= ENABLE;
		if (ADCPeripheralMap[commonIndex].dmaIndex != ADC_POLLING)
		{
			ADCHandleStructure[commonIndex].Init.ContinuousConvMode = ENABLE;
			ADCHandleStructure[commonIndex].Init.DMAContinuousRequests = ENABLE;
			status = initDMA(commonIndex);
			ADCHandleStructure[commonIndex].Init.NbrOfConversion = getNumberOfRanks(commonIndex);

		} else
		{
			ADCHandleStructure[commonIndex].Init.ContinuousConvMode = DISABLE;
			ADCHandleStructure[commonIndex].Init.DMAContinuousRequests = DISABLE;
			ADCHandleStructure[commonIndex].Init.NbrOfConversion		= 1;
		}
		ADCHandleStructure[commonIndex].Init.DiscontinuousConvMode 	= DISABLE;
		ADCHandleStructure[commonIndex].Init.NbrOfDiscConversion = 0;
		ADCHandleStructure[commonIndex].Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
		ADCHandleStructure[commonIndex].Init.ExternalTrigConv		= ADC_EXTERNALTRIG_T1_CC1;

		ADCHandleStructure[commonIndex].Init.EOCSelection 			= DISABLE;

		/* Initializes the ADC peripheral */
		status = HAL_ADC_Init(&ADCHandleStructure[commonIndex]);

		if (ADCPeripheralMap[commonIndex].dmaIndex != ADC_POLLING)
		{
			uint8_t i=0;

			while (i<NUM_OF_ADC_DEVICES && status == HAL_OK)
			{
				uint8_t countDmaChannels = 0;
				if (ADCInstanceMap[i].adc==commonIndex)
				{
					ADC_ChannelConfTypeDef sConfig;
					/*##-2- Configure ADC regular channel ######################################*/
					sConfig.Channel = ADCInstanceMap[i].channel;
					sConfig.Rank =  ADCInstanceMap[i].rank;
					dmaRankToChannel[commonIndex][ADCInstanceMap[i].rank-1] = i;
					sConfig.SamplingTime =  ADCInstanceMap[i].samplingTime;
					sConfig.Offset = 0;
					status = HAL_ADC_ConfigChannel(&ADCHandleStructure[commonIndex], &sConfig);
					countDmaChannels++;
				}
				i++;
			}
			if (status == HAL_OK)
			{
				/*##-3- Start the conversion process and enable interrupt ##################*/
				uint8_t indexDma =  ADCPeripheralMap[commonIndex].dmaIndex;
				status = HAL_ADC_Start_DMA(&ADCHandleStructure[commonIndex], AdcDmaMap[indexDma].DMA_buffer,AdcDmaMap[indexDma].DMA_BufferSize );
			}
		}
		commonIndex++;
	}

	if (status != HAL_OK)
	{
		success = RET_FAIL;
	}

	return success;
}


/****************************************************************************
 * @brief  	Stops ADC driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError ADCStop( void )
{
	eError 	success 		= RET_OK;
	uint8_t commonIndex 	= 0;

	HAL_StatusTypeDef status;

	for (commonIndex = 0; commonIndex < NUM_OF_ADC; commonIndex++)
	{
		/* Deinitialize ADC */
		status = HAL_ADC_DeInit(&ADCHandleStructure[commonIndex]);
		if (status == HAL_OK)
		{
			/* Disable ADC clock */
#ifdef IS_ADC_3
			if (((uint32_t)ADCPeripheralMap[commonIndex].adc & (uint32_t)ADC3_BASE) == ADC3_BASE)
			{
				__HAL_RCC_ADC_CLK_DISABLE();
			}
#endif
#ifdef IS_ADC_2
			if (((uint32_t)ADCPeripheralMap[commonIndex].adc & (uint32_t)ADC2_BASE) == ADC2_BASE)
			{
				__HAL_RCC_ADC_CLK_DISABLE();
			}
#endif
#ifdef IS_ADC_1
			if (((uint32_t)ADCPeripheralMap[commonIndex].adc & (uint32_t)ADC1_BASE) == ADC1_BASE)
			{
				__HAL_RCC_ADC_CLK_DISABLE();
			}
#endif
		}

		else
		{
			success = RET_FAIL;
		}
	}

	return success;
}

/****************************************************************************
 * @brief  	Sleeps ADC driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError ADCSleep( void )
{
	eError 	success = RET_OK;

	success = ADCStop();

	return success;
}


/****************************************************************************
 * @brief  	Wakes up ADC driver.
 * @param	None.
 * @return  None.
 ****************************************************************************/
eError ADCWake( void )
{
	eError 	success = RET_OK;

	success = ADCStart();

	return success;
}


/*****************************************************************************
 * @brief  	ADC get value.
 * @param	ADC channel.
 * @return  ADC value read.
 ****************************************************************************/
eError ADCGetValue( tADC ADCdevice, uint32_t *value)
{
	eError success 	= RET_OK;

	*value = ADCValueChannel[ADCdevice];

	return success;
}


/*****************************************************************************
 * @brief  	ADC conversion of group by polling: Starts ADC conversion and
 * 			waits until results are ready returning them.
 * @param	ADC group.
 * @return  Array of ADC values read.
 ****************************************************************************/
eError ADCGetChannelConversion(tADC ADCIndex)
{
	eError success = RET_OK;

	uint8_t commonIndex 	= ADCInstanceMap[ADCIndex].adc;
	uint32_t value 			= 0;

	ADC_ChannelConfTypeDef 	sConfig;
	HAL_StatusTypeDef 	status 	= HAL_OK;


	sConfig.Channel = ADCInstanceMap[ADCIndex].channel;
	sConfig.Rank = 1;
	sConfig.SamplingTime =  ADCInstanceMap[ADCIndex].samplingTime;
	sConfig.Offset = 0;
	status = HAL_ADC_ConfigChannel(&ADCHandleStructure[commonIndex], &sConfig);

				/* Activate the ADC peripheral and start conversions */

	status = HAL_ADC_Start(&ADCHandleStructure[commonIndex]);
	if(status == HAL_OK)
	{
		/* Wait for ADC conversion completion */
		status = HAL_ADC_PollForConversion(&ADCHandleStructure[commonIndex], CONVERT_TIMEOUT);
		if(status != HAL_OK)
		{
			success = RET_FAIL;
		}
	}
	/* Retrieve conversion results for each selected channel */
	value = HAL_ADC_GetValue(&ADCHandleStructure[commonIndex]);
	/* Save in table of values */
	ADCValueChannel[ADCIndex]  = value;

	/* Stop conversion and disable the ADC peripheral */
	status = HAL_ADC_Stop(&ADCHandleStructure[commonIndex]);

	return success;
}


/*****************************************************************************
 * @brief   Gets ADC resolution
 * @param   ADCport.
 * @return  ADC resolution
 ****************************************************************************/
eError ADCGetResolution( tADCCommon ADCconversor, uint32_t *resolution)
{
	eError success = RET_OK;

	switch(ADCPeripheralMap[ADCconversor].resolution){

	case ADC_RESOLUTION_12B:
		*resolution = 0xFFF;
		break;

	case ADC_RESOLUTION_10B:
		*resolution = 0x3FF;
		break;

	case ADC_RESOLUTION_8B:
		*resolution = 0xFF;
		break;

	case ADC_RESOLUTION_6B:
		*resolution = 0x3F;
		break;

	default:
		success = RET_FAIL;
		*resolution = 0;
		break;
	}

	return success;
}

/*****************************************************************************
 * @brief   ADC return the number of rank associated
 * @param   AdcChannel : tAdc
 * @return  Rank : uint8_t
 ****************************************************************************/
uint8_t ADCGetnRank ( tADC channel)
{
	return ADCInstanceMap[channel].rank;
}

/*****************************************************************************
 * @brief   ADC Conversion callback in non blocking mode
 * @param   AdcHandle : AdcHandle handle
 * @return  None
 ****************************************************************************/
void adcDriverHandler(ADC_HandleTypeDef* AdcHandle,tAdcCallBack type)
{
	uint8_t i,indexAdc = 0,indexDma;
	static int8_t j;
	static uint16_t adcSum[NUM_OF_ADC_DEVICES];
	static uint16_t adcBuffer[NUM_OF_ADC_DEVICES];

	/* Enable ADC clock */
#ifdef IS_ADC_3
	if (((uint32_t)AdcHandle->Instance & (uint32_t)ADC3_BASE) == ADC3_BASE)
	{

		indexAdc= ADC_3;

	}
#endif
#ifdef IS_ADC_2
	if (((uint32_t)AdcHandle->Instance & (uint32_t)ADC2_BASE) == ADC2_BASE)
	{
		indexAdc= ADC_2;
	}
#endif
#ifdef IS_ADC_1
	if (((uint32_t)AdcHandle->Instance & (uint32_t)ADC1_BASE) == ADC1_BASE)
	{
		indexAdc= ADC_1;
	}
#endif


	indexDma = ADCPeripheralMap[indexAdc].dmaIndex;

	switch (type)
	{
		case FULL_TRASFER:
			while (j>=0)
			{
				adcSum[j%AdcDmaMap[indexDma].DMA_nChannels] += uhADCxConvertedValue[j];
				j--;

			}
			break;
		case HALF_TRASFER:
			j=AdcDmaMap[indexDma].DMA_BufferSize-1;
			while (j>(AdcDmaMap[indexDma].DMA_BufferSize/2-1))
			{
				adcSum[j%AdcDmaMap[indexDma].DMA_nChannels] += uhADCxConvertedValue[j];
				j--;
			}
			break;
		default:
			break;
	}

	for (i=0;i<AdcDmaMap[indexDma].DMA_nChannels;i++)
	{
		adcBuffer[i] = adcSum[i]/(AdcDmaMap[indexDma].DMA_nCov/2);
		ADCValueChannel[dmaRankToChannel[indexAdc][i]] = adcBuffer[i];
		adcSum[i] = 0;
	}
	if (AdcDmaMap[indexDma].DMA_TransferComplete!=NULL)
	{
		AdcDmaMap[indexDma].DMA_TransferComplete(adcBuffer);
	}

}

static HAL_StatusTypeDef initDMA(uint8_t index)
{

	HAL_StatusTypeDef status = HAL_OK;

	uint8_t dmaIndex = ADCPeripheralMap[index].dmaIndex;

	AdcDmaMap[dmaIndex].DMA_nCov = AdcDmaMap[dmaIndex].DMA_BufferSize/AdcDmaMap[dmaIndex].DMA_nChannels;
	status = HAL_DMA_Init(&DMAHandleStructure[dmaIndex]);

	/* Associate the initialized DMA handle to the the ADC handle */
	__HAL_LINKDMA(&ADCHandleStructure[index], DMA_Handle, DMAHandleStructure[dmaIndex]);

	return status;
}

static uint8_t getNumberOfRanks(uint8_t index)
{
	uint8_t maxRank = 1,i;

	for (i=0;i<NUM_OF_ADC_DEVICES;i++)
	{
		if ((ADCInstanceMap[i].adc == index) && (maxRank < ADCInstanceMap[i].rank))
		{
			maxRank = ADCInstanceMap[i].rank;
		}
	}
	return maxRank;
}


/****************************************************************************
 *    End of file
 ***************************************************************************/

#endif // _ADC_C_

