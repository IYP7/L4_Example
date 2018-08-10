/****************************************************************************
 * @file 	VirtualEEPROM.h
 *
 * @brief 	VirtualEEPROM functionality Header.
 *
 * @author 	Albert Serrallo
 *
 * @date 	20/04/2016
 *
 ****************************************************************************/

#ifndef _VIRTUALEEPROM_H_
#define _VIRTUALEEPROM_H_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"
#include "sreg.h"
#include "sreg_dat.h"

/****************************************************************************
 *  INCLUDES
 ****************************************************************************/
#include "VirtualEEPROM_map.h"
#include "Flash.h"

/****************************************************************************
 *    DEFINES
 ****************************************************************************/
#define PAGE_STATUS		(uint16_t)0x00
#define PAGE_STATUS2	(uint16_t)0x02

#define EEPROM_SIZE_MAP			1
#define UNIQUE_EEPROM_INSTANCE 	1

//#define SIZE_OF_VIRTUAL_EEPROM_CACHE 80
#define EEPROM_REVERSE_CACHE
/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

typedef struct sVirtualEEPROMRegister
{
	uint32_t 	ptrAddrOffset;   /**< @brief offset pointer to write next register */
	uint32_t	maxOffset;       /**< @brief max offset Page for actual Page */
	tFlash 	    actualPage;     /**< @brief  actual flash area for VirtualEEPROM */
}tVirtualEEPROMRegister;


const typedef struct sVirtualEEPROMDeviceMap
{
	tFlash 	flashArea1;		/**< @brief flash area for VirtEEPROM image 1 */
	tFlash	flashArea2;	    /**< @brief flash area of VirtEEPROM image 2 */
} tVirtualEEPROMDeviceMap;


const typedef struct SVirtualEEPROMAreaMap
{
    uint8_t	numOfInstances;
    uint8_t	sizeOfInstance;
}tVirtualEEPROMAreaMap;


const typedef struct sVirtualEEPROMInstanceMap
{
    uint16_t                offsetArea;
    tVirtualEEPROMAreaMap*	regTable;
}tVirtualEEPROMInstanceMap;


typedef struct sVirtualEEPROMContext
{
	uint16_t 	virtualOffset;
	uint8_t		size;
}tVirtualEEPROMContext;


const typedef struct sVirtualEEPROMareaContext
{
	uint8_t				  areaSize;
	tVirtualEEPROMContext *areaEEPROM;
}tVirtualEEPROMareaContext;

typedef struct sVirtualEEPROMCache
{
	uint16_t virtualAddress;
	uint32_t value;
} tVirtualEEPROMCache;

/****************************************************************************
 *  PUBLIC FUCNTION PROTOTYPES
 ***************************************************************************/
eError VirtualEEPROMinit(void);
eError VirtualEEPROMStart( void );
eError VirtualEEPROMStop( void );
eError VirtualEEPROMSleep( void );
eError VirtualEEPROMWake( void );
eError VirtualEEPROMWriteRegister(tVirtualEEPROM eeprom, uint16_t reg, uint16_t instanceReg, uint32_t value);
eError VirtualEEPROMReadRegister(tVirtualEEPROM eeprom, uint16_t reg,  uint16_t instanceReg, uint32_t *value);



#endif /* _EEPROM_H_ */

 /****************************************************************************
 *    End of file
 ****************************************************************************/
