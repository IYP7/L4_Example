/***********************************************************************
 * @file	uart.h
 * @brief	Serial Spi map declaration
 * @author	Albert Serrallo
 * @date	15/06/2017
 **********************************************************************/

#ifndef	_MON_SPI_MAP_H_
#define	_MON_SPI_MAP_H_

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "common.h"

/***********************************************************************
 * COMMON ARCHITECTURE INCLUDES
 **********************************************************************/

/***********************************************************************
 * INCLUDES
 **********************************************************************/

/***********************************************************************
 * DEFINES
 **********************************************************************/
//Define here which driver implementation to use
//#define MON_SPI_TX_IT
//#define MON_SPI_RX_IT
//#define MON_SPI_TX_POLL
//#define MON_SPI_RX_POLL

//#define IS_SPI1
//#define IS_SPI2
//#define IS_SPI3

#define SPI1_WR_BUFFER          100
//#define SPI2_WR_BUFFER          100
#define SPI3_WR_BUFFER            200

#define SPI1_RD_BUFFER          100
//#define SPI2_RD_BUFFER          100
#define SPI3_RD_BUFFER            100

/***********************************************************************
 * TYPE DEFINITIONS
 **********************************************************************/

/**
 * @brief Device list of all SPI devices connected to this micro
 * @details device specific information is defined (following this same order)
 * in spi_map.c
 */
typedef enum eSpi {
	SPI_TEST_1,
	SPI_TEST_3,
    NUM_OF_SPI_DEVICES,
} tSpi;



/***********************************************************************
 * PUBLIC FUCNTION PROTOTYPES
 **********************************************************************/

#endif	/*_MON_SPI_MAP_H_*/

/***********************************************************************
 * End of file
 **********************************************************************/
