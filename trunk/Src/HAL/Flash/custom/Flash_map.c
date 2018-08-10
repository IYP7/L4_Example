/*****************************************************************************
 * @file 	Flash_map.c
 *
 * @brief 	Flash map map declaration. To be defined for every project's needs
 *
 * @details Define here the Flash characteristics. Note that this has to
 * 			be correctly defined in conjunction with GPIO_map.
 * @note	Once Flash_Channelx is enabled, the corresponding GPIO (PA4 or PA5) pin is
 * 			automatically connected to the analog converter output (Flash_OUTx).
 * 			In order to avoid parasitic consumption, the GPIO pin should first
 * 			be configured to analog (AIN).
 *
 * @author 	FV4MMA
 *
 * @date 	26/02/2016
 *
 ****************************************************************************/

#ifndef	_FLASH_MAP_C_
#define	_FLASH_MAP_C_

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "Common.h"

/***********************************************************************
 * COMMON ARCHITECTURE INCLUDES
 **********************************************************************/

/***********************************************************************
 * INCLUDES
 **********************************************************************/
#include "Flash.h"
#include "Flash_map.h"

/***********************************************************************
 * DEFINES
 **********************************************************************/
#define FLASH_PROGRAM_SECTORS				6
#define FLASH_EEPROM_IMAGE_NUM_SECTORS		1	// EEPROM_IMAGE_NUM_PAGES * 2
#define FLASH_DATA_NUM_SECTORS				0

#if((FLASH_PROGRAM_SECTORS + (FLASH_EEPROM_IMAGE_NUM_SECTORS*2) + FLASH_DATA_NUM_SECTORS) > FLASH_MAX_SECTORS)
#error "Exceeded Flash size"
#endif


const tFlashInstanceMap FlashInstanceMap[NUM_OF_FLASH] =
{
		/*							{   number of sectors	   }	*/
		/* FLASH_PROGRAM	 */ 	{ FLASH_PROGRAM_SECTORS 			},
		/* FLASH_EEPROM_IMG1 */ 	{ FLASH_EEPROM_IMAGE_NUM_SECTORS 	},
		/* FLASH_EEPROM_IMG2 */ 	{ FLASH_EEPROM_IMAGE_NUM_SECTORS 	},
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
 * PUBLIC FUCNTION PROTOTYPES
 **********************************************************************/

/***********************************************************************
 * PUBLIC FUCNTIONS
 **********************************************************************/

/***********************************************************************
 * PRIVATE FUCNTIONS
 **********************************************************************/

#endif	/*_FLASH_MAP_C_*/

/***********************************************************************
 * End of file
 **********************************************************************/
