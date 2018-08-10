/*************************************************************************//**
 * @file spi.c
 * @date 20/01/2016
 * @Author Jordi Castells Sala
 *
 * @addtogroup HAL
 * @{
 * @addtogroup SPI
 * @{
 *
 * @brief SPI driver control.
 *
 * @details spi driver control Can work in either polling mode, interruption mode or dma Mode
 * with proper defines on compilation time.
 *
 * The possible defines are:
 *
 *      MON_SPI_POLL [IMPLEMENTED]
 *      MON_SPI_IT   [IMPLEMENTED]
 *      MON_SPI_DMA  [NOT IMPLEMENTED]
 *
 *      MON_SPI_TX_POLL [NOT IMPLEMENTED]
 *      MON_SPI_TX_IT   [IMPLEMENTED]
 *      MON_SPI_TX_DMA  [NOT IMPLEMENTED]
 *
 *      MON_SPI_RX_POLL [NOT IMPLEMENTED]
 *      MON_SPI_RX_IT   [IMPLEMENTED] //Differences between MASTER and SLAVE
 *      MON_SPI_RX_DMA  [NOT IMPLEMENTED]
 *
 * Define all this either at compile time, or at the spi_map.h
 *
 * The API for this Driver is handled with Three HREGS.
 *  - BUFFER SIZE HREG (HREG_BS)
 *  - DATA HREG        (HREG_DATA)
 *  - STATUS HREG      (HREG_STATUS)
 *
 * The functionality depends on a Write or a read, and the work mode
 *
 * | HREG   | WR                           | RD                                     |
 * |--------|------------------------------|----------------------------------------|
 * | SIZE   | Amount of data to write/Read | Amount of data available on the buffer |
 * | DATA   | Buffer with data             | Buffer where to write the data         |
 * | STATUS | DO NOT WRITE STATUS          | STATUS of the Transmission             |
 *
 *
 * This driver won't handle the specific messages to send. Upper layers of the software
 * (Or Drivers) are responsible to send the correct data through the SPI driver.
 *
 * ### POLLING write "foobar" : (BLOCKING)
 *
 * ### POLLING READ 5 bytes : (NON BLOCKING)
 *
 * ### INTERRUPT write. (NON BLOCKING)
 *
 *      ********************  ********************  ********************
 *      *  START WRITING   *=>* POLL STATUS      *=>* WRITE FINISHED   *
 *      ********************  ********************  ********************
 *      uint8_t buffer[32];
 *
 *      switch(state){
 *         case 1:
 *             //Write Something from SPI1 (This is directly connected to SPI2)
 *             WRITE_HREG(MON_SPI_1, HREG_SPI_BUFFER_SIZE, 34);
 *             WRITE_HREG(MON_SPI_1, HREG_SPI_DATA, "SPI Mother*****, Do you speak it?\n");
 *             state++;
 *         break;
 *
 *         case 2:
 *             //Wait unti SPI1 finished transmitting / and SPI2 recieving
 *             READ_HREG(MON_SPI_1, HREG_SPI_STATE, &ret);
 *             if(ret == RET_OK){
 *                 state++; //Transfer finished
 *             }
 *       }
 *
 * ### INTERRUPT read amount of bytes: (NON BLOCKING)
 *
 *      ********************  ********************  ********************
 *      *  START READING   *=>* POLL STATUS      *=>* read  FINISHED   *
 *      ********************  ********************  ********************
 *      uint8_t buffer[32]; //This has to be accessible, not function scoped
 *
 *      switch(state){
 *         case 0:
 *             //We'll expect something in SPI2. So start running
 *             WRITE_HREG(MON_SPI_2, HREG_SPI_BUFFER_SIZE, 34);
 *             READ_HREG(MON_SPI_2, HREG_SPI_DATA, &buffer);
 *             state++;
 *         break;
 *
 *         case 2:
 *             //Wait unti SPI1 finished transmitting / and SPI2 recieving
 *             READ_HREG(MON_SPI_2, HREG_SPI_STATE, &ret2);
 *             if(ret2 == RET_OK){
 *                 state++; //Buffer filled with expected data
 *             }
 *         break;
 *      }
 *
 * ### INTERRUPT Slave READ:
 *
 * Slave read differs from Master read in that it does not expect a specific amount
 * of data. So the RX buffer is always ready to receive data, and what we poll is
 * the already read contents.
 *
 *      ********************  ********************
 *      * POLL bytes       *=>* READ  FINISHED   *
 *      ********************  ********************
 *
 *      READ_HREG(MON_SPI_2, HREG_SPI_BUFFER_SIZE, &size);
 *      if(size > EXPECTED_SIZE){
 *          WRITE_HREG(MON_SPI_2, HREG_SPI_BUFFER_SIZE, size);
 *          READ_HREG(MON_SPI_2,  HREG_SPI_DATA, &buffer);
 *      }
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
#include <string.h>

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/
#include "hreg_dat.h"

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "spi.h"

/****************************************************************************
 *  EXTERNAL VARIABLES
 ****************************************************************************/
extern const tSpiInstanceMap SPIInstanceMap[NUM_OF_SPI];
extern const tSpiPeripheralMap SPIPeripheralMap[NUM_OF_SPI_DEVICES];

/****************************************************************************
 *  PRIVATE MACROS
 ***************************************************************************/

/****************************************************************************
 *  PRIVATE VARIABLES
 ***************************************************************************/
SPI_HandleTypeDef ST_SpiHandlers[NUM_OF_SPI];   //ST HAL handlers
uint32_t spiBufferSize[NUM_OF_SPI];          //Buffer Size

//#if defined(MON_SPI_RX_IT) || defined(MON_SPI_TX_IT)
#define RX_HAL_BUFFER_SIZE 5
tSpiContext spiCBuffers[NUM_OF_SPI];
/**@todo Maybe using this "fake" buffer is not the best way to handle this situation*/
uint8_t halRxSlaveBuffer[NUM_OF_SPI][RX_HAL_BUFFER_SIZE];
//#endif


/****************************************************************************
 *    PRIVATE FUNCTIONS DECLARATIONS
 ****************************************************************************/
static void spiRestartSlaveMode(SPI_HandleTypeDef*);

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/

static void spiRestartSlaveMode(SPI_HandleTypeDef *hspi)
{
    uint8_t i;
#ifdef IS_SPI1
    if (hspi->Instance == SPI1){
        i = SPI_1;
    }
#endif
#ifdef IS_SPI2
    if (hspi->Instance == SPI2){
        i = SPI_2;
    }
#endif
#ifdef IS_SPI3
    if (hspi->Instance == SPI3){
        i = SPI_3;
    }
#endif

    if(hspi->Init.Mode == SPI_MODE_SLAVE) {
        HAL_SPI_Receive_IT(hspi, halRxSlaveBuffer[i], SPIInstanceMap[i].rxBufferSize);
    }
}

/**
 * When a transfer is over. If it's SLAVE mode. Start Listening again
 **/
void  HAL_SPI_TxCpltCallback (SPI_HandleTypeDef *hspi){
    spiRestartSlaveMode(hspi);
}

/**
 * When an error ocurred. If it's SLAVE mode. Start Listening again
 **/
void  HAL_SPI_ErrorCallback (SPI_HandleTypeDef *hspi){
    spiRestartSlaveMode(hspi);
}


/*****************************************************************************
  * @brief SPI MSP Transfer completed callback. In Slave mode will re-enable
  * interrupts and reset the ST handler as appropiate
  * @param hspi: SPI handle pointer
  * @retval None
 ****************************************************************************/
//void  HAL_SPI_RxCpltCallback (SPI_HandleTypeDef *hspi)
//{
//    return;
//    if(hspi->Init.Mode == SPI_MODE_SLAVE){
//        if(hspi->RxXferCount == 0){
//            hspi->State       = HAL_SPI_STATE_BUSY_RX;
//            hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
//            hspi->pRxBuffPtr  = hspi->pRxBuffPtr - RX_HAL_BUFFER_SIZE - 1; //&halRxSlaveBuffer[spiPort];
//            hspi->RxXferSize  = RX_HAL_BUFFER_SIZE;
//            hspi->RxXferCount = RX_HAL_BUFFER_SIZE;
//        }
//        __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));
//    }
//}


/*****************************************************************************
  * @brief SPI MSP Initialization CALLBACK
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - NVIC configuration for SPI interrupt request enable (SHOULD NOT BE DONE HERE)
  * @param hspi: SPI handle pointer
  * @retval None
 ****************************************************************************/
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
#if defined(IS_SPI1)
    if (hspi->Instance == SPI1){
        /* Enable SPI clock */
        __HAL_RCC_SPI1_CLK_ENABLE();
    }
#endif

#if defined(IS_SPI2)
    if (hspi->Instance == SPI2){
        /* Enable SPI clock */
        __HAL_RCC_SPI2_CLK_ENABLE();
    }
#endif

#if defined(IS_SPI3)
    if (hspi->Instance == SPI3){
        /* Enable SPI clock */
        __HAL_RCC_SPI3_CLK_ENABLE();
    }
#endif
}


/*************************************************************************//**
 * @brief SPI De Initialization ST HAL callback
 * @param hspi ST Handler
 ****************************************************************************/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
#if defined(IS_SPI1)
    if(hspi->Instance == SPI1){
        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
    }
#endif

#if defined(IS_SPI2)
    if(hspi->Instance == SPI2){
        __HAL_RCC_SPI2_FORCE_RESET();
        __HAL_RCC_SPI2_RELEASE_RESET();
    }
#endif

#if defined(IS_SPI3)
    if(hspi->Instance == SPI3){
        __HAL_RCC_SPI3_FORCE_RESET();
        __HAL_RCC_SPI3_RELEASE_RESET();
    }
#endif
}

/*************************************************************************//**
 * @brief Returns which kind of interrupt happened in a specific spi handler
 * @param hspi ST HAL spi handler
 * @return  SPI IRQ number identifier (one of SPI_IT_TXE, SPI_IT_RXNE, SPI_IT_ERR)
 ****************************************************************************/
static inline uint16_t interruptType(SPI_HandleTypeDef* hspi)
{
    uint32_t itsource = hspi->Instance->CR2;
    uint32_t itflag   = hspi->Instance->SR;

    if(((itflag & SPI_FLAG_OVR) == RESET) &&
            ((itflag & SPI_FLAG_RXNE) != RESET) &&
            ((itsource & SPI_IT_RXNE) != RESET)){
        return SPI_IT_RXNE;
    }
    else if(((itflag & SPI_FLAG_TXE) != RESET) &&
            ((itsource & SPI_IT_TXE) != RESET))
    {
        return SPI_IT_TXE;
    }

    return SPI_IT_ERR;
}

/*************************************************************************//**
 * @brief  Callback Handler for the IRQ interrupt.
 * @details On TX we'll use the default ST HAL functionality. Reading is different
 * since we want to callback on each byte, not just wait until everything is
 * finished. It also has differences between Master and Slave mode.
 *
 * For now what it does is:
 * 1) Call ST default handlers
 * 2) Post process the ST results and perform callbacks
 * 3) Prepare For Slave mode lecture in case the spiPort is a slave
 *
 * @param  spiPort Which port caused the interrupt
 ****************************************************************************/
void spiDriverMainIRQHandler(tSpiPort spiPort)
{
    SPI_HandleTypeDef* hspi; //ST HAL SPI
    const tSpiInstanceMap* mspi;
    uint16_t interrupt;
    uint8_t byte;

    hspi        = &ST_SpiHandlers[spiPort];
    mspi        = &SPIInstanceMap[spiPort];
    interrupt   = interruptType(hspi);

    HAL_SPI_IRQHandler(hspi);

    //If it was a read. Check what we read and put it in our buffer
    //+call any defined callback
    if(interrupt == SPI_IT_RXNE){
        //We'll retrieve whatever is already writtern in the buffer
        byte = *(hspi->pRxBuffPtr-1);

        //Process the BYTE
        if(mspi->rxCallback!= NULL){
            mspi->rxCallback(byte);
        }
        else{
            if(GetQueueFreeBytes(&spiCBuffers[spiPort].rxBuffer) > 0){
                AddQueueByte(&spiCBuffers[spiPort].rxBuffer, byte);
            }
            else{
                /** @todo No space available! Error msg baybe?*/
            }
        }

        /* In Slave mode will re-enable
        * interrupts and reset the ST handler as appropiate to continue
        * listening*/
        if(hspi->Init.Mode == SPI_MODE_SLAVE){
            hspi->State       = HAL_SPI_STATE_BUSY_RX;
            hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
            hspi->pRxBuffPtr  = halRxSlaveBuffer[spiPort];
            hspi->RxXferSize  = RX_HAL_BUFFER_SIZE;
            hspi->RxXferCount = RX_HAL_BUFFER_SIZE;

            if(hspi->RxXferCount == 0){
                __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));
            }
        }
    }
}

/*************************************************************************//**
 * @brief  Init the SPI Handler. Set up basic structures but won't modify
 *         any microcontroller register
 *
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
eError spiInit(void)
{
    eError result = RET_OK;
    SPI_HandleTypeDef* hspi;
    const tSpiInstanceMap* mspi;
    uint8_t i;

    for(i=0; i<NUM_OF_SPI; i++) {
        hspi                         = &ST_SpiHandlers[i];
        mspi                         = &SPIInstanceMap[i];
        hspi->Instance               = mspi->port;
        hspi->Init.BaudRatePrescaler = mspi->bratePreescale;
        hspi->Init.Direction         = mspi->direction;
        hspi->Init.CLKPhase          = mspi->clkPhase;
        hspi->Init.CLKPolarity       = mspi->clkPol;
        hspi->Init.CRCCalculation    = mspi->crcCal;
        hspi->Init.CRCPolynomial     = mspi->crcPol;
        hspi->Init.DataSize          = mspi->dataSize;
        hspi->Init.FirstBit          = mspi->firstBit;
        hspi->Init.NSS               = mspi->NSS;
        hspi->Init.TIMode            = mspi->TIMMode;
        hspi->Init.Mode              = mspi->mode;

#if defined(MON_SPI_TX_IT) || defined(MON_SPI_RX_IT)
		CreateQueue(&spiCBuffers[i].rxBuffer,
                SPIInstanceMap[i].rxBufferPtr,
                SPIInstanceMap[i].rxBufferSize);

		CreateQueue(&spiCBuffers[i].txBuffer,
                SPIInstanceMap[i].txBufferPtr,
                SPIInstanceMap[i].txBufferSize);
#endif
    }

    return result;
}

/*************************************************************************//**
 * @brief  ??????
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
eError spiStop(void)
{
    uint8_t i;
    for(i=0; i<NUM_OF_SPI; i++) {
        HAL_SPI_DeInit(&ST_SpiHandlers[i]);
    }
    return RET_OK;
}

/*************************************************************************//**
 * @brief  Star the SPI handler. Called after init, sets up the microcontroller
 *         to work with SPI
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
eError spiStart(void)
{
    uint8_t i;
    SPI_HandleTypeDef*  hspi;  //HAL spi

    for(i=0; i<NUM_OF_SPI; i++) {
        hspi = &ST_SpiHandlers[i];
        HAL_SPI_DeInit(hspi);
        HAL_SPI_Init(hspi);

        /* Configure Generic communication so we can use the HAL
        * IRQ functions to retrieve the data, but not use the HAL_RECEIVE_IT
        * functions since we'll set the RXNE interrupt. We are using here a
        * halRxSlaveBuffer to configure the ST HAL, but we'll be using our own buffers
        * */
        if(SPIInstanceMap[i].mode == SPI_MODE_SLAVE){
            HAL_SPI_Receive_IT(hspi, halRxSlaveBuffer[i], SPIInstanceMap[i].rxBufferSize);
        }
    }

    return RET_OK;
}

/*************************************************************************//**
 * @brief  ??????
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
eError spiSleep(void)
{
    return RET_OK;
}

/*************************************************************************//**
 * @brief  ??????
 * @return RET_OK or RET_FAIL
 ****************************************************************************/
eError spiWake(void)
{
    return RET_OK;
}


/*************************************************************************//**
 * @brief Returns current status of the port related to an SPI device.
 *
 * @param	spiDevice Spi device to get the status from its port
 * @return  Status of the driver, either busy or allright.
 ****************************************************************************/
eError spiDriverStatus(tSpi spiDevice)
{
    SPI_HandleTypeDef*  hspi;  //HAL spi
    tSpiPort spiPort;

    spiPort = SPIPeripheralMap[spiDevice].port;
    hspi = &ST_SpiHandlers[spiPort];

    switch (HAL_SPI_GetState(hspi)){
        case HAL_SPI_STATE_RESET:
            return RET_NOT_INITIALIZED;
        case HAL_SPI_STATE_READY:
            return RET_OK;
        case HAL_SPI_STATE_BUSY:
        case HAL_SPI_STATE_BUSY_TX:
        case HAL_SPI_STATE_BUSY_RX:
        case HAL_SPI_STATE_BUSY_TX_RX:
            return RET_BUSY;
        case HAL_SPI_STATE_ERROR:
            return RET_FAIL;
    }

    return RET_FAIL; //Default case, we don't know what happened
}

#if defined(MON_SPI_TX_IT)
/*************************************************************************//**
 * @brief Add buffer contents to the TX buffer. To be sent as soon as possible.
 *
 * @param	spiDevice Spi Device to write to
 * @param	buffer Buffer with data to send to the device
 * @param   size Amount of bytes to send to the device
 * @return  error state.
 ****************************************************************************/
static eError spiDriverWriteIT(tSpi spiDevice, uint8_t* buffer, uint32_t size)
{
    SPI_HandleTypeDef*  hspi;  //HAL spi
    eError              result;
    tSpiPort            spiPort;

    result   = RET_OK;
    spiPort = SPIPeripheralMap[spiDevice].port;
    hspi     = &ST_SpiHandlers[spiPort];

    //IF SPI in slave mode (continuous listening) we have to disable it
    //before sending, or there's going to be an error
    if(hspi->Init.Mode == SPI_MODE_SLAVE){
        hspi->State = HAL_SPI_STATE_READY;
        __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR)); //Do NOT READ DATA!
    }

    if(HAL_SPI_Transmit_IT(hspi, buffer, size) != HAL_OK){
        result = RET_BUSY;
    }

    return result;
}
#endif

#if defined(MON_SPI_TX_POLL)
/*************************************************************************//**
 * @brief Write a buffer to RX. Blocks the microprocessor.
 *
 * @param	spiDevice SpiDevice to write to
 * @param	buffer Buffer with data to write to the bus
 * @param   size Amount of bytes to send
 * @return  error state.
 ****************************************************************************/
static eError spiDriverWritePolling(tSpi spiDevice, uint8_t* buffer, uint32_t size)
{
	eError retValue = RET_OK;

    SPI_HandleTypeDef*  hspi;  //HAL spi
    tSpiPort            spiPort;

    spiPort = SPIPeripheralMap[spiDevice].port;
    hspi = &ST_SpiHandlers[spiPort];

    /**@todo Timeout of polling should be calculated*/
    HAL_SPI_Transmit(hspi, buffer, size, 5000);

    return retValue;
}
#endif

#if defined(MON_SPI_TX_DMA)
/*************************************************************************//**
 * @brief Write a buffer to RX
 * @todo pending to implement this function
 *
 * @param	spiDevice SpiDevice to write to
 * @param	buffer Buffer with data to write to the bus
 * @param   size Amount of bytes to send
 * @return  error state.
 ****************************************************************************/
static eError spiDriverWriteDMA(tSpi spiDevice, uint8_t* buffer, uint32_t size)
{
    return RET_FAIL;
}
#endif

/*************************************************************************//**
 * @brief  Sends the specified amount of bytes in HREG_SPI_BUFFER_SIZE from the
 * provided data pointer.
 *
 * @param	spiDevice SpiDevice to write to
 * @param	buffer Buffer with data to write to the bus
 * @return  error state.
 ****************************************************************************/
eError spiDriverWrite(tSpi spiDevice, uint8_t* buffer)
{
    eError result = RET_OK;
    uint32_t bufferSize;
    tSpiPort spiPort;

    spiPort = SPIPeripheralMap[spiDevice].port;
    bufferSize = spiBufferSize[spiPort];

    if(bufferSize == 0){
        return RET_INVALID_PARAMETER;
    }
    //Fill write buffer
    if(bufferSize > SPIInstanceMap[spiPort].txBufferSize){
        return RET_INVALID_PARAMETER;
    }
    else{
    	if ( SPIInstanceMap[spiPort].dataSize == SPI_DATASIZE_8BIT){
    		memcpy(SPIInstanceMap[spiPort].txBufferPtr, buffer, bufferSize);
    	}
    	else if(SPIInstanceMap[spiPort].dataSize == SPI_DATASIZE_16BIT)
    	{
    		memcpy(SPIInstanceMap[spiPort].txBufferPtr, buffer, bufferSize*2);
    	}
    	else
    	{
    		return RET_FAIL;
    	}


    }

#if defined(MON_SPI_TX_IT)
    result = spiDriverWriteIT(spiPort, SPIInstanceMap[spiPort].txBufferPtr, bufferSize);
#elif defined(MON_SPI_TX_DMA)
    result = spiDriverWriteDMA(spiPort, SPIInstanceMap[spiPort].txBufferPtr, bufferSize);
#elif defined(MON_SPI_TX_POLL)
    result = spiDriverWritePolling(spiPort, SPIInstanceMap[spiPort].txBufferPtr, bufferSize);
#endif

    return result;
}


#if defined(MON_SPI_RX_POLL)
/*************************************************************************//**
 * @brief Blocks the system to read rdSyze bytes to rdBuffer. UNTESTED
 *
 * @param  spiDevice Port where to read data from. Mainly, which port to read from
 * @param  rdBuffer Buffer where we want to get the data written to
 * @param  rdSize Amount of bytes to read
 * @return eError RET_OK if read successful, RET_FAIL otherwise
 ****************************************************************************/
static eError spiDriverReadPolling(tSpi spiDevice, uint8_t* rdBuffer, uint32_t rdSize)
{
    eError result = RET_OK;
    uint32_t timeout;
    SPI_HandleTypeDef* hspi;
    tSpiPort spiPort;

    spiPort = SPIPeripheralMap[spiDevice].port;
    hspi = &ST_SpiHandlers[spiPort];

    //timeout = (rdSize* 8) * SPIMap[spiPort].baudRate;
    timeout = 50000; /**@todo Timeout of polling should be calculated*/

    if(HAL_SPI_Receive(hspi, (uint8_t *)rdBuffer, rdSize, timeout) != HAL_OK){
        result = RET_FAIL;
    }

    return result;
}
#endif

#if defined(MON_SPI_RX_IT)
/*************************************************************************//**
 * @brief Read rdSize bytes from the received buffered data.
 * If not enough data is available, simply return RET_FAIL
 *
 * @param  spiDevice Port where to read data from. Mainly, which port to read from
 * @param  rdBuffer Buffer where we want to get the data written to
 * @param  rdSize rdSize Amount of bytes to read
 * @return eError RET_OK if read successful, RET_FAIL otherwise
 ****************************************************************************/
static eError spiDriverReadITSlaveMode(tSpi spiDevice, uint8_t* rdBuffer , uint32_t rdSize)
{
    eError result = RET_FAIL;
    uint32_t i;
    uint8_t byte;
    tSpiPort spiPort;

    spiPort = SPIPeripheralMap[spiDevice].port;

    if(GetQueuePendingBytes(&spiCBuffers[spiPort].rxBuffer) < rdSize ){
        result =  RET_FAIL;
    }else{
        for(i=0; i<rdSize; i++){
            byte = GetQueueByte(&spiCBuffers[spiPort].rxBuffer);
            rdBuffer[i] = byte;
        }
        result = RET_OK;
    }

    return result;
}

/*************************************************************************//**
 * @brief Start SPI IT recieving mode to receive rdSize bytes
 *
 * @param  spiDevice Port where to read data from. Mainly, which port to read from
 * @param  rdBuffer Buffer where we want to get the data written to
 * @param  rdSize Amount of bytes to read
 * @return eError RET_OK if read process started successfully, RET_FAIL otherwise
 ****************************************************************************/
static eError spiDriverReadITMasterMode(tSpi spiDevice, uint8_t* rdBuffer , uint32_t rdSize)
{
    eError result = RET_OK;
    SPI_HandleTypeDef* hspi;
    tSpiPort spiPort;

    spiPort = SPIPeripheralMap[spiDevice].port;
    hspi     = &ST_SpiHandlers[spiPort];

    if(HAL_SPI_Receive_IT(hspi, rdBuffer, rdSize) != HAL_OK){
        result = RET_FAIL;
    }

    return result;
}
#endif

#if defined(MON_SPI_RX_DMA)
/*************************************************************************//**
 * @brief Read rdSize bytes from the received data. If not enough data is available, simply
 * return RET_FAIL
 *
 * @todo Still not implemented
 * @param  spiDevice Port where to read data from. Mainly, which port to read from
 * @param  rdBuffer Buffer where we want to get the data written to
 * @param  rdSize Amount of bytes to read
 * @return eError RET_OK if read process started successfully, RET_FAIL otherwise
 ****************************************************************************/
static eError spiDriverReadDMA(tSpi spiDevice, uint8_t* rdBuffer, uint32_t rdSize )
{
    return RET_FAIL;
}
#endif

/*************************************************************************//**
 * @brief Reads bytes from the SPI to the buffer parameter.
 * Will switch to different Read modes depending on the Compilation time
 * definition.
 *
 * @param Buffer where we want to get the data written to
 * @param rdSize Amount of bytes to read
 * @return eError RET_OK if read successful, RET_FAIL otherwise
 ****************************************************************************/
eError spiDriverRead(tSpi spiDevice, uint8_t* buffer)
{
    uint32_t rdSize;
    tSpiPort spiPort;

    spiPort = SPIPeripheralMap[spiDevice].port;
    rdSize = spiBufferSize[spiPort];

    if(rdSize == 0){
        return RET_INVALID_PARAMETER;
    }

#if defined(MON_SPI_RX_IT)
    if(SPIInstanceMap[spiPort].mode == SPI_MODE_MASTER){
        return spiDriverReadITMasterMode(spiPort, buffer, rdSize);
    }
    else{
        return spiDriverReadITSlaveMode(spiPort, buffer, rdSize);
    }
#elif defined(MON_SPI_RX_DMA)
    return spiDriverReadDMA(spiPort, buffer, rdSize);
#elif defined(MON_SPI_RX_POLL)
    return spiDriverReadPolling(spiPort, buffer, rdSize );
#else
#endif

    return RET_FAIL;
}

/*************************************************************************//**
 * @brief  Set SPI buffer size to write/read
 *
 * @param	spiDevice Device to write/read to/from
 * @param	bufferSize buffer size.
 * @return  error state.
 ****************************************************************************/
eError spiDriverSetBufferSize(tSpi spiDevice, uint32_t bufferSize )
{
    eError result = RET_OK;
    tSpiPort spiPort;

    spiPort = SPIPeripheralMap[spiDevice].port;

    spiBufferSize[spiPort] = bufferSize;
    return result;
}


/*************************************************************************//**
 * @brief Returns the amount of bytes in the buffer, pending to be read.
 *
 * @param	spi port to use.
 * @param	buffer size
 * @return  error state.
 *
 * @todo    pending implementation
 ****************************************************************************/
uint32_t spiDriverGetBufferSize(tSpi spiDevice)
{

#if defined(MON_SPI_RX_IT) || defined(MON_SPI_RX_DMA)
    tSpiPort spiPort;

    spiPort = SPIPeripheralMap[spiDevice].port;

    return (GetQueuePendingBytes(&spiCBuffers[spiPort].rxBuffer));
#else
    return 0; //If it's Polling there are no buffers
#endif
}


/*************************************************************************//**
 * @brief   taking care of N extra pulse.
 *
 * @param	spi port to use.
 * @param	number of extra pulses
 * @return  error state.
 *
 * @todo    pending implementation
 ****************************************************************************/
eError spiDriverExtraPulse(tSpi spiDevice,uint8_t count)
{
    eError result = RET_OK;
    uint8_t  index;
    tSpiPort spiPort;
    SPI_HandleTypeDef* hspi; //ST HAL SPI

    spiPort = SPIPeripheralMap[spiDevice].port;
    hspi        = &ST_SpiHandlers[spiPort];

    for (index=0;index < count; index++)
    {
		hspi->Instance->CR1 |= SPI_PHASE_2EDGE;
		hspi->Instance->CR1 |= SPI_POLARITY_HIGH;
		hspi->Instance->CR1 &= ((uint16_t)0xFFFD);
		hspi->Instance->CR1 |= SPI_POLARITY_LOW;
    }

    return result;
}

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
