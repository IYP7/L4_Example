/****************************************************************************
 * @file 	EEPROMEmul.h
 *
 * @brief 	EEPROM emulation functionality Header.
 *
 * @author 	Gabriel Perez
 *
 * @date 	17/10/2018
 *
 ****************************************************************************/

#ifndef _VIRTUAL_EEPROM_H
#define _VIRTUAL_EEPROM_H

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  INCLUDES
 ****************************************************************************/
#include "Flash.h"

#include "../custom/VirtualEEPROM_map.h"

/*
#include "stm32l4xx_hal.h"

#include "stm32l4xx_ll_crc.h"
#include "stm32l4xx_ll_bus.h"

#if defined(RECOVERY_TEST)
#include "stm32l4xx_ll_rtc.h"
#endif
*/

/****************************************************************************
 *    DEFINES
 ****************************************************************************/
/* Masks of EE_Status return codes */
#define EE_STATUSMASK_ERROR   (uint16_t)0x00FFU /*!< Mask on EE_Status return code, selecting error codes */
#define EE_STATUSMASK_CLEANUP (uint16_t)0x0100U /*!< Mask on EE_Status return code, selecting cleanup request codes */


/* Page definitions */
#define PAGE_SIZE               FLASH_PAGE_SIZE                                  /*!< Page size */
#define PAGE_HEADER_SIZE        EE_ELEMENT_SIZE * 4U                             /*!< Page Header is 4 elements to save page state */
#define NB_MAX_ELEMENTS_BY_PAGE ((PAGE_SIZE - PAGE_HEADER_SIZE) / EE_ELEMENT_SIZE) /*!< Max number of elements by page */
#define PAGES_NUMBER            (((((NB_OF_VARIABLES + NB_MAX_ELEMENTS_BY_PAGE) / NB_MAX_ELEMENTS_BY_PAGE) * 2U) * CYCLES_NUMBER) + GUARD_PAGES_NUMBER)
                                                                                 /*!< Number of consecutives pages used by the application */
#define NB_MAX_WRITTEN_ELEMENTS ((NB_MAX_ELEMENTS_BY_PAGE * PAGES_NUMBER) / 2U)  /*!< Max number of elements written before triggering pages transfer */
#define START_PAGE              PAGE(START_PAGE_ADDRESS)                         /*!< Page index of the 1st page used for EEPROM emul, in the bank */
#define END_EEPROM_ADDRESS      (START_PAGE_ADDRESS + (PAGES_NUMBER * FLASH_PAGE_SIZE) - 1) /*!< Last address of EEPROM emulation flash pages */

/* No page define */
//#define EE_NO_PAGE_FOUND        ((uint32_t)0xFFFFFFFFU)


/* Macros to manipulate pages */
#define PAGE_ADDRESS(__PAGE__)   (uint32_t)(FLASH_BASE + (__PAGE__) * PAGE_SIZE + ((START_PAGE_ADDRESS - FLASH_BASE) / BANK_SIZE) * BANK_SIZE) /*!< Get page address from page index */
#define PAGE(__ADDRESS__)        (uint32_t)((((__ADDRESS__) - FLASH_BASE) % BANK_SIZE) / FLASH_PAGE_SIZE) /*!< Get page index from page address */
#define PREVIOUS_PAGE(__PAGE__)  (uint32_t)((((__PAGE__) - START_PAGE - 1U + PAGES_NUMBER) % PAGES_NUMBER) + START_PAGE) /*!< Get page index of previous page, among circular page list */
#define FOLLOWING_PAGE(__PAGE__) (uint32_t)((((__PAGE__) - START_PAGE + 1U) % PAGES_NUMBER) + START_PAGE) /*!< Get page index of following page, among circular page list */

/* Macros to manipulate elements */
#define EE_VIRTUALADDRESS_VALUE(__ELEMENT__)            (EE_VIRTUALADDRESS_TYPE)((__ELEMENT__) & EE_MASK_VIRTUALADDRESS) /*!< Get virtual address value from element value */
#define EE_DATA_VALUE(__ELEMENT__)                      (EE_DATA_TYPE)(((__ELEMENT__) & EE_MASK_DATA) >> EE_DATA_SHIFT) /*!< Get Data value from element value */
#define EE_CRC_VALUE(__ELEMENT__)                       (EE_CRC_TYPE)(((__ELEMENT__) & EE_MASK_CRC) >> EE_CRC_SHIFT) /*!< Get Crc value from element value */
#define EE_ELEMENT_VALUE(__VIRTADDR__,__DATA__,__CRC__) (((EE_ELEMENT_TYPE)(__DATA__) << EE_DATA_SHIFT) | (__CRC__) << EE_CRC_SHIFT | (__VIRTADDR__)) /*!< Get element value from virtual addr, data and crc values */

#define BANK_SIZE               FLASH_BANK_SIZE         /*!< Alias to FLASH_BANK_SIZE definition from HAL STM32L4 */
#define EE_ACCESS_32BITS    /*!< Enable EEPROM 32bits R/W functions, only valid for flash allowing 64bits access*/

/* Page state header */
#define EE_PAGESTAT_ERASED      (uint64_t)0xFFFFFFFFFFFFFFFFU  /*!< State saved in 1st,2nd,3rd,4th data type of page header */
#define EE_PAGESTAT_RECEIVE     (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 1st data type of page header */
#define EE_PAGESTAT_ACTIVE      (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 2nd data type of page header */
#define EE_PAGESTAT_VALID       (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 3rd data type of page header */
#define EE_PAGESTAT_ERASING     (uint64_t)0xAAAAAAAAAAAAAAAAU  /*!< State saved in 4th data type of page header */

#define EE_FLASH_PROGRAM(__ADDRESS__, __DATA__) HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (__ADDRESS__), (__DATA__))

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

/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/
/* Define of the return value */
typedef enum {
  /* External return codes : ok */
  EE_OK = 0U,

  /* External return codes : errors */
  EE_ERASE_ERROR,
  EE_WRITE_ERROR,
  EE_ERROR_NOACTIVE_PAGE,
  EE_ERROR_NOERASE_PAGE,
  EE_ERROR_NOERASING_PAGE,
  EE_ERROR_NOACTIVE_NORECEIVE_NOVALID_PAGE,
  EE_NO_DATA,
  EE_INVALID_VIRTUALADDRESS,
  EE_INVALID_PAGE,
  EE_INVALID_PAGE_SEQUENCE,
  EE_INVALID_ELEMENT,
  EE_TRANSFER_ERROR,
  EE_DELETE_ERROR,
  EE_INVALID_BANK_CFG,

  /* Internal return code */
  EE_NO_PAGE_FOUND,
  EE_PAGE_NOTERASED,
  EE_PAGE_ERASED,
  EE_PAGE_FULL,

  /* External return code : action required */
  EE_CLEANUP_REQUIRED = 0x100U,
} EE_Status;

/* Type of page erasing:
       EE_FORCED_ERASE     --> pages to erase are erased unconditionnally
       EE_CONDITONAL_ERASE --> pages to erase are erased only if not fully erased */
typedef enum {
   EE_FORCED_ERASE,
   EE_CONDITIONAL_ERASE
} EE_Erase_type;


/****************************************************************************
 *  PUBLIC FUCNTION PROTOTYPES
 ***************************************************************************/
eError EE_Format(EE_Erase_type EraseType);
eError EE_Init(uint16_t* VirtAddTab, EE_Erase_type EraseType);
#if defined(EE_ACCESS_32BITS)
eError EE_ReadVariable32bits(uint16_t VirtAddress, uint32_t* pData);
eError EE_WriteVariable32bits(uint16_t VirtAddress, uint32_t Data);
#endif
eError EE_ReadVariable16bits(uint16_t VirtAddress, uint16_t* pData);
eError EE_WriteVariable16bits(uint16_t VirtAddress, uint16_t Data);
eError EE_ReadVariable8bits(uint16_t VirtAddress, uint8_t* pData);
eError EE_WriteVariable8bits(uint16_t VirtAddress, uint8_t Data);
eError EE_ReadRegister(uint16_t VirtAddress, uint32_t* Data);
eError EE_WriteRegister(uint16_t VirtAddress, uint32_t Data);
eError EE_CleanUp(void);
eError EE_CleanUp_IT(void);
eError EE_DeleteCorruptedFlashAddress(uint32_t Address);
void EE_EndOfCleanup_UserCallback(void);

/**
  * @}
  */

/**
  * @}
  */

#endif /* __EEPROM_EMUL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
