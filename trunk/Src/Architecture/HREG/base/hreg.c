/*************************************************************************//**
 * @file hreg.c
 *
 * @brief This file is providing the interface of HREG access function.
 *
 * This function is generic independently of number of HREG. File hreg_dat.c
 * is defining number and type of HREG.
 *
 * @author Rubén Rodríguez
 *
 * @date 4/1/2012
 *
 ****************************************************************************/

#ifndef _HREG_C_
#define _HREG_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/
#include "hreg.h"
#include "hreg_api.h"
#include "hreg_dat.h"

#ifdef EEPROM_HREG_SUPPORT
#ifdef EEPROM_INTERNAL
//#include "EEPROM.h"
//#include "EEPROM_map.h"
#endif //EEPROM_INTERNAL
#ifdef EEPROM_VIRTUAL
#include "VirtualEEPROM.h"
#include "VirtualEEPROM_map.h"
#endif //EEPROM_VIRTUAL
#endif // EEPROM_HREG_SUPPORT

/****************************************************************************
 *  INCLUDE
 ***************************************************************************/

/****************************************************************************
 *    PUBLIC VARIABLE
 ***************************************************************************/

/****************************************************************************
 *  PRIVATE VARIABLES
 ***************************************************************************/
static uint8_t numOfHregs = 0;

/****************************************************************************
 *  PUBLIC FUNCTIONS
 ***************************************************************************/

 /*************************************************************************//**
 * @brief  Initialize all HREG.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
void initHregAll( void )
{
	uint16_t i = 0;

    /* Init function call */
    while ( i < HREG_GROUP_INDEX_MAX )
    {
    	if ( apiFuncHregGroup[i].Init != HREG_NULL )
    	{
    		apiFuncHregGroup[i].Init();
    	}
        hregGroupsState[i].hregGroupEnabled = HREG_GROUP_INIT;
        i++;
    }
}

/*************************************************************************//**
 * @brief  STOP all HREG.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
void stopHregGroupAll( void )
{
    int16_t i = 0;

    /* Stop function call from bottom to top */
    for ( i = HREG_GROUP_INDEX_MAX - 1; i >= 0; i--)
    {
    	if ( apiFuncHregGroup[i].Stop != HREG_NULL )
    	{
    		apiFuncHregGroup[i].Stop();
    	}
        hregGroupsState[i].hregGroupEnabled = HREG_GROUP_STOPPED;
    }
}

/*************************************************************************//**
 * @brief  START all HREG.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
void startHregGroupAll( void )
{
	const HREG *hregPtr;
    uint8_t instance, groupType = 0;
    uint32_t i = 0, value;

#ifdef EEPROM_HREG_SUPPORT
    uint8_t  updateNVM = FALSE;
#endif //EEPROM_HREG_SUPPORT

    /* Initialize HREG value */
	numOfHregs = 0;

	for ( groupType = 0; groupType < NUM_OF_HREG_GROUP_TYPES; groupType++)
	{

#ifdef EEPROM_HREG_SUPPORT

		if ( groupType == HREG_GROUP_TYPE_DEVICE_DRIVER )
		{
			/* Check if NVM version has changed */
			VirtualEEPROMReadRegister(EEPROM_HREG, EEPROM_HREG_MAP, UNIQUE_INSTANCE, &value);
			if ( value != NVM_HREG_MAP_VERSION )
			{
				updateNVM = TRUE;
			}
		}
#endif //EEPROM_HREG_SUPPORT

		i = 0;
		/* Start function call for groupType drivers drivers */
		while (  i < HREG_GROUP_INDEX_MAX )
		{
			if ( hregTableList[i].GroupType == groupType )
			{
				/* Initialize each HREG */
				for ( hregPtr = hregTableList[i].TableHreg; hregPtr < hregTableList[i].TableHreg + hregTableList[i].TableSize; hregPtr++ )
				{
					numOfHregs++;

					/* Initialize each instance */
					for(instance = 0; instance < hregTableList[i].NumInstances; instance++)
					{
#ifdef EEPROM_HREG_SUPPORT
						if ( groupType == HREG_GROUP_TYPE_DEVICE_DRIVER )
						{
							/* Set register value to init value if it is not linked with EEPROM
							 * or the EEPROM has no correct map.
							 */
							if( hregPtr->NVMId == NVM_NOP || updateNVM == TRUE)
							{
								value = hregPtr->Init;
							}
							else
							{
								VirtualEEPROMReadRegister(EEPROM_HREG, hregPtr->NVMId, instance, &value);
							}

							/* Store value in EEPROM if map is not correct */
							if( updateNVM == TRUE && hregPtr->NVMId != NVM_NOP )
							{
								VirtualEEPROMWriteRegister(EEPROM_HREG, hregPtr->NVMId, instance, value);
							}
						}
						else
						{
							value = hregPtr->Init;
						}
#else //EEPROM_HREG_SUPPORT
						value = hregPtr->Init;
#endif //EEPROM_HREG_SUPPORT
						//writeHreg(instance, hregPtr, value);
						hregFunc.Write( instance, hregPtr, value );
					}
				}
				if ( apiFuncHregGroup[i].Start != HREG_NULL )
				{
					apiFuncHregGroup[i].Start();
				}
				hregGroupsState[i].hregGroupEnabled = HREG_GROUP_STARTED;
			}
			i++;
		}
	}

#ifdef EEPROM_HREG_SUPPORT
    /* Update Test byte on NVM */
    if ( updateNVM == TRUE )
    {
    	VirtualEEPROMWriteRegister(EEPROM_HREG, EEPROM_HREG_MAP, UNIQUE_INSTANCE, NVM_HREG_MAP_VERSION);
    }
#endif //EEPROM_HREG_SUPPORT
}

/*************************************************************************//**
 * @brief  SLEEP all HREG.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
void sleepHregGroupAll( void )
{
    int16_t i = 0;

    /* Stop function call from bottom to top */
    for ( i = HREG_GROUP_INDEX_MAX - 1; i >= 0; i--)
    {
        apiFuncHregGroup[i].Sleep();
        hregGroupsState[i].hregGroupEnabled = HREG_GROUP_SLEPT;
    }
}

/*************************************************************************//**
 * @brief  WAKE UP all HREG.
 *
 * @param	None.
 *
 * @return  None.
 *
 ****************************************************************************/
void wakeHregGroupAll( void )
{
    uint16_t i = 0;

    /* Stop function call */
    while ( apiFuncHregGroup[i].Wake != HREG_NULL )
    {
        apiFuncHregGroup[i].Wake();
        hregGroupsState[i].hregGroupEnabled = HREG_GROUP_STARTED;
        i++;
    }
}

/*************************************************************************//**
 * @brief  STOP one HREG GROUP.
 *
 * @param	HREG group to stop.
 *
 * @return  None.
 *
 ****************************************************************************/
void stopHregGroup( uint8_t hregGroup )
{
    /* Start function call */
    if ( apiFuncHregGroup[hregGroup].Stop != HREG_NULL )
    {
        apiFuncHregGroup[hregGroup].Stop();
        hregGroupsState[hregGroup].hregGroupEnabled = HREG_GROUP_STOPPED;
    }
    else
    {
        /* Illegal HREG GROUP */
    }
}

/*************************************************************************//**
 * @brief  START one HREG GROUP.
 *
 * @param	HREG group to start.
 *
 * @return  None.
 *
 ****************************************************************************/
void startHregGroup( uint8_t hregGroup )
{
    /* Start function call */
    if ( apiFuncHregGroup[hregGroup].Start != HREG_NULL )
    {
        apiFuncHregGroup[hregGroup].Start();
        hregGroupsState[hregGroup].hregGroupEnabled = HREG_GROUP_STARTED;
    }
    else
    {
        /* Illegal HREG GROUP */
    }
}

/*************************************************************************//**
 * @brief  SLEEP one HREG GROUP.
 *
 * @param	HREG group to start.
 *
 * @return  None.
 *
 ****************************************************************************/
void sleepHregGroup( uint8_t hregGroup )
{
    /* Start function call */
    if ( apiFuncHregGroup[hregGroup].Sleep != HREG_NULL )
    {
        apiFuncHregGroup[hregGroup].Sleep();
        hregGroupsState[hregGroup].hregGroupEnabled = HREG_GROUP_SLEPT;
    }
    else
    {
        /* Illegal HREG GROUP */
    }
}

/*************************************************************************//**
 * @brief  SLEEP one HREG GROUP.
 *
 * @param	HREG group to start.
 *
 * @return  None.
 *
 ****************************************************************************/
void wakeHregGroup( uint8_t hregGroup )
{
    /* Start function call */
    if ( apiFuncHregGroup[hregGroup].Wake != HREG_NULL )
    {
        apiFuncHregGroup[hregGroup].Wake();
        hregGroupsState[hregGroup].hregGroupEnabled = HREG_GROUP_STARTED;
    }
    else
    {
        /* Illegal HREG GROUP */
    }
}

/*************************************************************************//**
 * @brief  Write one HREG.
 *
 * @param	HREG pointer to write.
 * @param	New value for HREG.
 *
 * @return  success or not.
 *
 ****************************************************************************/
eError writeHreg( uint8_t instance, const HREG *hregP , uint32_t value )
{
    eError  result = RET_OK;
#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
	uint16_t index = 0;
#endif // MON_SHELL_COMMAND_VERBOSE_REGS

    if ( value > hregP->Max )
    {
        value = hregP->Max;
        printShellString((int8_t *)"HREG value adjusted to maximum\n", VERBOSE_WARNING);
    }
    else if ( value < hregP->Min )
    {
        value = hregP->Min;
        printShellString((int8_t *)"HREG value adjusted to minimum\n", VERBOSE_WARNING);
    }

    /* Check for OnWrite function */
    if ( ( hregP->OnWrite != HREG_NOP ) &&
         ( hregGroupsState[hregP->GroupId].hregGroupEnabled == HREG_GROUP_STARTED ) &&
         ( shellGetDebugState() == FALSE) )
    {
        result = onWriteHregFunc[hregP->OnWrite].Write(instance, hregP, value);
    }

    if ( result == RET_OK )
    {
        /* Write on HREG register */
        result = hregFunc.Write( instance, hregP, value );

#ifdef EEPROM_HREG_SUPPORT
    	/* Check NVM write operation needed */
    	if ( result == RET_OK && hregP->NVMId != NVM_NOP )
    	{
            VirtualEEPROMWriteRegister(EEPROM_HREG, hregP->NVMId, instance, value);
    	}
#endif

#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
		/* Check if new value should be sent to Shell */
		if( getHregIndexFromPointer( hregP, &index ) == RET_OK )
		{
			shellEvaluateHREGVerboseStatus(index, instance, value);
		}
#endif // MON_SHELL_COMMAND_VERBOSE_REGS

    }
    else
    {
    	result = RET_FAIL;
    }

    return result;
}

/*************************************************************************//**
 * @brief  Write one HREG requested from shell.
 *
 * @param	HREG index to write.
 * @param	New value for HREG.
 *
 * @return  success or not.
 *
 ****************************************************************************/
eError writeShellHreg( uint8_t instance, uint16_t hregIndex , uint32_t hregValue)
{
    uint8_t tablePointer = 0;
    uint16_t tableCount = 0;
    uint16_t previousTableCount = 0;
    uint8_t registerFound = FALSE;
    uint8_t result = RET_FAIL;

    /* Table count */
    while ((hregTableList[tablePointer].TableHreg != HREG_NULL) && (registerFound == FALSE))
    {
        tableCount += hregTableList[tablePointer].TableSize;
        if( hregIndex <= (tableCount - 1))
        {
            /* Group found */
            result = writeHreg(instance, &(hregTableList[tablePointer].TableHreg)[hregIndex-previousTableCount], hregValue);
            registerFound = TRUE;
        }
        else
        {
            /* Try next group */
            previousTableCount = tableCount;
            tablePointer++;
        }
    }
    return result;
}

/*************************************************************************//**
 * @brief  Reads one HREG.
 *
 * @param	HREG pointer to read.
 *
 * @return  value of HREG.
 *
 ****************************************************************************/
eError readHreg( uint8_t instance, const HREG *hregP, void *value )
{
    eError result = RET_OK;
    uint8_t size = 0;

    /* Check OnRead function availability*/
    if ( ( hregP->OnRead != HREG_NOP ) &&
         ( hregGroupsState[hregP->GroupId].hregGroupEnabled == HREG_GROUP_STARTED ) &&
         ( shellGetDebugState() == FALSE) )
    {
        result = onReadHregFunc[hregP->OnRead].Read(instance, hregP, value);
    }
    else
    {
		size = SIZE_OF_HREG( hregP );

		switch(size)
		{
			case 1:		*(uint8_t*)value = (uint8_t)hregFunc.Read( instance, hregP );		break;
			case 2:		*(uint16_t*)value = (uint16_t)hregFunc.Read( instance, hregP );		break;
			default:	*(uint32_t*)value = (uint32_t)hregFunc.Read( instance, hregP );		break;
        }
    }

    return result;
}

/*************************************************************************//**
 * @brief  Reads one HREG requested from shell.
 *
 * @param	HREG index to read.
 *
 * @return  value of HREG.
 *
 ****************************************************************************/
eError readShellHreg( uint8_t instance, uint16_t hregIndex, uint32_t *hregValue)
{
    eError result = RET_OK;
    uint8_t tablePointer = 0;
    uint16_t tableCount = 0;
    uint16_t previousTableCount = 0;
    uint8_t registerFound = FALSE;

    /* Table count */
    while ((hregTableList[tablePointer].TableHreg != HREG_NULL) && (registerFound == FALSE))
    {
        tableCount += hregTableList[tablePointer].TableSize;
        if( hregIndex <= (tableCount - 1))
        {
			/* Group found */
			result = readHreg(instance, &(hregTableList[tablePointer].TableHreg)[hregIndex-previousTableCount], hregValue);
			registerFound = TRUE;
        }
        else
        {
			/* Try next group */
			previousTableCount = tableCount;
			tablePointer++;
        }
    }
    return result;
}

/*************************************************************************//**
 * @brief  Reads the total number of HREGs.
 *
 * @return  Number of HREGs.
 *
 ****************************************************************************/
uint8_t getNumOfHregs( void )
{
    return numOfHregs;
}

/*************************************************************************//**
 * @brief  Standard HREG Read function.
 *
 * @param	Pointer to the target HREG.
 * @param   mode Action. HR_REAL_DATA / HR_MAX_DATA / HR_MIN_DATA / HR_INIT_DATA
 *
 * @return  read value.
 *
 ****************************************************************************/
uint32_t readHregTableStd( uint8_t instance, const HREG *hregPtr )
{
    uint32_t ret_data = 0xffffffff;
    uint8_t size = 0;

    /* Read according to the data size (1byte/2byte) */
    size = SIZE_OF_HREG( hregPtr );
	switch(size)
	{
		case 1:		ret_data = ( uint32_t )( *( (uint8_t *)(hregPtr->data) + instance ) );		break;
		case 2:		ret_data = ( uint32_t )( *( (uint16_t *)(hregPtr->data) + instance ) );		break;
		default:	ret_data = ( uint32_t )( *( (uint32_t *)(hregPtr->data) + instance ) );		break;
	}

    return ret_data;
}

/*************************************************************************//**
 * @brief  Standard HREG write function.
 *
 * @param	Pointer to the target HREG.
 * @param   value to write.
 * @param   mode Action. HR_REAL_DATA / HR_MAX_DATA / HR_MIN_DATA / HR_INIT_DATA
 *
 * @return  Success or not.
 *
 ****************************************************************************/
eError writeHregTableStd( uint8_t instance, const HREG *hregPtr, uint32_t value )
{
    eError ret = RET_OK;
    uint8_t size = 0;

    size = SIZE_OF_HREG( hregPtr );

	switch(size)
	{
		case 1:		*( (uint8_t *)(hregPtr->data) + instance ) = (uint8_t)value;	break;
		case 2:		*( (uint16_t *)(hregPtr->data) + instance ) = (uint16_t)value;	break;
		default:	*( (uint32_t *)(hregPtr->data) + instance ) = (uint32_t)value;	break;
	}

    return ret;
}

#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
/*************************************************************************//**
 * @brief  Return HREG index from HREG pointer.
 *
 * @param	HREG pointer.
 *
 * @return  HREG index.
 *
 ****************************************************************************/
eError getHregIndexFromPointer( const HREG *hregPtr, uint16_t *hregIndex )
{
    uint8_t  tablePointer = 0;
    uint8_t  registerCount = 0;
    uint8_t  i = 0;
    eError   success = RET_FAIL;

    tablePointer = hregPtr->GroupId;

    *hregIndex = 0;
    for (i = 0; i < tablePointer; i++)
    {
		*hregIndex = *hregIndex + hregTableList[i].TableSize;
    }

    for (registerCount = 0; registerCount < hregTableList[tablePointer].TableSize; registerCount++)
    {
		if ( (&(hregTableList[tablePointer].TableHreg)[registerCount]) == hregPtr )
		{
			success = RET_OK;
			break;
		}
    }

    *hregIndex = *hregIndex + registerCount;

    return success;
}
#endif // MON_SHELL_COMMAND_VERBOSE_REGS

#endif /* _HREG_C_ */

/****************************************************************************
 *    End of file
 ***************************************************************************/
