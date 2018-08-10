/************************************************************************//**
 * @file CircularBuffer.c
 *
 * @brief
 *
 * @author Marc Bernabé
 *
 * @date 10/12/2013
 *
 ****************************************************************************/

#ifndef _CIRCULARBUFFER_C_
#define _CIRCULARBUFFER_C_

/*****************************************************************************
 *  COMMON INCLUDE
 ****************************************************************************/
#include "common.h"

/*****************************************************************************
 *  ARCHITECTURE INCLUDES
 ****************************************************************************/

/*****************************************************************************
 *  INCLUDES
 ****************************************************************************/
#include "CircularBuffer.h"
#include "interrupts.h"

/*************************************************************************//**
 * @brief
 *
 * @param
 *
 * @return  None.
 *
 ****************************************************************************/
void CreateQueue(tCircularBuffer* buffer, uint8_t* bufferPtr, uint16_t size)
{
	buffer->bufferPtr = bufferPtr;
	buffer->bufferLen = size;
	buffer->readPtr = 0;
	buffer->writePtr = 0;
    buffer->numOfBytes = 0;
}

/*************************************************************************//**
 * @brief  Reset queue
 *
 * @param   Circular buffer
 *
 * @return  None.
 *
 ****************************************************************************/
void ResetQueue(tCircularBuffer* buffer)
{
	buffer->readPtr = 0;
	buffer->writePtr = 0;
    buffer->numOfBytes = 0;
}

/*************************************************************************//**
 * @brief
 *
 * @param
 *
 * @return  None.
 *
 ****************************************************************************/
void ClearQueue(tCircularBuffer* buffer)
{
	buffer->readPtr = buffer->writePtr;
	buffer->numOfBytes = 0;
}
/*************************************************************************//**
 * @brief   Add new byte into queue
 *
 * @param   Circular buffer
 *
 * @return  None.
 *
 ****************************************************************************/
void AddQueueByte(tCircularBuffer* buffer, uint8_t data)
{
	interruptsDisable();
    buffer->bufferPtr[buffer->writePtr]= data;
    buffer->numOfBytes++;
    buffer->writePtr++;
    buffer->writePtr %= buffer->bufferLen;
	interruptsEnable();
}

/*************************************************************************//**
 * @brief  Get next byte into queue
 *
 * @param   Circular buffer
 *
 * @return  Next byte into queue
 *
 ****************************************************************************/
uint8_t GetQueueByte(tCircularBuffer* buffer)
{
    uint8_t result;
	interruptsDisable();
    result = buffer->bufferPtr[buffer->readPtr];
    buffer->numOfBytes--;
    buffer->readPtr++;
    buffer->readPtr %= buffer->bufferLen;
    interruptsEnable();
    return result;
}

/*************************************************************************//**
 * @brief  Return number of pending bytes to send in queue
 *
 * @param   Circular buffer
 *
 * @return  Pending bytes into queue
 *
 ****************************************************************************/
uint16_t GetQueuePendingBytes(tCircularBuffer* buffer)
{
    return buffer->numOfBytes;
}

/*************************************************************************//**
 * @brief  Return number of free spaces in queue
 *
 * @param   Circular buffer
 *
 * @return  Free spaces in queue
 *
 ****************************************************************************/
uint16_t GetQueueFreeBytes(tCircularBuffer* buffer)
{
    return (buffer->bufferLen-buffer->numOfBytes);
}

void IncreaseQueueWriteIndex(tCircularBuffer* buffer, uint16_t index)
{
	interruptsDisable();
	UpdateQueueWriteIndex(buffer, (buffer->writePtr + index) % buffer->bufferLen, true);
	interruptsEnable();
}

void DecreaseQueueWriteIndex(tCircularBuffer* buffer, uint16_t index)
{
	interruptsDisable();
	UpdateQueueWriteIndex(buffer, (buffer->bufferLen + buffer->writePtr - index) % buffer->bufferLen, false);
	interruptsEnable();
}

void UpdateQueueWriteIndex(tCircularBuffer* buffer, uint16_t index, bool inc_dec)
{
	uint16_t temp;
	interruptsDisable();
	temp = buffer->writePtr;
	buffer->writePtr = index;
	if(inc_dec==true)	
	{
		temp = ((buffer->bufferLen + index) - temp) % buffer->bufferLen;
		buffer->numOfBytes = (buffer->numOfBytes + temp) % buffer->bufferLen;
	}
	else
	{
		temp = ((buffer->bufferLen + temp) - index) % buffer->bufferLen;
		buffer->numOfBytes = (buffer->bufferLen + buffer->numOfBytes - temp) % buffer->bufferLen;
	}
	interruptsEnable();
}

//void IncreaseQueueReadIndex(tCircularBuffer* buffer, uint16_t index)
//{
//	interruptsDisable();
//	UpdateQueueReadIndex(buffer, (buffer->readPtr + index) % buffer->bufferLen, true);
//	interruptsEnable();
//}
//
//void DecreaseQueueReadIndex(tCircularBuffer* buffer, uint16_t index)
//{
//	interruptsDisable();
//	UpdateQueueReadIndex(buffer, (buffer->readPtr - index) % buffer->bufferLen, false);
//	interruptsEnable();
//}

void UpdateQueueReadIndex(tCircularBuffer* buffer, uint16_t index, bool inc_dec)
{
	uint16_t temp;
	interruptsDisable();
	temp = buffer->readPtr;
	buffer->readPtr = index;
	if(inc_dec==true)
	{
		temp = ((buffer->bufferLen + index) - temp) % buffer->bufferLen;
		buffer->numOfBytes = (buffer->bufferLen + buffer->numOfBytes - temp) % buffer->bufferLen;
	}
	else
	{
		temp = ((buffer->bufferLen + temp) - index) % buffer->bufferLen;
		buffer->numOfBytes = (buffer->numOfBytes + temp) % buffer->bufferLen;
	}
	if(buffer->numOfBytes > buffer->bufferLen)
	{
		buffer->numOfBytes = buffer->bufferLen;
	}
	interruptsEnable();
}

#endif // _CIRCULARBUFFER_C_

/****************************************************************************
 *    End of file
 ***************************************************************************/
