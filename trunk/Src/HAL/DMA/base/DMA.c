/*************************************************************************//**
 * @file DMA.c
 *
 * @brief DMA driver control.
 *
 * @author Gonzalo Serrano
 *
 * @date 27/10/2014
 *
 ****************************************************************************/

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
#include "DMA.h"

/****************************************************************************
 *  EXTERNAL VARIABLES
 ****************************************************************************/
extern const tDmaStreamMap dmaStreamMap[NUM_OF_DMA_STREAMS];
/****************************************************************************
 *  PRIVATE VARIABLES
 ***************************************************************************/

DMA_HandleTypeDef DMA_Handler[NUM_OF_DMA_STREAMS];

/****************************************************************************
 *    PRIVATE FUNCTIONS DECLARATIONS
 ****************************************************************************/


/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/

/*************************************************************************//**
 * @brief  Callback Handler for the DMA interrupt.
 * @param  dmaChannel Which channel caused the interrupt
 ****************************************************************************/
void dmaDriverMainIRQHandler(tDmaStream dmaChannel)
{
    DMA_HandleTypeDef* hdma; //HAL Uart

    hdma = &DMA_Handler[dmaChannel];
    HAL_DMA_IRQHandler(hdma);
}

/*************************************************************************//**
 * @brief  Init DMA driver.
 * @return  eError.
 *
 ****************************************************************************/
eError dmaInit(void)
{
	eError result = RET_OK;
	tDmaStream dmaStream;
	DMA_HandleTypeDef* hdma;

	for (dmaStream = 0; dmaStream<NUM_OF_DMA_STREAMS; dmaStream++)
	{
		hdma = &DMA_Handler[dmaStream];

		/* Deinitialize the stream for new transfer */
		HAL_DMA_DeInit(hdma);

		 /* Determine the DMA to which belongs the stream */
		hdma->Instance = dmaStreamMap[dmaStream].DMAy_Streamx;
		hdma->Init.Request = dmaStreamMap[dmaStream].Request;
		hdma->Init.Direction = dmaStreamMap[dmaStream].DIR;
		hdma->Init.PeriphInc =  dmaStreamMap[dmaStream].PeripheralInc;
		hdma->Init.MemInc = dmaStreamMap[dmaStream].MemoryInc;
		hdma->Init.PeriphDataAlignment = dmaStreamMap[dmaStream].PeripheralDataSize;
		hdma->Init.MemDataAlignment = dmaStreamMap[dmaStream].MemoryDataSize;
		hdma->Init.Mode = dmaStreamMap[dmaStream].Mode;
		hdma->Init.Priority = dmaStreamMap[dmaStream].Priority;

		/* Configure the DMA stream */
		HAL_DMA_Init(hdma);

	}

	return result;

}

/*************************************************************************//**
 * @brief  Stops DMA driver.
 *
 * @return  None.
 *
 ****************************************************************************/
eError dmaStop( void )
{
	eError result = RET_OK;

	return result;
}

/*************************************************************************//**
 * @brief  Starts DMA driver.
 *
 * @return  None.
 *
 ****************************************************************************/
eError dmaStart( void )
{
	eError result = RET_OK;
	tDmaStream dmaStream;
	DMA_HandleTypeDef* hdma;

	for (dmaStream = 0; dmaStream<NUM_OF_DMA_STREAMS; dmaStream++)
	{
		hdma = &DMA_Handler[dmaStream];

		/* Enable DMA2 clocks */
		if (hdma->Instance < DMA2_Channel1)
		{
			/* DMA1 */
			__HAL_RCC_DMA1_CLK_ENABLE();
		}
		else if ((hdma->Instance >= DMA2_Channel1) && (hdma->Instance <= DMA2_Channel7 ))
		{
			/* DMA2 */
			__HAL_RCC_DMA2_CLK_ENABLE();
		}

		/* Configure DMA Rx parameters */
		__HAL_DMA_CLEAR_FLAG(hdma, DMA_FLAG_GL4 | DMA_FLAG_TE4 | DMA_FLAG_HT4 | DMA_FLAG_TC4);

		if(dmaStreamMap[dmaStream].dmaInitState == TRUE)
		{
			__HAL_DMA_ENABLE(hdma);
		}
		else
		{
			__HAL_DMA_DISABLE(hdma);
		}

	}

	return result;
}


/*************************************************************************//**
 * @brief  Sleeps DMA driver.
 *
 * @return  None.
 *
 ****************************************************************************/
eError dmaSleep( void )
{
	eError result = RET_OK;
	return result;

}

/*************************************************************************//**
 * @brief  Wakes up dma driver.
 *
 * @return  None.
 *
 ****************************************************************************/
eError dmaWake( void )
{
	eError result = RET_OK;

	return result;
}

/*************************************************************************//**
 * @brief  begins dma driver transfer.
 *
 * @param	*Buffer: Read or Write buffer
 * @param	Dir: Data direction. DMA_DIR_PeripheralToMemory, DMA_DIR_MemoryToPeripheral, DMA_DIR_MemoryToMemory
 *
 * @return  None.
 *
 ****************************************************************************/
void dmaTransfer (uint32_t *Buffer, uint32_t Dir, uint32_t BufferSize, tDmaStream dmaStream)
{
	DMA_HandleTypeDef* hdma = &DMA_Handler[dmaStream];

	uint32_t periphBaseAddress = (uint32_t) dmaStreamMap[dmaStream].PeripheralBaseAddr;

	__HAL_DMA_DISABLE(hdma);
	__HAL_DMA_CLEAR_FLAG(hdma, DMA_FLAG_GL3 | DMA_FLAG_TE3 | DMA_FLAG_HT3 | DMA_FLAG_TC3);


	HAL_DMA_DeInit(hdma);

	hdma->Init.Request = dmaStreamMap[dmaStream].Request;
	hdma->Init.Direction = dmaStreamMap[dmaStream].DIR;
	hdma->Init.PeriphInc =  dmaStreamMap[dmaStream].PeripheralInc;
	hdma->Init.MemInc = dmaStreamMap[dmaStream].MemoryInc;
	hdma->Init.PeriphDataAlignment = dmaStreamMap[dmaStream].PeripheralDataSize;
	hdma->Init.MemDataAlignment = dmaStreamMap[dmaStream].MemoryDataSize;
	hdma->Init.Priority = dmaStreamMap[dmaStream].Priority;
	hdma->Init.Mode = DMA_NORMAL;

	HAL_DMA_Init(hdma);

	interruptsSetState(IRQ_DMA2_CH3, IRQ_ENABLE);

	HAL_DMA_Start_IT(hdma, periphBaseAddress, (uint32_t) Buffer, BufferSize);

}

/*****************************************************************************
 * @brief  check the DMA flag state.
 *
 * @param	dmaStream: DMAx_Steamy to be checked
 *
 * @return  eError: RET_OK if flag is TRUE
 * 					RET_FAIL if flag is FALSE
 *
 ****************************************************************************/
eError dmaState(tDmaStream dmaStream, uint32_t flagToCheck)
{
	eError result = RET_FAIL;
	DMA_HandleTypeDef* hdma = &DMA_Handler[dmaStream];
	if (DMA2->ISR & flagToCheck)
	{
		__HAL_DMA_CLEAR_FLAG(hdma, flagToCheck);
		result = RET_OK;
	}
	return result;
}

/****************************************************************************
 *    End of file
 ***************************************************************************/
