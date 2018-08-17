/*************************************************************************//**
 * @file
 * @date 20/01/2016
 * @Author Jordi Castells Sala
 *
 * @todo This is implemented using CircularBuffer. Maybe makes more sense to implement using
 *       pipeDevice, or even internal buffers
 *
 * @addtogroup HAL
 * @{
 * @addtogroup UART
 * @{
 *
 * @brief uart driver control.
 *
 * @details UART driver control Can work in either polling mode, interruption mode or dma Mode
 * with proper defines on compilation time.
 *
 * The possible defines are:
 *
 *      MON_UART_POLL [IMPLEMENTED]
 *      MON_UART_IT   [IMPLEMENTED]
 *      MON_UART_DMA  [NOT IMPLEMENTED]
 *
 *      MON_UART_TX_POLL [IMPLEMENTED]
 *      MON_UART_TX_IT   [IMPLEMENTED]
 *      MON_UART_TX_DMA  [NOT IMPLEMENTED]
 *
 *      MON_UART_RX_POLL [NOT IMPLEMENTED]
 *      MON_UART_RX_IT   [IMPLEMENTED]
 *      MON_UART_RX_DMA  [NOT IMPLEMENTED]
 *
 * Define all this either at compile time, or at the uart_map.h
 *
 *
 * The API for this Driver is handled with Three HREGS.
 *  - BUFFER SIZE HREG (HREG_BS)
 *  - DATA HREG        (HREG_DATA)
 *  - STATUS HREG      (HREG_STATUS)
 *
 * The functionality depends on a Write or a read, and the work mode
 *
 * | HREG | WR                           | RD                                     |
 * |------|------------------------------|----------------------------------------|
 * | SIZE | Amount of data to write/Read | Amount of data available on the buffer |
 * | DATA | Buffer with data             | Buffer where to write the data         |
 * |STATUS| DO NOT WRITE UART STATE      | For uart the STATUS is always OK       |
 *
 * ### POLLING write "foobar" : (BLOCKING)
 *  - Write HREG_BS   -> strlen("foobar")
 *  - Write HREG_DATA -> "foobar"
 *
 * ### POLLING READ 5 bytes : (NON BLOCKING)
 *  - Write HREG_BS   -> 5 bytes  = I want to read 5 bytes
 *  - Read HREG_DATA  -> &buffer. = Cpy the read thing to this buffer plz
 *
 *  The Polling read will read as much bytes as possible from UART during
 *  the default timeout window. This window is the double time needed for
 *  reading those specified bytes at the specified speed. This is not expected
 *  to be used
 *
 *
 * ### INTERRUPT write "foobar" : (NON BLOCKING)
 *  - Write HREG_BS   -> strlen("foobar")
 *  - Write HREG_DATA -> "foobar"
 *
 *  This will be queued to the Tx buffer. And will be sent in IT mode
 *
 * ### INTERRUPT Read 5 bytes : (NON BLOCKING)
 *  - Write HREG_BS   -> 5 bytes  = I want to read 5 bytes
 *  - Read HREG_DATA  -> &buffer. = Cpy the read thing to this buffer plz
 *
 *  Read 5 bytes, will return all the already read bytes. Since it is interrupt
 *  Read, the RX buffer will be already filled with this data. If there's not
 *  enough Bytes it will simply return a RET_FAIL.
 *
 * ### DMA read
 *  Not implemented
 * ### DMA write
 *  Not implemented
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
#include "uart.h"

/****************************************************************************
 *  EXTERNAL VARIABLES
 ****************************************************************************/
extern const tUartInstanceMap UARTInstanceMap[NUM_OF_UART];

/****************************************************************************
 *  PRIVATE MACROS
 ***************************************************************************/

/****************************************************************************
 *  PRIVATE VARIABLES
 ***************************************************************************/

uint32_t uartBufferSize[NUM_OF_UART];          //Buffer Size
UART_HandleTypeDef ST_UartHandlers[NUM_OF_UART];   //ST HAL handlers

#if defined(MON_UART_RX_IT) || defined(MON_UART_TX_IT)
tUartContext uartCBuffers[NUM_OF_UART];
eError uartStatus[NUM_OF_UART];
static tUart uartPortIrq;
uint8_t byteRxBuffer;
#endif

/****************************************************************************
 *    PRIVATE FUNCTIONS DECLARATIONS
 ****************************************************************************/

#if defined(MON_UART_TX_IT)
static eError uartDriverWriteIT(tUart uartPort, uint8_t* buffer, uint32_t size);
#endif

#if defined(MON_UART_TX_POLL)
static eError uartDriverWritePolling(tUart uartPort, uint8_t* buffer, uint32_t size);
#endif

#if defined(MON_UART_TX_DMA)
static eError uartDriverWriteDMA(tUart uartPort, uint8_t* buffer, uint32_t size);
#endif


#if defined(MON_UART_RX_IT)
static eError uartDriverReadIT(tUart uartPort, uint8_t* rdBuffer , uint32_t rdSize);
#endif

#if defined(MON_UART_RX_POLL)
static eError uartDriverReadPolling(tUart uartPort, uint8_t* rdBuffer, uint32_t rdSize);
#endif

#if defined(MON_UART_RX_DMA)
static eError uartDriverReadDMA(tUart uartPort, uint8_t* rdBuffer, uint32_t rdSize);
#endif

/**
 * @brief HAL callback for UART initialization. Starts the clocks
 * @todo Also starts interrupts, but it's not supposed to do so
 * @param huart ST Huart handler
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1){
        __HAL_RCC_USART1_CLK_ENABLE();
    }
    else if (huart->Instance == USART2){
        __HAL_RCC_USART2_CLK_ENABLE();
    }
#ifdef IS_UART3
    else if (huart->Instance == USART3){
        __HAL_RCC_USART3_CLK_ENABLE();
    }
#endif
#ifdef IS_USART6
    else if (huart->Instance == USART6){
        __HAL_RCC_USART6_CLK_ENABLE();
    }
#endif
    else
    {
    	return;
    }

#if defined(MON_UART_RX_IT)
    /* Enable the UART Data Register not empty Interrupt. ALWAYS ENABLED */
    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
#endif
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1){
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();
    }
    else if (huart->Instance == USART2){
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();
    }
#ifdef IS_UART3
    else if (huart->Instance == USART3){
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();
    }
#endif
#ifdef IS_UART6
    else if (huart->Instance == USART6){
        __HAL_RCC_USART6_FORCE_RESET();
        __HAL_RCC_USART6_RELEASE_RESET();
    }
#endif
}


#if defined(MON_UART_RX_IT) || defined(MON_UART_TX_IT)

/*************************************************************************//**
  * @brief  Tx Half Transfer completed callbacks.
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
****************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t byte;
	if ( GetQueuePendingBytes(&uartCBuffers[uartPortIrq].txBuffer) != 0)
	{
		 byte = (uint8_t)(GetQueueByte(&uartCBuffers[uartPortIrq].txBuffer) & (uint8_t)0xFF);
		 HAL_UART_Transmit_IT(&ST_UartHandlers[uartPortIrq], &byte, 1);
	}
	else
	{
		 __HAL_UART_DISABLE_IT(&ST_UartHandlers[uartPortIrq], UART_IT_TXE);
	}
}


/*************************************************************************//**
 * @brief  Rx Transfer completed callback.
 * @param  huart: pointer to a UART_HandleTypeDef structure that contains
 *                the configuration information for the specified UART module.
 ****************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	const tUartInstanceMap* muart; //Montseny handler

	muart = &UARTInstanceMap[uartPortIrq];

	//IF callback defined use it. Otherwise, default behaviour
	if(muart->rxCallback != NULL){
		muart->rxCallback(byteRxBuffer);
	}
	else{
		if(GetQueueFreeBytes(&uartCBuffers[uartPortIrq].rxBuffer) > 0){
			AddQueueByte(&uartCBuffers[uartPortIrq].rxBuffer, byteRxBuffer);
		}
		else{
			uartStatus[uartPortIrq] = RET_BUFFER_FULL;
		}
	}

	 HAL_UART_Receive_IT(huart, &byteRxBuffer, 1 );

}



/*************************************************************************//**
 * @brief  Callback Handler for the UART interrupt.
 * @param  uartPort Which port caused the interrupt
 ****************************************************************************/
void uartDriverMainIRQHandler(tUart uartPort)
{
    UART_HandleTypeDef* huart; //HAL Uart

    huart = &ST_UartHandlers[uartPort];
    uartPortIrq = uartPort;
    HAL_UART_IRQHandler(huart);
}

#else
//Empty IRQ handler
void uartDriverMainIRQHandler(tUart uartPort){}
#endif



/*************************************************************************//**
 * @brief  Callback Handler for Errors in UART.
 * @param  huart Which port caused the interrupt
 ****************************************************************************/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}



/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/

/*************************************************************************//**
 * @brief  Init UART driver.
 * 		   Initializes all UART declared on UART_map list
 *
 * @return  None.
 ****************************************************************************/
eError uartInit(void)
{
    eError result = RET_OK;
    UART_HandleTypeDef* huart;  //ST handler
    const tUartInstanceMap* muart;  //Montseny handler
    uint8_t i;

    for(i=0; i<NUM_OF_UART; i++) {
        huart = &ST_UartHandlers[i];
        muart = &UARTInstanceMap[i];
        huart->Instance        = muart->port;
        huart->Init.BaudRate   = muart->baudRate;
        huart->Init.WordLength = muart->dataSize;
        huart->Init.StopBits   = muart->stopBits;
        huart->Init.Parity     = muart->parity;
        huart->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
        huart->Init.Mode       = UART_MODE_TX_RX;
        huart->Init.OverSampling = UART_OVERSAMPLING_16;

#if defined(MON_UART_TX_IT) || defined(MON_UART_RX_IT)
		CreateQueue(&uartCBuffers[i].rxBuffer,
                UARTInstanceMap[i].rxBufferPtr,
                UARTInstanceMap[i].rxBufferSize);

		CreateQueue(&uartCBuffers[i].txBuffer,
                UARTInstanceMap[i].txBufferPtr,
                UARTInstanceMap[i].txBufferSize);
#endif
    }




    return result;
}

/*************************************************************************//**
 * @brief  Stops UART driver.
 * @return  None.
 ****************************************************************************/
eError uartStop(void)
{
    eError result = RET_OK;
    uint8_t i;
    UART_HandleTypeDef* huart;

    for(i=0; i<NUM_OF_UART; i++){
        huart = &ST_UartHandlers[i];
        if(HAL_UART_DeInit(huart) != HAL_OK)
        {
            return RET_FAIL;
        }
    }

    return result;
}

/*************************************************************************//**
 * @brief  Starts UART driver.
 * @return  None.
 ****************************************************************************/
eError uartStart(void)
{
    eError result = RET_OK;
    uint8_t i;
    UART_HandleTypeDef* huart;

    for(i=0; i<NUM_OF_UART; i++){
        huart = &ST_UartHandlers[i];
        if(HAL_UART_DeInit(huart) != HAL_OK)
        {
            return RET_FAIL;
        }
        if(HAL_UART_Init(huart) != HAL_OK)
        {
            return RET_FAIL;
        }
#if defined(MON_UART_RX_IT) || defined(MON_UART_RX_DMA)
        HAL_UART_Receive_IT(huart, &byteRxBuffer, 1 );
#endif
    }

    return result;
}

/*************************************************************************//**
 * @brief  Sleeps UART driver.
 * @return  Sucess/Fail.
 ****************************************************************************/
eError uartSleep(void)
{
    eError result = RET_OK;

    return result;
}

/*************************************************************************//**
 * @brief  Wakes up Uart driver.
 * @return  Success/fail.
 ****************************************************************************/
eError uartWake(void)
{
    eError result = RET_OK;

    uartStart();

    return result;
}

/*************************************************************************//**
 * @brief  Set UART buffer size to write
 *
 * @param	uartPort port to use.
 * @param	bufferSize Size to write in following write command
 * @return  RET_OK.
 ****************************************************************************/
eError uartDriverSetBufferSize(tUart uartPort, uint32_t bufferSize )
{
    eError result = RET_OK;
    uartBufferSize[uartPort] = bufferSize;
    return result;
}

/*************************************************************************//**
 * @brief Returns the amount of bytes in the buffer, pending to be read.
 *
 * @param	uartPort port to use.
 * @return  Amount of bytes pending to be read in the circular buffer
 ****************************************************************************/
uint32_t uartDriverGetBufferSize(tUart uartPort)
{

#if defined(MON_UART_RX_IT) || defined(MON_UART_RX_DMA)
    return (GetQueuePendingBytes(&uartCBuffers[uartPort].rxBuffer));
#else
    return 0;
#endif
}


#if defined(MON_UART_TX_IT)
/*************************************************************************//**
 * @brief Add buffer contents to the TX buffer. To be sent as soon as possible.
 *
 * @param	Uart port to use.
 * @param	buffer to copy from
 * @param   Amount of bytes to copy
 * @return  error state.
 ****************************************************************************/
static eError uartDriverWriteIT(tUart uartPort, uint8_t* buffer, uint32_t size)
{
    eError result = RET_OK;
    uint32_t i;
    uint8_t byte = 0;

    if(GetQueueFreeBytes(&uartCBuffers[uartPort].txBuffer) < size){
        return RET_BUFFER_FULL;
    }

    for(i = 0; i < size; i++){
        AddQueueByte(&uartCBuffers[uartPort].txBuffer, buffer[i]);
    }

    if ( GetQueuePendingBytes(&uartCBuffers[uartPort].txBuffer) > 0)
    {
    	 /* send a empty buffer to enable Transmision and TXE */
    	 HAL_UART_Transmit_IT(&ST_UartHandlers[uartPort], &byte, 1);
    }


    return result;
}
#endif


#if defined(MON_UART_TX_POLL)
/*************************************************************************//**
 * @brief Write a buffer to RX. Blocks the microprocessor.
 *
 * @param	Uart port to use.
 * @param	buffer to copy from
 * @param   Amount of bytes to copy
 * @return  error state.
 ****************************************************************************/
static eError uartDriverWritePolling(tUart uartPort, uint8_t* buffer, uint32_t size)
{
    eError result = RET_OK;
    uint32_t bufferSize, timeout;
    UART_HandleTypeDef* huart;  //HAL uart

    bufferSize = uartBufferSize[uartPort];
    huart = &ST_UartHandlers[uartPort];

    timeout = (bufferSize * 8) * UARTInstanceMap[uartPort].baudRate;
    if (HAL_UART_Transmit(huart, buffer, bufferSize, timeout) != HAL_OK){
        result = RET_FAIL;
    }

    return result;
}
#endif

#if defined(MON_UART_TX_DMA)
/*************************************************************************//**
 * @brief Write a buffer to RX
 *
 * @param	Uart port to use.
 * @param	buffer to copy from
 * @param   Amount of bytes to copy
 * @return  error state.
 ****************************************************************************/
static eError uartDriverWriteDMA(tUart uartPort, uint8_t* buffer, uint32_t size)
{
    return RET_FAIL;
}
#endif

/*************************************************************************//**
 * @brief  Sends the specified amount of bytes in HREG_UART_BUFFER_SIZE from the
 * provided data pointer.
 *
 * @param	uartPort Uart port to use.
 * @param	buffer Buffer with data to send
 * @return  error state.
 ****************************************************************************/
eError uartDriverWrite(tUart uartPort, uint8_t* buffer)
{
    eError result = RET_OK;
    uint32_t bufferSize;

    bufferSize = uartBufferSize[uartPort];

    if(bufferSize == 0){
        return RET_INVALID_PARAMETER;
    }

#if defined(MON_UART_TX_IT)
    result = uartDriverWriteIT(uartPort, buffer, bufferSize);
#elif defined(MON_UART_TX_DMA)
    result = uartDriverWriteDMA(uartPort, buffer, bufferSize);
#elif defined(MON_UART_TX_POLL)
    result = uartDriverWritePolling(uartPort, buffer, bufferSize);
#endif

    return result;
}


#if defined(MON_UART_RX_POLL)
/*************************************************************************//**
 * @brief Blocks the system to read rdSyze bytes to rdBuffer. UNTESTED
 *
 * @param Buffer where we want to get the data written to
 * @param rdSize Amount of bytes to read
 * @return eError RET_OK if read successful, RET_FAIL otherwise
 ****************************************************************************/
static eError uartDriverReadPolling(tUart uartPort, uint8_t* rdBuffer, uint32_t rdSize)
{
    eError result = RET_OK;
    uint32_t timeout;
    UART_HandleTypeDef* huart;

    huart = &ST_UartHandlers[uartPort];

    timeout = (rdSize* 8) * UARTInstanceMap[uartPort].baudRate;

    if(HAL_UART_Receive(huart, (uint8_t *)rdBuffer, rdSize, timeout) != HAL_OK){
        result = RET_FAIL;
    }

    return result;
}
#endif

#if defined(MON_UART_RX_IT)
/*************************************************************************//**
 * @brief Read rdSize bytes from the received data. If not enough data is available, simply
 * return RET_FAIL
 *
 * @param Buffer where we want to get the data written to
 * @param rdSize Amount of bytes to read
 * @return eError RET_OK if read successful, RET_FAIL otherwise
 ****************************************************************************/
static eError uartDriverReadIT(tUart uartPort, uint8_t* rdBuffer , uint32_t rdSize)
{
    eError result = RET_FAIL;
    uint32_t i;
    uint8_t byte;

    if(GetQueuePendingBytes(&uartCBuffers[uartPort].rxBuffer) < rdSize ){
        result =  RET_FAIL;
    }else{
        for(i=0; i<rdSize; i++){
            byte = GetQueueByte(&uartCBuffers[uartPort].rxBuffer);
            rdBuffer[i] = byte;
        }
        result = RET_OK;
    }

    return result;
}
#endif

#if defined(MON_UART_RX_DMA)
/*************************************************************************//**
 * @brief Read rdSize bytes from the received data. If not enough data is available, simply
 * return RET_FAIL
 *
 * @todo Still not implemented
 * @param Buffer where we want to get the data written to
 * @param rdSize Amount of bytes to read
 * @return eError RET_OK if read successful, RET_FAIL otherwise
 ****************************************************************************/
static eError uartDriverReadDMA(tUart uartPort, uint8_t* rdBuffer, uint32_t rdSize )
{
    return RET_FAIL;
}
#endif

/*************************************************************************//**
 * @brief Reads bytes from the UART to the buffer parameter.
 * Will switch to different Read modes depending on the Compilation time
 * definition.
 *
 * @param uartPort UART port to read from
 * @param buffer Buffer where to write the data to
 * @return eError RET_OK if read successful, RET_FAIL otherwise
 ****************************************************************************/
eError uartDriverRead(tUart uartPort, uint8_t* buffer)
{
    eError result = RET_FAIL;
    uint32_t rdSize;

    rdSize = uartBufferSize[uartPort];

#if defined(MON_UART_RX_IT)
    return uartDriverReadIT(uartPort, buffer, rdSize);
#elif defined(MON_UART_RX_DMA)
    return uartDriverReadDMA(uartPort, buffer, rdSize);
#elif defined(MON_UART_RX_POLL)
    return uartDriverReadPolling(uartPort, buffer, rdSize );
#else
#endif

    return result;
}



/*************************************************************************//**
 * @brief Returns current status of the UART Driver. Either. Since UART works
 * by interrupt reading and writing the Buffer, for now, this is always OK.
 * Except when the recieving buffer is overrun.
 *
 * @param	uart port to use.
 * @return  Status of the driver, either busy or allright.
 ****************************************************************************/
eError uartDriverStatus(tUart uartPort)
{

#if defined(MON_UART_RX_IT) || defined(MON_UART_TX_IT)
    return uartStatus[uartPort];
#else
    return RET_OK;
#endif
}


/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
