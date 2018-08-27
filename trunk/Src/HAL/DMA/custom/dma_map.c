/***********************************************************************
 * @file	DMA_map.c
 * '
 * @brief	DMA map declaration
 *
 * @author Gonzalo Serrano
 *
 * @date 28/10/2014
 **********************************************************************/

#ifndef	_DMA_MAP_C_
#define	_DMA_MAP_C_

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "common.h"
/***********************************************************************
 * COMMON ARCHITECTURE INCLUDES
 **********************************************************************/
#include "hreg_dat.h"

/***********************************************************************
 * INCLUDES
 **********************************************************************/
#include "dma_map.h"
#include "DMA.h"
/***********************************************************************
 * DEFINES
 **********************************************************************/

/***********************************************************************
 * TYPE DEFINITIONS
 **********************************************************************/


const tDmaStreamMap dmaStreamMap[NUM_OF_DMA_STREAMS] =
{
#ifdef IS_DMA2_Stream3
    {
    	DMA2_Channel3,
    	DMA_REQUEST_4,
		0x40012C80,
      	DMA_PERIPH_TO_MEMORY,
		1,
		DMA_PINC_DISABLE,
		DMA_MINC_ENABLE,
		DMA_PDATAALIGN_WORD,
		DMA_MDATAALIGN_WORD,
		DMA_NORMAL,
		DMA_PRIORITY_VERY_HIGH,
		IRQ_DMA2_CH3,
		ENABLE,
    },
#endif
#ifdef IS_DMA2_Stream6
    {
    	DMA2_Channel6,
    	DMA_REQUEST_4,
      	0x40012C80,
      	DMA_MEMORY_TO_PERIPH,
		1,
		DMA_PINC_DISABLE,
		DMA_MINC_ENABLE,
		DMA_PDATAALIGN_WORD,
		DMA_MDATAALIGN_WORD,
		DMA_NORMAL,
		DMA_PRIORITY_VERY_HIGH,
		IRQ_DMA2_CH6,
		ENABLE,
    },
#endif
};



#endif	/*_DMA_MAP_C_*/
/***********************************************************************
 * End of file
 **********************************************************************/
