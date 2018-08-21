/*************************************************************************//**
 * @file spi_map.c
 * @addtogroup HAL
 * @{
 * @addtogroup SPI
 * @{
 *
 * @brief SPI spi Mapping.
 *
 * @author Albert Serrallo
 *
 * @date 15/06/2017
 *
 ****************************************************************************/

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "common.h"
#include "spi.h"
#include "spi_map.h"

/***********************************************************************
 * COMMON ARCHITECTURE INCLUDES
 **********************************************************************/

/***********************************************************************
 * INCLUDES
 **********************************************************************/
#include "spi.h"
#include "spi_map.h"


extern eError testSpi2RxCallback(uint8_t c);
extern eError testSpi2TxCallback(uint8_t c);


/***********************************************************************
 * DEFINES
 **********************************************************************/
#ifdef IS_SPI1
    uint8_t spi1WrBuffer[SPI1_WR_BUFFER];
    uint8_t spi1RdBuffer[SPI1_RD_BUFFER];
#endif
#ifdef IS_SPI2
    uint8_t spi2WrBuffer[SPI2_WR_BUFFER];
    uint8_t spi2RdBuffer[SPI2_RD_BUFFER];
#endif
#ifdef IS_SPI3
    uint8_t spi3WrBuffer[SPI3_WR_BUFFER];
    uint8_t spi3RdBuffer[SPI3_RD_BUFFER];
#endif

const tSpiInstanceMap SPIInstanceMap[NUM_OF_SPI] =  {

#ifdef IS_SPI1
    {
        SPI1,                       //port
        SPI_MODE_SLAVE,            //mode
        SPI_BAUDRATEPRESCALER_128,  //brate
        SPI_DIRECTION_2LINES,       //direction
        SPI_PHASE_1EDGE,            //clkPhase
        SPI_POLARITY_LOW,           //clkPol
        SPI_DATASIZE_8BIT,          //dataSize
        SPI_FIRSTBIT_MSB,           //firstBit
        SPI_NSS_SOFT,               //NSS
		SPI_NSS_PULSE_DISABLE,
        SPI_TIMODE_DISABLE,         //TIMMode
        SPI_CRCCALCULATION_DISABLE, //crcCal
        7,                          //crcPol
        SPI_CRC_LENGTH_8BIT,        //crcLen
        SPI1_WR_BUFFER,             //WrBufferSize
        spi1WrBuffer,               //WdBufferPtr
		SPI1_RD_BUFFER,             //RdBufferSize
		spi1RdBuffer,               //RdBufferPtr
        NULL,                       //callback1
        NULL,                       //example TX callback
    },
#endif
#ifdef IS_SPI2
    {
        SPI2,
        SPI_MODE_MASTER,
        SPI_BAUDRATEPRESCALER_256,
        SPI_DIRECTION_2LINES,
        SPI_PHASE_1EDGE,
        SPI_POLARITY_LOW,
        SPI_DATASIZE_8BIT,
        SPI_FIRSTBIT_MSB,
        SPI_NSS_SOFT,
        SPI_TIMODE_DISABLE,
        SPI_CRCCALCULATION_DISABLE,
        7,

        SPI2_RD_BUFFER,
        spi2RdBuffer,
        SPI2_WR_BUFFER,
        spi2WrBuffer,

        NULL, //&testSpi2RxCallback, //example RX callback
        NULL, //example TX callback
    },
#endif
#ifdef IS_SPI3
    {
        SPI3,
        SPI_MODE_MASTER,
        SPI_BAUDRATEPRESCALER_128,
        SPI_DIRECTION_2LINES,
        SPI_PHASE_1EDGE,
        SPI_POLARITY_LOW,
        SPI_DATASIZE_8BIT,
        SPI_FIRSTBIT_MSB,
        SPI_NSS_SOFT,
		SPI_NSS_PULSE_DISABLE,
        SPI_TIMODE_DISABLE,
        SPI_CRCCALCULATION_DISABLE,
        7,
		SPI_CRC_LENGTH_8BIT,

        SPI3_WR_BUFFER,
        spi3WrBuffer,
        SPI3_RD_BUFFER,
        spi3RdBuffer,

        NULL,
        NULL,
    },
#endif
};

/**
 * @brief Map of devices connecting to a specific SPI
 * defined in spi_map.h
 */
const tSpiPeripheralMap SPIPeripheralMap[NUM_OF_SPI_DEVICES] =
{
	{SPI_1},	/* SPI_TEST_1 */
    {SPI_3},	/* SPI_TEST_3 */
};
/***********************************************************************
 * TYPE DEFINITIONS
 **********************************************************************/

/***********************************************************************
 * EXTERN VARIABLES
 **********************************************************************/

/***********************************************************************
 * PUBLIC VARIABLES
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUCNTION PROTOTYPES
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUCNTIONS
 **********************************************************************/

/****************************************************************************
 *    End of file
 *    }@}@
 ***************************************************************************/
