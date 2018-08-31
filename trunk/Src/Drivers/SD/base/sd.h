/*************************************************************************//**
 * @file sdio.h
 *
 * @brief sdio functionality Header.
 *
 * @author Gonzalo Serrano
 *
 * @date 27/10/2014
 *
 ****************************************************************************/

#ifndef SDIO_H_
#define SDIO_H_

/* eError*/


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
#include "sd_map.h"
#include "DMA.h"
/****************************************************************************
 *  DEFINITIONS
 ****************************************************************************/
//#define MON_SDIO_POLL
#define MON_SDIO_DMA


#if defined (MON_SDIO_POLL)
    #define MON_SDIO_TX_POLL
    #define MON_SDIO_RX_POLL
#elif defined MON_SDIO_DMA
    #define MON_SDIO_TX_DMA
    #define MON_SDIO_RX_DMA
#endif
/****************************************************************************
 *  TYPE DEFINITIONS
 ****************************************************************************/

typedef enum eDataBitTransfer
{
	TRANSMIT_1_BIT,
	TRANSMIT_4_BIT,
	TRANSMIT_8_BIT,
	NUM_OF_BIT_TRANSFER_TYPES
}tDataBitTransfer;



typedef struct
{
	uint8_t  ClockDiv;              /*!< Specifies the clock frequency of the SDIO controller.
										   This parameter can be a value between 0x00 and 0xFF. */
	uint32_t ClockEdge;            /*!< Specifies the clock transition on which the bit capture is made.
										   This parameter can be a value of @ref SDIO_Clock_Edge */
	uint32_t ClockBypass;
	uint32_t ClockPowerSave;
	uint32_t BusWide;              /*!< Specifies the SDIO bus width.
										THis parameter can be a value of @ref SDIO_Bus_Wide */
	uint32_t HardwareFlowControl;

} tSdioInit;

typedef struct
{

	  uint32_t Argument;  /*!< Specifies the SDIO command argument which is sent
	                                to a card as part of a command message. If a command
	                                contains an argument, it must be loaded into this register
	                                before writing the command to the command register */

	  uint32_t CmdIndex;  /*!< Specifies the SDIO command index. It must be lower than 0x40. */

	  uint32_t Response;  /*!< Specifies the SDIO response type.
	                                This parameter can be a value of @ref SDIO_Response_Type */

	  uint32_t Wait;      /*!< Specifies whether SDIO wait for interrupt request is enabled or disabled.
	                                This parameter can be a value of @ref SDIO_Wait_Interrupt_State */

	  uint32_t CPSM;      /*!< Specifies whether SDIO Command path state machine (CPSM)
	                                is enabled or disabled.
	                                This parameter can be a value of @ref SDIO_CPSM_State */

} tSdioCmd;

typedef struct
{
  uint32_t DataTimeOut;    /*!< Specifies the data timeout period in card bus clock periods. */

  uint32_t DataLength;     /*!< Specifies the number of data bytes to be transferred. */

  uint32_t DataBlockSize;  /*!< Specifies the data block size for block transfer.
                                     This parameter can be a value of @ref SDIO_Data_Block_Size */

  uint32_t TransferDir;    /*!< Specifies the data transfer direction, whether the transfer
                                     is a read or write.
                                     This parameter can be a value of @ref SDIO_Transfer_Direction */

  uint32_t TransferMode;   /*!< Specifies whether data transfer is in stream or block mode.
                                     This parameter can be a value of @ref SDIO_Transfer_Type */

  uint32_t DPSM;           /*!< Specifies whether SDIO Data path state machine (DPSM)
                                     is enabled or disabled.
                                     This parameter can be a value of @ref SDIO_DPSM_State */
} tSdioDataConfig;



/****************************************************************************
 *    PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************/
eError sdInit(void);
eError sdTransferInit(uint8_t BusWide);
eError sdStop(void);
eError sdStart(void);
eError sdSleep(void);
eError sdWake(void);

eError sdDriverRead(tSd sdCard, uint32_t *buff, uint64_t readAddress, uint32_t NumberOfBlocks);
eError sdDriverWrite(tSd sdCard, uint32_t *buff, uint64_t writeAddress, uint32_t NumberOfBlocks);
uint32_t sdDriverGetBufferSize(void);
eError sdDriverSetBufferSize(uint32_t bufferSize);
eError sdDriverGetStatus(tSd sdCard);
uint32_t sdDriverGetSector(void);
eError sdDriverSetSector(uint32_t sector);
uint32_t sdDriverGetNumberOfBlocks(void);
eError sdDriverSetNumberOfBlocks(uint32_t numberOfBlocks);


//DMA
//void DMATxConfig (uint32_t *BufferSRC);
//void DMARxConfig (uint32_t *BufferDST);
#endif // _SDIO_H_

/****************************************************************************
 *    End of file
 ***************************************************************************/
