/*************************************************************************//**
 * @file circularBuffer.h
 *
 * @brief
 *
 * @author Marc Bernabé
 *
 * @date 20/03/2012
 *
 ****************************************************************************/

#ifndef _CIRCULARBUFFER_H
#define _CIRCULARBUFFER_H

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/

typedef struct sCircularBuffer
{
    uint16_t numOfBytes;
    uint16_t readPtr;
    uint16_t writePtr;
    uint16_t bufferLen;
    uint8_t *bufferPtr;
}tCircularBuffer;


void CreateQueue(tCircularBuffer* buffer, uint8_t* bufferPtr, uint16_t size);
void ResetQueue(tCircularBuffer* buffer);
void ClearQueue(tCircularBuffer* buffer);
void AddQueueByte(tCircularBuffer* buffer, uint8_t data);
uint8_t GetQueueByte(tCircularBuffer* buffer);
uint16_t GetQueuePendingBytes(tCircularBuffer* buffer);
uint16_t GetQueueFreeBytes(tCircularBuffer* buffer);

void IncreaseQueueWriteIndex(tCircularBuffer* buffer, uint16_t index);
void DecreaseQueueWriteIndex(tCircularBuffer* buffer, uint16_t index);
void UpdateQueueWriteIndex(tCircularBuffer* buffer, uint16_t index, bool inc_dec);
//void IncreaseQueueReadIndex(tCircularBuffer* buffer, uint16_t index);
//void DecreaseQueueReadIndex(tCircularBuffer* buffer, uint16_t index);
void UpdateQueueReadIndex(tCircularBuffer* buffer, uint16_t index, bool inc_dec);


#endif // _CIRCULARBUFFER_H

/****************************************************************************
 *    End of file
 ***************************************************************************/