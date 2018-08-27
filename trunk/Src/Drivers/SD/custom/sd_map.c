/***********************************************************************
 * @file	SD_map.c
 * '
 * @brief	SD map declaration
 *
 * @author Alex Colomer
 *
 * @date 01/03/2018
 **********************************************************************/

#ifndef	_SD_MAP_C_
#define	_SD_MAP_C_

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "common.h"

/***********************************************************************
 * COMMON ARCHITECTURE INCLUDES
 **********************************************************************/
#include "sd_map.h"
#include "sd.h"
/***********************************************************************
 * INCLUDES
 **********************************************************************/

/***********************************************************************
 * DEFINES
 **********************************************************************/


const tSdioInit sdioInitMap[NUM_OF_SD]=
{
		{
			SDMMC_INIT_CLK_DIV,
			SDMMC_CLOCK_EDGE_RISING,
			SDMMC_CLOCK_BYPASS_DISABLE,
			SDMMC_CLOCK_POWER_SAVE_DISABLE,
			SDMMC_BUS_WIDE_1B,
			SDMMC_HARDWARE_FLOW_CONTROL_DISABLE,
		},

};


const tSdioInit sdioTransferMap[NUM_OF_BIT_TRANSFER_TYPES] = {

    /* Transfer Mode: SDIO_CK = 12MHz */
	/* SDIO BusWide: 1 bit */
    {
    	SDMMC_TRANSFER_CLK_DIV, 	/* 12MHz */
		SDMMC_CLOCK_EDGE_RISING,
		SDMMC_CLOCK_BYPASS_DISABLE,
		SDMMC_CLOCK_POWER_SAVE_DISABLE,
		SDMMC_BUS_WIDE_1B, /* 1 bit */
		SDMMC_HARDWARE_FLOW_CONTROL_DISABLE,
    },
    /* Transfer Mode: SDIO_CK = 12MHz */
    /* SDIO BusWide: 4 bits */
    {
		SDMMC_TRANSFER_CLK_DIV, 	/* 12MHz */
		SDMMC_CLOCK_EDGE_RISING,
		SDMMC_CLOCK_BYPASS_DISABLE,
		SDMMC_CLOCK_POWER_SAVE_DISABLE,
		SDMMC_BUS_WIDE_4B, /* 4 bits */
		SDMMC_HARDWARE_FLOW_CONTROL_DISABLE,
    },
    /* Transfer Mode: SDIO_CK = 12MHz */
    /* SDIO BusWide: 8 bits */
    {
		SDMMC_TRANSFER_CLK_DIV, 	/* 12MHz */
		SDMMC_CLOCK_EDGE_RISING,
		SDMMC_CLOCK_BYPASS_DISABLE,
		SDMMC_CLOCK_POWER_SAVE_DISABLE,
		SDMMC_BUS_WIDE_8B, /* 8 bits */
		SDMMC_HARDWARE_FLOW_CONTROL_DISABLE,
    },

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
 * PRIVATE VARIABLES
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUNCTIONS
 **********************************************************************/

/***********************************************************************
 * PRIVATE FUNCTIONS
 **********************************************************************/

#endif	/*_SD_MAP_C_*/

/***********************************************************************
 * End of file
 **********************************************************************/
