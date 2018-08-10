/*****************************************************************************
 * @file interrupts.h
 * @addtogroup HAL
 * @{
 * @addtogroup Interrupts
 * @{
 *
 * @brief HAL interrupts Header.
 *
 * @author Dario Davide
 *
 * @date 25/01/2016
 *
 ****************************************************************************/

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_


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
#include "interrupts_map.h"

/****************************************************************************
 * DEFINES
 ****************************************************************************/

#define INT_PRIORITY_0      	0x00
#define INT_PRIORITY_1      	0x01
#define INT_PRIORITY_2      	0x02
#define INT_PRIORITY_3      	0x03

#define INT_SUBPRIORITY_0       0x00
#define INT_SUBPRIORITY_1       0x01
#define INT_SUBPRIORITY_2       0x02
#define INT_SUBPRIORITY_3       0x03

#define IS_SETTABLE_PRIORITY_IRQ(IRQ)                ((IRQ) >= SVCall_IRQn)


/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

typedef enum eIRQState
{
	IRQ_DISABLE=0,
	IRQ_ENABLE,

} tIRQState;


/****************************************************************************/
typedef struct sInterruptsInstanceMap
{
	IRQn_Type  IRQn;
	uint8_t   PreemptPriority;
	uint8_t   SubPriority;
	tIRQState  State;

}tInterruptsInstanceMap;

/****************************************************************************
 *    PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************/
eError interruptsInit( void );
eError interruptsStart( void );
eError interruptsStop( void );
eError interruptsSleep( void );
eError interruptsWake( void );

void interruptsDisable( void );
void interruptsEnable( void );


//HREG ENABLE functions
tIRQState  interruptsGetState(tInterrupts indexIRQ);
eError 	   interruptsSetState(tInterrupts indexIRQ,tIRQState state);

#endif	/* INTERRUPTS_H */

/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/
