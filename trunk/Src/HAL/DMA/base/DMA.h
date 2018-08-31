/*************************************************************************//**
 * @file DMA.h
 *
 * @brief dma functionality Header.
 *
 * @author Gonzalo Serrano
 *
 * @date 27/10/2014
 *
 ****************************************************************************/

#ifndef DMA_H_
#define DMA_H_


/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"
#include "dma_map.h"



/****************************************************************************
 *  ARCHITECTURE INCLUDE
 ***************************************************************************/

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/

/****************************************************************************
 *  DEFINITIONS
 ****************************************************************************/



/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/


typedef enum eDmaStream
{
   DMA_INIT = -1,
#ifdef IS_DMA1_Stream0
    DMA1_S0,
#endif
#ifdef IS_DMA1_Stream1
    DMA1_S1,
#endif
#ifdef IS_DMA1_Stream2
    DMA1_S2,
#endif
#ifdef IS_DMA1_Stream3
    DMA1_S3,
#endif
#ifdef IS_DMA1_Stream4
    DMA1_S4,
#endif
#ifdef IS_DMA1_Stream5
    DMA1_S5,
#endif
#ifdef IS_DMA1_Stream6
    DMA1_S6,
#endif
#ifdef IS_DMA1_Stream7
    DMA1_S7,
#endif
#ifdef IS_DMA2_Stream0
    DMA2_S0,
#endif
#ifdef IS_DMA2_Stream1
    DMA2_S1,
#endif
#ifdef IS_DMA2_Stream2
    DMA2_S2,
#endif
#ifdef IS_DMA2_Stream3
    DMA2_S3,
#endif
#ifdef IS_DMA2_Stream4
    DMA2_S4,
#endif
#ifdef IS_DMA2_Stream5
    DMA2_S5,
#endif
#ifdef IS_DMA2_Stream6
    DMA2_S6,
#endif
#ifdef IS_DMA2_Stream7
    DMA2_S7,
#endif
   NUM_OF_DMA_STREAMS
} tDmaStream;

typedef struct sDmaStreamMap
{
	DMA_Channel_TypeDef*   DMAy_Streamx;
	uint32_t 	Request;
    uint32_t 	PeripheralBaseAddr;
    /* uint32_t 	Memory0BaseAddr; defined in application */
    uint32_t 	DIR;
    uint32_t 	BufferSize;
    uint32_t 	PeripheralInc;
    uint32_t 	MemoryInc;
    uint32_t 	PeripheralDataSize;
    uint32_t 	MemoryDataSize;
    uint32_t 	Mode;
    uint32_t 	Priority;
    uint8_t     Irq;
    tBool		dmaInitState;

} tDmaStreamMap;



/****************************************************************************
 *    PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************/
eError dmaInit(void);
eError dmaStop(void);
eError dmaStart(void);
eError dmaSleep(void);
eError dmaWake(void);

void SD_LowLevel_DMA_TxConfig (uint32_t *BufferSRC, uint32_t BufferSize);
eError dmaState(tDmaStream dmaStream, uint32_t flagToCheck);
void dmaTransfer (uint32_t *Buffer, uint32_t Dir, uint32_t BufferSize, tDmaStream dmaStream);
#endif // _DMA_H_

/****************************************************************************
 *    End of file
 ***************************************************************************/
