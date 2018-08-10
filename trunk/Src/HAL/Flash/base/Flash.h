/*****************************************************************************
 * @file 	Flash.h
 *
 * @brief 	Flash functionality Header for stm32F4.
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
#define FLASH_SECTOR_16KB						0
#define FLASH_SECTOR_64KB						1
#define FLASH_SECTOR_128KB						2


#if defined(STM32F407xx) || defined(STM32F417xx) || defined(STM32F405xx)
#define FLASH_NUM_OF_SECTORS_16				4
#define FLASH_NUM_OF_SECTORS_64				1
#define FLASH_MAX_SECTORS					12
#endif

#if defined(STM32F401xB) || defined(STM32F401xC)
#define FLASH_NUM_OF_SECTORS_16				4
#define FLASH_NUM_OF_SECTORS_64				1
#define FLASH_MAX_SECTORS					6
#endif

#if defined(STM32F401xD) || defined(STM32F401xE) || defined(STM32F411xE)
#define FLASH_NUM_OF_SECTORS_16				4
#define FLASH_NUM_OF_SECTORS_64				1
#define FLASH_MAX_SECTORS					8
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
	uint8_t		sectors;				/**< @brief number of sectors */
} tFlashInstanceMap;

/**
 * @typedef
 * @brief Definition of a single Flash attributes
 */
typedef struct sFlashContext
{
	uint8_t		initSector;		    /**< @brief initial sector */
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
eError FlashReadData(tFlash flashArea, uint16_t *data, uint32_t offset);
eError FlashProgramData( tFlash flashArea, uint16_t data, uint32_t offset);
eError FlashErase( tFlash flashArea, uint8_t page);
uint32_t FlashGetStartAddress( tFlash flashArea);
eError FlashActualInstance( tFlash *areaFlash );

void FlashGetSize( tFlash flashArea, uint32_t *size);
void FlashGetPages( tFlash flashArea, uint8_t *pages);

#endif /* FLASH_H_ */

/****************************************************************************
 *    End of file
 ***************************************************************************/

