/*************************************************************************//**
 * @file
 * @addtogroup HAL
 * @{
 * @addtogroup SPI
 * @{
 *
 * @brief SPI spi Header.
 *
 * @author Jordi Castells Sala
 *
 * @date 18/1/2012
 *
 ****************************************************************************/

#ifndef __MON_SPI_H__
#define __MON_SPI_H__

/***********************************************************************
 * INCLUDES
 **********************************************************************/
#include "spi_map.h"

/***********************************************************************
 * DEFINITIONS
 **********************************************************************/
#define MAX_SPI_BUFFER_SIZE    256

//Define Default behaviour if not already defined in spi_map.h or compile time
#if defined(MON_SPI_POLL)
    #define MON_SPI_TX_POLL
    #define MON_SPI_RX_POLL
#elif defined(MON_SPI_IT)
    #define MON_SPI_TX_IT
    #define MON_SPI_RX_IT
#elif defined(MON_SPI_DMA)
    #define MON_SPI_TX_DMA
    #define MON_SPI_RX_DMA
#else
    #if !defined(MON_SPI_TX_POLL) && !defined(MON_SPI_TX_IT) && !defined(MON_SPI_TX_DMA)
        #define MON_SPI_TX_IT
    #endif
    #if !defined(MON_SPI_RX_POLL) && !defined(MON_SPI_RX_IT) && !defined(MON_SPI_RX_DMA)
        #define MON_SPI_RX_IT
    #endif
#endif

/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/
typedef eError (*spiTxCallback)(uint8_t);
typedef eError (*spiRxCallback)(uint8_t);

typedef enum eSpiPort
{
    SPI_INIT = -1,
#ifdef IS_SPI1
    SPI_1,
#endif
#ifdef IS_SPI2
    SPI_2,
#endif
#ifdef IS_SPI3
    SPI_3,
#endif
    NUM_OF_SPI
} tSpiPort;

/**
 * @typedef Configuration struct for an SPI port
 **/
typedef struct sSpiPortMap
{
    SPI_TypeDef*    port;               /**< @brief ST HAL SPI port structure*/
    uint32_t        mode;               /**< @brief MASTER or SLAVE*/
    uint32_t        bratePreescale;     /**< @brief BaudRate prescaler*/
    uint32_t        direction;          /**< */
    uint32_t        clkPhase;           /**< */
    uint32_t        clkPol;             /**< */
    uint32_t        dataSize;           /**< */
    uint32_t        firstBit;           /**< Most/Least significant bit*/

    uint32_t        NSS;                /**< */
    uint32_t 		NSSPMode;			/**< */
    uint32_t        TIMMode;            /**< */

    uint32_t        crcCal;             /**< */
    uint32_t        crcPol;             /**< */
    uint32_t 		crcLength;			/**< */

    //Read/Write Buffers
    uint16_t        txBufferSize;       /**< Size of the Write buffer    */
    uint8_t*        txBufferPtr;        /**< Pointer of the Write buffer */
    uint16_t        rxBufferSize;       /**< Size of the Read buffer     */
    uint8_t*        rxBufferPtr;        /**< Pointer of the Read buffer  */

    //Callbacks
    spiTxCallback  rxCallback;  /**< Callback on each output Interrupt */
    spiRxCallback  txCallback;   /**< Callback on each output Interrupt */
} tSpiInstanceMap;

/**
 * @brief Structure to define a SPI device
 */
typedef struct sSpiPeripheralMap
{
    tSpiPort port;
    //eGpioPort gpioPort;
} tSpiPeripheralMap;

typedef struct sSpiContext
{
	tCircularBuffer rxBuffer;
	tCircularBuffer txBuffer;
} tSpiContext;


/****************************************************************************
 *    PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************/

eError spiInit(void);
eError spiStop(void);
eError spiStart(void);
eError spiSleep(void);
eError spiWake(void);


eError spiDriverSetBufferSize(tSpi, uint32_t);
uint32_t spiDriverGetBufferSize(tSpi);
eError spiDriverWrite(tSpi, uint8_t*);
eError spiDriverRead(tSpi, uint8_t*);
eError spiDriverStatus(tSpi);

eError spiDriverExtraPulse(tSpi spiDevice,uint8_t count);

//Interrupt Handler
void spiDriverMainIRQHandler(tSpiPort);

#endif

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
