/*************************************************************************//**
 * @file tools.h
 *
 * @brief Support functions library header file.
 *
 * @author Rub�n Rodr�guez
 * @author Jose Antonio Garcia Peiro
 *
 * @date 4/6/2012
 *
 ****************************************************************************/

#ifndef _TOOLS_H_
#define _TOOLS_H_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"
#include "shell.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "stdarg.h"
#ifdef DIAGNOSTICS_AVAILABLE
 #include "Diagnostics.h"
#endif

/****************************************************************************
 *  DEFINES
 ***************************************************************************/
#ifdef	__DEBUG__
#define		MY_ASSERT( cond )		((cond)?0:myAssertFail(__FILE__, __FUNCTION__, __LINE__, #cond ))
#define     DBG( cond )             printf("DBG: %s,%d,\t\t\t%s\n", __FILE__, __LINE__, #cond ); cond
#else
#define	MY_ASSERT(cond)
#define DBG( cond ) cond
#endif


#define CLAMP( x, low, high )	(((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define	MAX(a, b)  				(((a) > (b)) ? (a) : (b))
#define	MIN(a, b)  				(((a) < (b)) ? (a) : (b))

#define COUNTOF(a)              (sizeof(a)/sizeof(a[0]))

#define BufferSetData8(pf, i, v) 		(pf)[(i)] = (uint8_t)(v)
#define BufferSetData16(pf, i, v) 	\
    do { BufferSetData8(pf, i, (v)); BufferSetData8(pf, (i)+1, (v) >> 8); } while(0)
#define BufferSetData32(pf, i, v) 	\
    do { BufferSetData16(pf, i, (v)); BufferSetData16(pf, (i)+2, (v) >> 16); } while(0)


#ifdef DIAGNOSTICS_AVAILABLE
 #define DIAGNOSTICS_SET_EVENT(a, i, v) DiagnosticsSetEvent(a, i, v);
#else
 #define DIAGNOSTICS_SET_EVENT(a, i, v)
#endif


/****************************************************************************
 *  TYPES DEFINITIONS
 ***************************************************************************/
typedef struct
{
	uint8_t token;
	uint8_t size;
	uint8_t* buffer;
}tTokenDelimitedTable;

/****************************************************************************
 *  PRIVATE VARIABLES
 ***************************************************************************/


/****************************************************************************
 *  PUBLIC FUNCTIONS
 ***************************************************************************/
uint8_t dtoh( int32_t in, int8_t* out, uint8_t length);
eError htod(int8_t *buffer, void *result, uint8_t length);
void itos( int32_t value, uint8_t type, int8_t* buffer );
void _atoi(const void *buf, uint32_t *result, uint8_t *length);
uint8_t memCompare(const void *buf1, const void *buf2, uint8_t count);
void myAssertFail(const char *sFile, const char *sFunction, uint16_t line,const char *sCond );


void my_printf(tBool enable,uint8_t level,const char *fmt, ...); // custom printf() function
void my_printTrace(uint8_t channel,const char *fmt, ...); // custom printf() function;
void my_printTraceBin(uint8_t channel,uint8_t *buffer,uint8_t size);

#endif /* _TOOLS_H_ */

/****************************************************************************
 *    End of file
 ****************************************************************************/
