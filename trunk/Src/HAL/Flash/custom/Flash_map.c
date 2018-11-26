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
#define FLASH_PROGRAM_PAGES					(uint32_t) 32
#define FLASH_EEPROM_IMAGE_PAGES			1
#define FLASH_DATA_PAGES					5

#define FLASH_PROGRAM_SECTORS				(FLASH_PROGRAM_PAGES * (FLASH_PAGE_SIZE / FLASH_SECTOR_SIZE)) // 2560
#define FLASH_EEPROM_IMAGE_NUM_SECTORS		(FLASH_EEPROM_IMAGE_PAGES * (FLASH_PAGE_SIZE / FLASH_SECTOR_SIZE))	// EEPROM_IMAGE_NUM_PAGES * 2
#define FLASH_DATA_NUM_SECTORS				(FLASH_DATA_PAGES * (FLASH_PAGE_SIZE / FLASH_SECTOR_SIZE)) // 1280

//#if((FLASH_PROGRAM_PAGES + (FLASH_EEPROM_IMAGE_PAGES*2) + FLASH_DATA_PAGES) > FLASH_MAX_PAGES)
//#error "Exceeded Flash size"
//#endif


const tFlashInstanceMap FlashInstanceMap[NUM_OF_FLASH] =
{
		/*							{   number of sectors	   }	*/
		/* FLASH_PROGRAM	 */ 	{ FLASH_PROGRAM_SECTORS 			},
		/* FLASH_EEPROM_IMG1 */ 	{ FLASH_EEPROM_IMAGE_NUM_SECTORS 	},
		/* FLASH_EEPROM_IMG2 */ 	{ FLASH_EEPROM_IMAGE_NUM_SECTORS 	},
		/* FLASH_USER_DATA */ 		{ FLASH_DATA_NUM_SECTORS 	},
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
