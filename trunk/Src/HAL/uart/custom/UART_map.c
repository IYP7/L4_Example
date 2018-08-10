/***********************************************************************
 * @file UART_map.c
 * @date 20/01/2016
 * @Author Jordi Castells Sala
 * '
 * @addtogroup HAL
 * @{
 * @addtogroup UART_MAP
 * @{

 * @brief UART map declaration. To be defined for every project's needs
 *
 * @details Define here the Uarts characteristics. Note that this has to
 * be correctly defined in conjunction with  GPIO_map so the correct GPIOS
 * are enabled for the UARTS
 *
 * For callbacks. Declare as externs and just point to that function.
 *
 *      extern eError testRxCallback(uint8_t c);
 *      extern eError testTxCallback(uint8_t c);
 *
 *
 **********************************************************************/

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "Common.h"

/***********************************************************************
 * COMMON ARCHITECTURE INCLUDES
 **********************************************************************/
#include "PipeDevice.h" //For the Pipe Helpers

/***********************************************************************
 * INCLUDES
 **********************************************************************/
#include "UART.h"
#include "UART_map.h"

extern eError testRxCallback(uint8_t c);
extern eError testTxCallback(uint8_t c);

/***********************************************************************
 * DEFINES
 **********************************************************************/
#ifdef IS_UART1
    uint8_t uart1TxBuffer[UART1_TX_BUFFER];
    uint8_t uart1RxBuffer[UART1_RX_BUFFER];
#endif
#ifdef IS_UART2
    uint8_t uart2TxBuffer[UART2_TX_BUFFER];
    uint8_t uart2RxBuffer[UART2_RX_BUFFER];
#endif
#ifdef IS_UART3
    uint8_t uart3TxBuffer[UART3_TX_BUFFER];
    uint8_t uart3RxBuffer[UART3_RX_BUFFER];
#endif
#ifdef IS_UART4
    uint8_t uart4TxBuffer[UART4_TX_BUFFER];
    uint8_t uart4RxBuffer[UART4_RX_BUFFER];
#endif
#ifdef IS_UART5
    uint8_t uart5TxBuffer[UART5_TX_BUFFER];
    uint8_t uart5RxBuffer[UART5_RX_BUFFER];
#endif
#ifdef IS_UART6
    uint8_t uart6TxBuffer[UART6_TX_BUFFER];
    uint8_t uart6RxBuffer[UART6_RX_BUFFER];
#endif

const tUartInstanceMap UARTInstanceMap[NUM_OF_UART] =  {
/*
    {
        port                USART_TypeDef
        baudRate            uint32_t
        dataSize            uint8_t
        parity              uint8_t
        stopBits            uint8_t
        txBufferSize        uint16_t
        txBufferPtr         uint8_t*
        rxBufferSize        uint16_t
        rxBufferPtr         uint8_t*
        dmaChn              //DmaChannel
        dmaSrc              uint32_t*
        dmaSrcLen           uint8_t
        dmaIrq              uint8_t
        rxCallback          uartRxCallback
        txCallback          uartTxCallback
    }
*/
#ifdef IS_UART1
    {
        USART1,
        9600,
        UART_WORDLENGTH_8B,
        UART_PARITY_NONE,
        UART_STOPBITS_1,
        UART1_TX_BUFFER,
        uart1TxBuffer,
        UART1_RX_BUFFER,
        uart1RxBuffer,
        0, //NO DMA
        0, //NO DMA
        0, //NO DMA
        &pipeDevice1ByteReceived, //&testRxCallback, //example RX callback
        NULL, //&testTxCallback, //example TX callback
    },
#endif
#ifdef IS_UART2
    {
        USART2,
        9600,
        UART_WORDLENGTH_8B,
        UART_PARITY_NONE,
        UART_STOPBITS_1,
        UART2_TX_BUFFER,
        uart2TxBuffer,
        UART2_RX_BUFFER,
        uart2RxBuffer,
        0, //NO DMA
        0, //NO DMA
        0, //NO DMA
        &pipeDevice1ByteReceived, //No RX callback
        NULL, //No TX callback
    },
#endif
#ifdef IS_UART3
    {
        UART3,
        9600,
        UART_WORDLENGTH_8B,
        UART_PARITY_NONE,
        UART_STOPBITS_1,
        UART3_TX_BUFFER,
        uart3TxBuffer,
        UART3_RX_BUFFER,
        uart3RxBuffer,
        0, //NO DMA
        0, //NO DMA
        0, //NO DMA
        NULL, //No RX callback
        NULL, //No TX callback
    },
#endif
#ifdef IS_UART4
    {
        UART4,
        9600,
        UART_WORDLENGTH_8B,
        UART_PARITY_NONE,
        UART_STOPBITS_1,
        UART4_TX_BUFFER,
        uart4TxBuffer,
        UART4_RX_BUFFER,
        uart4RxBuffer,
        0, //NO DMA
        0, //NO DMA
        0, //NO DMA
        NULL, //No RX callback
        NULL, //No TX callback
    },
#endif
#ifdef IS_UART5
    {
        UART5,
        9600,
        UART_WORDLENGTH_8B,
        UART_PARITY_NONE,
        UART_STOPBITS_1,
        UART5_TX_BUFFER,
        uart5TxBuffer,
        UART5_RX_BUFFER,
        uart5RxBuffer,
        0, //NO DMA
        0, //NO DMA
        0, //NO DMA
        NULL, //No RX callback
        NULL, //No TX callback
    },
#endif
#ifdef IS_UART6
    {
        UART6,
        9600,
        UART_WORDLENGTH_8B,
        UART_PARITY_NONE,
        UART_STOPBITS_1,
        UART6_TX_BUFFER,
        uart6TxBuffer,
        UART6_RX_BUFFER,
        uart6RxBuffer,
        0, //NO DMA
        0, //NO DMA
        0, //NO DMA
        NULL, //No RX callback
        NULL, //No TX callback
    },
#endif
};



/****************************************************************************
 *    End of file
 *    }@}@
 ***************************************************************************/
