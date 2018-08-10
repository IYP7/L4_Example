/*************************************************************************//**
 * @file uart.h
 * @addtogroup HAL
 * @{
 * @addtogroup UART
 * @{
 *
 * @brief HAL uart Header.
 *
 * @author Jordi Castells Sala
 *
 * @date 20/01/2016
 *
 ****************************************************************************/

#ifndef __UART_H__
#define __UART_H__

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
#include "UART_map.h"

/****************************************************************************
 *  DEFINITIONS
 ****************************************************************************/

#define MAX_UART_BUFFER_SIZE    256

//Define Default behaviour if not already defined in uart_map.h or compile time
#if defined(MON_UART_POLL)
    #define MON_UART_TX_POLL
    #define MON_UART_RX_POLL
#elif defined(MON_UART_IT)
    #define MON_UART_TX_IT
    #define MON_UART_RX_IT
#elif defined(MON_UART_DMA)
    #define MON_UART_TX_DMA
    #define MON_UART_RX_DMA
#else
    #if !defined(MON_UART_TX_POLL) && !defined(MON_UART_TX_IT) && !defined(MON_UART_TX_DMA)
        #define MON_UART_TX_IT
    #endif
    #if !defined(MON_UART_RX_POLL) && !defined(MON_UART_RX_IT) && !defined(MON_UART_RX_DMA)
        #define MON_UART_RX_IT
    #endif
#endif

/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

/**
 * Generic callback for a defined UART. It expects an uint8_t parameter
 * and returns eError.
 */
typedef eError (*uartRxCallback)(uint8_t);
typedef eError (*uartTxCallback)(uint8_t);

/**
 * @typedef
 * @brief Definition of a single UART port
 */
typedef struct sUartInstanceMap
{
    USART_TypeDef*  port;           /**< @brief ST HAL UART port structure */
    uint32_t        baudRate;       /**< @brief speed of the UART port */
    uint8_t         dataSize;       /**< @brief Word Length of the sent data */
    uint8_t         parity;         /**< @brief Activate or deactivate Parity */
    uint8_t         stopBits;       /**< @brief Activate or deactivate stop bits*/

    //Read/Write Buffers
    uint16_t        txBufferSize;   /**< @brief Size of the tx buffer */
    uint8_t*        txBufferPtr;    /**< @brief Pointer of the tx buffer */
    uint16_t        rxBufferSize;   /**< @brief Size of the tx buffer */
    uint8_t*        rxBufferPtr;    /**< @brief Pointer of the tx buffer */

    //DmaChannel      dmaChn; //PENDING TO BE DEFINED
    uint32_t*       dmaSrc;         /**< @brief Not implemented */
    uint8_t         dmaSrcLen;      /**< @brief Not implemented */
    uint8_t         dmaIrq;         /**< @brief Not implemented */

    //Callbacks
    uartRxCallback  rxCallback;     /**< @brief Callback for RX interruption */
    uartTxCallback  txCallback;     /**< @brief Callback for TX interruption */
} tUartInstanceMap;


typedef struct sUartContext
{
	tCircularBuffer rxBuffer;
	tCircularBuffer txBuffer;
} tUartContext;

/****************************************************************************
 *    PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************/
eError uartInit(void);
eError uartStop(void);
eError uartStart(void);
eError uartSleep(void);
eError uartWake(void);

//HREG SIZE functions
eError uartDriverSetBufferSize(tUart uartPort, uint32_t bufferSize );
uint32_t uartDriverGetBufferSize(tUart uartPort);

//HREG DATA functions
eError uartDriverWrite(tUart uartPort, uint8_t* buffer);
eError uartDriverRead(tUart uartPort, uint8_t* buffer);

//HREG STATUS functions
eError uartDriverStatus(tUart uartPort);

//Interrupt Handler
void uartDriverMainIRQHandler(tUart uartPort);

#endif // _UART_H_

/****************************************************************************
 *    End of file
 *    }@}@
 ***************************************************************************/
