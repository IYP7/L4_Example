/*************************************************************************//**
 * @file interrupts.c
 * @date 27/01/2016
 * @Author Dario Davide
 *
 * @addtogroup HAL
 * @{
 * @addtogroup Interrupts
 * @{
 *
 * @brief INTERRUPTS driver control.
 *
 * @details INTERRUTPS driver control provides functions allowing to configure the NVIC interrupts (IRQ)
 *  - Configure the priority of the selected IRQ Channels using interrupts_map.h
 *  - Enable the selected IRQ Channels using either API, or interrupts_map.h
 *
 * The API for this Module is handled with HREG following:
 *  - Enable IRQ Channel (HREG_ENABLE)
 *
 ****************************************************************************/


/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/
#include "hreg_dat.h"

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "interrupts.h"
#include "interrupts_map.h"

/****************************************************************************
 *  EXTERNAL VARIABLES
 ****************************************************************************/

extern tInterruptsInstanceMap interruptsInstanceMap[MAX_IRQ];

/**********************************************************************
 * PRIVATE DEFINES
 **********************************************************************/

/**********************************************************************
 * PRIVATE TYPES
 **********************************************************************/

/**********************************************************************
 * PRIVATE VARIABLES DEFINITIONS
 **********************************************************************/
volatile static uint16_t countIrqAllDisable = 0;

/****************************************************************************
 *    PRIVATE FUNCTIONS DECLARATIONS
 ****************************************************************************/

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ****************************************************************************/

/**********************************************************************
 * @brief  Init interrupts driver
 * 		   Configure all IRQ Channels declared on intPriorityMap list
 * @return eError :
 **********************************************************************/
eError interruptsInit( void )
{
	eError result = RET_OK;
	uint8_t i;

	for(i = 0; i < MAX_IRQ; i++)
	{
		if (IS_SETTABLE_PRIORITY_IRQ(interruptsInstanceMap[i].IRQn))
		{
			HAL_NVIC_SetPriority(interruptsInstanceMap[i].IRQn, (uint32_t)interruptsInstanceMap[i].PreemptPriority, (uint32_t)interruptsInstanceMap[i].SubPriority);
		}
	}
	return result;
}

/**********************************************************************
 * @brief  Start interrupts driver
 * 		   Enable all IRQ Channels configured as ENABLE on intPriorityMap list
 * @return eError :
 **********************************************************************/
eError interruptsStart( void )
{
	eError result = RET_OK;
	uint8_t i;

	for(i = 0; i < MAX_IRQ; i++)
	{
		if (interruptsInstanceMap[i].State == IRQ_ENABLE)
		{
			if (IS_NVIC_DEVICE_IRQ(interruptsInstanceMap[i].IRQn))
			{
				HAL_NVIC_EnableIRQ(interruptsInstanceMap[i].IRQn);
			}

		}
	}

	return result;
}

/**********************************************************************
 * @brief  Stop interrupts driver
 * 		   Disable all IRQ Channels declared on intPriorityMap list
 * @return eError :
 **********************************************************************/
eError interruptsStop( void )
{
	eError result = RET_OK;

	uint8_t i;

	for(i = 0; i < MAX_IRQ; i++)
	{
		if (IS_NVIC_DEVICE_IRQ(interruptsInstanceMap[i].IRQn))
		{
			HAL_NVIC_DisableIRQ(interruptsInstanceMap[i].IRQn);
		}
	}

	return result;
}

/*************************************************************************//**
 * @brief  Sleep interrupts driver
 * 		   Disable all IRQ Channels declared on intPriorityMap list
 * @return eError :
 ****************************************************************************/
eError interruptsSleep( void )
{
	eError result = RET_OK;

    interruptsStop();

	return result;
}

/*************************************************************************//**
 * @brief  Wake the interrupts driver
 * @return eError :
 ****************************************************************************/
eError interruptsWake( void )
{
	eError result = RET_OK;

    interruptsStart();

	return result;
}


/*************************************************************************//**
 * @brief  get Enable state of the selected IRQ Channels using intPriorityMap
 * @param  IRQn_Name: index of IRQ Channels
 * @return Enable state
 ****************************************************************************/
tIRQState interruptsGetState(tInterrupts indexIRQ)
{
	tIRQState state = IRQ_DISABLE;


	if (__get_PRIMASK()==0)
	{
		if (indexIRQ < MAX_IRQ)
		{
			state = interruptsInstanceMap[indexIRQ].State;
		}
		else if (indexIRQ==IRQ_ALL)
		{
			state = IRQ_ENABLE;
		}
	}

	return state;

}


/*************************************************************************//**
 * @brief  set Enables or disables a device specific interrupt
 * @param  IRQn_Name:	index of interrupt
 * @return eError :
 ****************************************************************************/
eError interruptsSetState(tInterrupts indexIRQ,tIRQState state)
{
	eError result = RET_OK;


	if (indexIRQ > MAX_IRQ)
	{
		result = RET_INVALID_PARAMETER;
	}
	else if (indexIRQ == IRQ_ALL)
	{
		if (state == IRQ_ENABLE)
		{
			interruptsEnable();
		} else
		{
			interruptsDisable();
		}

	}
	else if (IS_NVIC_DEVICE_IRQ(interruptsInstanceMap[indexIRQ].IRQn))
	{
		if (state == IRQ_ENABLE)
		{
			HAL_NVIC_EnableIRQ(interruptsInstanceMap[indexIRQ].IRQn);
		} else
		{
			HAL_NVIC_DisableIRQ(interruptsInstanceMap[indexIRQ].IRQn);
		}

		interruptsInstanceMap[indexIRQ].State = state;

	}
	else if (interruptsInstanceMap[indexIRQ].IRQn == SysTick_IRQn)
	{
		if (state == IRQ_ENABLE)
		{
			HAL_ResumeTick();
		} else
		{
			HAL_SuspendTick();
		}

		interruptsInstanceMap[indexIRQ].State = state;

	}
	else
	{
		result = RET_INVALID_PARAMETER;
	}

	return result;
}

/*************************************************************************//**
 * @brief  Enables all interrupts.
 * @param	None.
 * @return  None.
 ****************************************************************************/
void interruptsEnable( void )
{
	if (countIrqAllDisable<=1)
	{
		countIrqAllDisable=0;
		__enable_irq();
	}
	else
	{
		countIrqAllDisable--;
	}

}

/*************************************************************************//**
 * @brief  Disables all interrupts.
 * @param	None.
 * @return  None.
 ****************************************************************************/
void interruptsDisable( void )
{
	__disable_irq();
	countIrqAllDisable++;
}

/**@}*/
/**@}*/

/****************************************************************************
 *    End of file
 ***************************************************************************/

