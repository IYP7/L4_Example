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
#include "stm32l4xx_ll_crc.h"
#include "stm32l4xx_ll_bus.h"
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

/* Page state header */
#define EE_PAGESTAT_ERASED      (uint64_t)0xFFFFFFFFFFFFFFFFU  /*!< State saved in 1st,2nd,3rd,4th data type of page header */
#define EE_PAGESTAT_RECEIVE     (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 1st data type of page header */
#define EE_PAGESTAT_ACTIVE      (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 2nd data type of page header */
#define EE_PAGESTAT_VALID       (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 3rd data type of page header */
#define EE_PAGESTAT_ERASING     (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 4th data type of page header */
/* Description of the 8 Bytes (64 bits) element in flash   */
/* Bit:  63                  32  31      16  15         0  */
/*       <--- Data Value ----->  <-unused->  <-VirtAddr->  */
#define EE_ELEMENT_SIZE         8U                            /*!< Size of element in Bytes */
#define EE_ELEMENT_TYPE         uint64_t                      /*!< Type of element */
#define EE_VIRTUALADDRESS_TYPE  uint16_t                      /*!< Type of Virtual Address */
#define EE_VIRTUALADDRESS_SHIFT 0U                            /*!< Bits Shifting to get Virtual Address in element */
#define EE_CRC_TYPE             uint16_t                      /*!< Type of Crc */
#define EE_CRC_SHIFT            16U                           /*!< Bits Shifting to get Crc in element */
#define EE_DATA_TYPE            uint32_t                      /*!< Type of Data */
#define EE_DATA_SHIFT           32U                           /*!< Bits Shifting to get Data value in element */
#define EE_MASK_VIRTUALADDRESS  (uint64_t)0x000000000000FFFFU
#define EE_MASK_CRC             (uint64_t)0x00000000FFFF0000U
#define EE_MASK_DATA            (uint64_t)0xFFFFFFFF00000000U
#define EE_MASK_FULL            (uint64_t)0xFFFFFFFFFFFFFFFFU

/* Macros to manipulate elements */
#define EE_VIRTUALADDRESS_VALUE(__ELEMENT__)            (EE_VIRTUALADDRESS_TYPE)((__ELEMENT__) & EE_MASK_VIRTUALADDRESS) /*!< Get virtual address value from element value */
#define EE_DATA_VALUE(__ELEMENT__)                      (EE_DATA_TYPE)(((__ELEMENT__) & EE_MASK_DATA) >> EE_DATA_SHIFT) /*!< Get Data value from element value */
#define EE_CRC_VALUE(__ELEMENT__)                       (EE_CRC_TYPE)(((__ELEMENT__) & EE_MASK_CRC) >> EE_CRC_SHIFT) /*!< Get Crc value from element value */
#define EE_ELEMENT_VALUE(__VIRTADDR__,__DATA__,__CRC__) (((EE_ELEMENT_TYPE)(__DATA__) << EE_DATA_SHIFT) | (__CRC__) << EE_CRC_SHIFT | (__VIRTADDR__)) /*!< Get element value from virtual addr, data and crc values */

/* Configuration of crc calculation for eeprom emulation in flash */
#define CRC_POLYNOMIAL_LENGTH   LL_CRC_POLYLENGTH_16B /* CRC polynomial length 16 bits */
#define CRC_POLYNOMIAL_VALUE    0x8005U /* Polynomial to use for CRC calculation */
/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

/**
  * @brief  EE State Type structure definition.
  */
/* Type of state requested :
       ERASED  --> page is erased
       RECEIVE --> page used during data transfer when no more space available in the system
       VALID   --> page contains valid data
       INVALID --> page invalid state */
typedef enum {
   STATE_PAGE_ERASED,
   STATE_PAGE_RECEIVE,
   STATE_PAGE_ACTIVE,
   STATE_PAGE_VALID,
   STATE_PAGE_ERASING
} EE_State_type;

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
