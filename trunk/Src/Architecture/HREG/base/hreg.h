/*************************************************************************//**
 * @file hreg.h
 *
 * @brief This file is providing the interface of HREG access function.
 *
 * @author Rubén Rodríguez
 *
 * @date 4/1/2012
 *
 ****************************************************************************/

#ifndef _HREG_H_
#define _HREG_H_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDE
 ***************************************************************************/
//#include "hreg_dat.h"


/****************************************************************************
 *  DEFINES
 ****************************************************************************/
#define HREG_NULL 0
#define HREG_NOP  0xFF
#define UNIQUE_INSTANCE 0

enum {
	HREG_GROUP_INIT = 0x00,
	HREG_GROUP_STOPPED,
	HREG_GROUP_STARTED,
	HREG_GROUP_SLEPT,
	HREG_GROUP_DECLARED
};

typedef enum eHREG_GROUP_TYPE{
	HREG_GROUP_TYPE_HAL,
	HREG_GROUP_TYPE_EEPROM,
	HREG_GROUP_TYPE_DEVICE_DRIVER,

	NUM_OF_HREG_GROUP_TYPES,

} tHREG_GROUP_TYPE;

#define SIZE_OF_HREG(hreg)                  ( ((hreg)->Max > 0xff ) ? ( ((hreg)->Max > 0xffff ) ? 3 : 2 ) : 1 )
#define READ_HREG(instance, hreg, value)    ( readHreg(  (uint8_t)(instance), (hreg), (uint32_t*)(value) ) )
#define WRITE_HREG(instance, hreg, value)   ( writeHreg( (uint8_t)(instance), (hreg), (uint32_t)(value) ) )

/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

typedef struct _HREG_GROUP_INT_CALL
{ /* Access function table */
   eError ( *const Init )( void );
   eError ( *const Stop )( void );
   eError ( *const Start )( void );
   eError ( *const Sleep )( void );
   eError ( *const Wake )( void );
} HREG_GROUP_INT_CALL;

typedef struct _HREG_GROUP_STATE
{ /* Access function table */
   uint8_t hregGroupEnabled;
} HREG_GROUP_STATE;


/*---- HREG object body ----*/
typedef struct _HREG
{
    void* const 	data;

    const uint32_t 	Max;
    const uint32_t 	Min;
    const uint32_t 	Init;
    const uint8_t 	OnRead;
    const uint8_t 	OnWrite;
    const uint8_t 	GroupId;		/* Refresh Function ID to call*/
    const uint8_t 	NVMId;

} HREG;

typedef struct
{
	const HREG 				*TableHreg;
	const uint8_t 			TableSize;
    const uint8_t   		NumInstances;
    const tHREG_GROUP_TYPE 	GroupType;
} HREG_TABLE_INFO;

/*---- HREG object body ----*/
typedef struct _HREG_FUNC_READ_CALL
{ /* Access function table */
   eError ( *const Read )( uint8_t instance, const HREG *, void *value );
} HREG_FUNC_READ_CALL;

typedef struct _HREG_FUNC_WRITE_CALL
{ /* Access function table */
    eError ( *const Write )( uint8_t instance, const HREG *, uint32_t value );
} HREG_FUNC_WRITE_CALL;

typedef struct _HREG_FUNC
{   /* Access function table */
    uint32_t ( *const Read )( uint8_t instance, const HREG * );
    eError  ( *const Write )( uint8_t instance, const HREG *, uint32_t data );
} HREG_FUNC;

/****************************************************************************
 *    PUBLIC VARIABLE
 ****************************************************************************/

/*---------- HREG Table Read Function ---------*/
uint32_t readHregTableStd( uint8_t instance, const HREG *hregPtr );

/*---------- HREG Table Write Function --------*/
eError writeHregTableStd( uint8_t instance, const HREG *hregPtr, uint32_t data );

/****************************************************************************
 *  PUBLIC FUCNTION PROTOTYPE
 ****************************************************************************/
/*------------- HREG Read Function ------------*/
eError readHreg( uint8_t instance, const HREG *hregP, void *value );
eError readShellHreg( uint8_t instance, uint16_t hregIndex, uint32_t *hregValue );

/*------------- HREG Write Function -----------*/
eError writeHreg( uint8_t instance, const HREG *hregP, uint32_t value );
eError writeShellHreg( uint8_t instance, uint16_t hregIndex , uint32_t hregValue);

/*------------- HREG access function ----------*/
void initHregAll( void );
uint8_t getNumOfHregs( void );
void stopHregGroupAll( void );
void startHregGroupAll(void );
void wakeHregGroupAll( void );
void sleepHregGroupAll( void );
void startHregGroup( uint8_t hregGroup );
void stopHregGroup( uint8_t hregGroup );
void sleepHregGroup( uint8_t hregGroup );
void wakeHregGroup( uint8_t hregGroup );

#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
eError getHregIndexFromPointer( const HREG *hregPtr, uint16_t *hregIndex );
#endif // MON_SHELL_COMMAND_VERBOSE_REGS

#endif /* _HREG_H_ */

/****************************************************************************
 *    End of file
 ***************************************************************************/
