/*****************************************************************************
 * @file 	Flash.h
 *
 * @brief 	Flash functionality Header for stm32L4.
 *
 * @author 	Albert Serrallo
 *
 * @date 	26/09/2016
 *
 ****************************************************************************/

#ifndef _FLASH_H_
#define _FLASH_H_

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
#include "Flash_map.h"

/****************************************************************************
 * DEFINES
 ****************************************************************************/
#if defined(STM32L452xx)
//#define STM32L452_FLASH_SIZE				(0x7F800U)
#define FLASH_MAX_PAGES						(FLASH_SIZE / FLASH_PAGE_SIZE)
#define FLASH_MAX_SECTORS					(FLASH_MAX_PAGES * (FLASH_PAGE_SIZE / FLASH_SECTOR_SIZE))
#define FLASH_SECTOR_SIZE					((uint32_t) 8)
#endif

#define FLASH_ALL_PAGES						0xFF

/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

/**
 * @typedef
 * @brief Definition of a single Flash Map
 */
typedef struct sFLASHInstanceMap
{
	uint32_t		sectors;				/**< @brief number of pages */
} tFlashInstanceMap;

/**
 * @typedef
 * @brief Definition of a single Flash attributes
 */
typedef struct sFlashContext
{
	uint32_t	initSector;		    /**< @brief initial sector */
	uint32_t	initAddress;		/**< @brief initial flash address */
	uint32_t	maxOffset;			/**< @brief max. number of offsets */
}tFlashContext;

/****************************************************************************
 *    PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************/
eError FlashInit( void );
eError FlashStop( void );
eError FlashStart( void );
eError FlashSleep( void );
eError FlashWake( void );

eError FlashLock( void);
eError FlashUnlock( void);
eError FlashReadData(tFlash flashArea, uint64_t *data, uint32_t offset);
eError FlashProgramData( tFlash flashArea, uint64_t data, uint32_t offset);
eError FlashErase( tFlash flashArea, uint8_t page);
uint32_t FlashGetStartAddress( tFlash flashArea);
eError FlashActualInstance( tFlash *areaFlash );

void FlashGetSize( tFlash flashArea, uint32_t *size);
uint8_t FlashGetPages( tFlash flashArea );

#endif /* FLASH_H_ */

/****************************************************************************
 *    End of file
 ***************************************************************************/

