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

#define EEPROM_REVERSE_CACHE

#define EEPROM_HEADER           3
#define EEPROM_LENGTH_ID   	    0
#define EEPROM_CRC_ID			1
#define EEPROM_MAP_ID			2

#define EEPROM_SIZE_LENGTH      2
#define EEPROM_SIZE_CRC         1
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
    uint16_t	sizeOfInstance;
}tVirtualEEPROMAreaMap;

const typedef struct sVirtualEEPROMAreaInstMap
{
    uint8_t	numOfInstances;
    uint8_t sizeOfInstance;
}tVirtualEEPROMAreaInstMap;


typedef enum eVirtualEEPROMAreaType {
    EEPROM_AREA_NO_INST,
    EEPROM_AREA_INST,
} tVirtualEEPROMAreaType;


typedef struct sVirtualEEPROMInstanceMap
{
    uint16_t offsetArea;
    tVirtualEEPROMAreaType typeArea;
    union{
    	const tVirtualEEPROMAreaMap* noInst;
    	const tVirtualEEPROMAreaInstMap* withInst;
    }regTable;
}tVirtualEEPROMInstanceMap;


typedef struct sVirtualEEPROMContext
{
	uint16_t 	virtualOffset;
	uint16_t		size;
}tVirtualEEPROMContext;


const typedef struct sVirtualEEPROMareaContext
{
	uint8_t				  areaSize;
	tVirtualEEPROMContext *areaEEPROM;
}tVirtualEEPROMareaContext;

typedef struct sVirtualEEPROMCache
{
	uint16_t virtualAddress;
	uint16_t value;
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
eError VirtualEEPROMMarkPageAs(tFlash flashArea, uint16_t Value);


#endif /* _EEPROM_H_ */

 /****************************************************************************
 *    End of file
 ****************************************************************************/
