/***********************************************************************
 * @file	uart.h
 * @brief	Serial Uart map declaration
 * @author	Jordi Castells Sala
 * @date 20/01/2016
 **********************************************************************/

#ifndef	_UART_MAP_H_
#define	_UART_MAP_H_

/***********************************************************************
 * COMMON INCLUDES
 **********************************************************************/
#include "Common.h"
#include "config.h"

/***********************************************************************
 * COMMON ARCHITECTURE INCLUDES
 **********************************************************************/

/***********************************************************************
 * INCLUDES
 **********************************************************************/

/***********************************************************************
 * DEFINES
 **********************************************************************/
//Define here which driver implementation to use
//#define MON_UART_TX_IT
#define MON_UART_RX_IT
#define MON_UART_TX_POLL
//#define MON_UART_RX_POLL

#define UART_TX_BUFFER 100

#define IS_UART1
#define IS_UART2
//#define IS_UART3
//#define IS_UART4
//#define IS_UART5
//#define IS_UART6

#ifdef IS_UART1
#define UART1_TX_BUFFER          256
#endif
#ifdef IS_UART2
#define UART2_TX_BUFFER          256
#endif
#ifdef IS_UART3
#define UART3_TX_BUFFER          100
#endif
#ifdef IS_UART4
#define UART4_TX_BUFFER          100
#endif
#ifdef IS_UART5
#define UART5_TX_BUFFER          100
#endif
#ifdef IS_UART6
#define UART6_TX_BUFFER          100
#endif

#ifdef IS_UART1
#define UART1_RX_BUFFER          100
#endif
#ifdef IS_UART2
#define UART2_RX_BUFFER          100
#endif
#ifdef IS_UART3
#define UART3_RX_BUFFER          100
#endif
#ifdef IS_UART4
#define UART4_RX_BUFFER          100
#endif
#ifdef IS_UART5
#define UART5_RX_BUFFER          100
#endif
#ifdef IS_UART6
#define UART6_RX_BUFFER          100
#endif


/***********************************************************************
 * TYPE DEFINITIONS
 **********************************************************************/

typedef enum eUart
{
    UART_INIT = -1,
#ifdef IS_UART1
    UART_1,
#endif
#ifdef IS_UART2
    UART_2,
#endif
#ifdef IS_UART3
    UART_3,
#endif
#ifdef IS_UART4
    UART_4,
#endif
#ifdef IS_UART5
    UART_5,
#endif
#ifdef IS_UART6
    UART_6,
#endif
    NUM_OF_UART
} tUart;

/***********************************************************************
 * PUBLIC FUCNTION PROTOTYPES
 **********************************************************************/
void handleOREIRQ(UART_HandleTypeDef* huart);

#endif	/*_UART_MAP_H_*/

/***********************************************************************
 * End of file
 **********************************************************************/
