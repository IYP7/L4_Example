/*************************************************************************//**
 * @file sreg.h
 * 
 * @brief This file is providing the interface of SREG access function.
 * 
 * @author Rubén Rodríguez
 * 
 * @date 3/1/2012
 *
 ****************************************************************************/

#ifndef _SREG_H_
#define _SREG_H_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDE
 ***************************************************************************/
#include "sreg_dat.h"

/****************************************************************************
 * TYPE DEFINITIONS
 ****************************************************************************/
typedef enum eSregTypeNVM
 {
	SREG_FACTORY,
	SREG_CALIBRATION,
	SREG_USER,
	SREG_NO_STORAGE,
}tSregTypeNVM;

typedef enum eSregType
 {
	SREG_UINT8,
	SREG_UINT16,
	SREG_UINT32,
}tSregType;


typedef struct sSREGInstanceMap
{
	const tSregType	type;
	const uint8_t	indexSREGType;
    const uint8_t   Nvm;
    const uint8_t   TypeNvm;
}tSREGInstanceMap;

typedef struct sSREGParam
{
	uint32_t Data;
    uint32_t Max;
    uint32_t Min;
    uint32_t Init;
} tSREGParam;


typedef struct sSREG8InstanceMap
{
    const uint8_t Max;
	const uint8_t Min;
    const uint8_t Init;
} tSREG8InstanceMap;

typedef struct sSREG16InstanceMap
{
    const uint16_t Max;
	const uint16_t Min;
    const uint16_t Init;
} tSREG16InstanceMap;

typedef struct sSREG32InstanceMap
{
    const uint32_t Max;
	const uint32_t Min;
    const uint32_t Init;
} tSREG32InstanceMap;


typedef struct sTableSRegMapNVM
{
	uint16_t	eepromSregType;
	uint16_t	nvmSregMap;
}tTableSRegMapNVM;


/****************************************************************************
 * DEFINES
 ****************************************************************************/
#define SREG_NULL 0
#define SIZE_OF_SREG(sreg)                  ( ((sreg)->Max > 0xff ) ? 2 : 1 )
#define READ_SREG(sreg)                     ( readSreg( (sreg) ) )
#define WRITE_SREG(sreg,data)               ( writeSreg( (sreg), (data) ) )

#define SREG_WR_ERROR			1
#define SREG_WR_OK				0


/****************************************************************************
 *    PUBLIC FUCNTION PROTOTYPE
 ****************************************************************************/
/*------------- SREG Read Function ------------*/
uint32_t readSreg( SREG sreg );
uint32_t readShellSreg( uint16_t sregIndex );

eError sregReadParam(SREG sreg, tSREGParam *sregParam);

/*------------- SREG Write Function -----------*/
eError writeSreg( SREG sreg, uint32_t value );
eError writeShellSreg( uint16_t sregIndex , uint32_t sregValue);

/*------------- SREG access function ----------*/
void initSregAll( void );
uint8_t getNumOfSregs( void );

/*---------- SREG Table Read Function ---------*/
uint32_t readSregTableStd( SREG sreg );

/*---------- SREG Table Write Function --------*/
eError writeSregTableStd( SREG sreg, uint32_t value);


#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
eError getSregIndexFromPointer( const SREG *sregPtr, uint16_t *sregIndex );
#endif //MON_SHELL_COMMAND_VERBOSE_REGS

eError sregResetNVMValues(tSregTypeNVM typeNVM);

#endif /* _SREG_H_ */

/****************************************************************************
 *    End of file
 ***************************************************************************/
