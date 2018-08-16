/*************************************************************************//**
 * @file sreg.c
 * 
 * @brief This file is providing the interface of SREG access function.
 * 
 * This function is generic independently of number of SREG. File sreg_dat.c 
 * is defining number and type of SREG.
 * 
 * @author Rubén Rodríguez
 * 
 * @date 3/1/2012
 *
 ****************************************************************************/

#ifndef _SREG_C_
#define _SREG_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDE
 ***************************************************************************/
#include "sreg.h"
#include "sreg_dat.h"

#if defined(VIRTUAL_EEPROM_AVAILABLE)
  #include "VirtualEEPROM.h"
  #include "VirtualEEPROM_map.h"
#elif defined(EEPROM_AVAILABLE)
  #include "EEPROM.h"
  #include "Eeprom_map.h"
#endif // EEPROM_SREG_SUPPORT

/****************************************************************************
 *  DEFINES
 ***************************************************************************/
#define NUM_OF_NVM_SREG_MAPS 3

#if defined(VIRTUAL_EEPROM_AVAILABLE)
    #define EEPROM_WRITE_SREG(t, r, i, x) VirtualEEPROMWriteRegister(t, r, i, x)
    #define EEPROM_READ_SREG(t, r, i, x) VirtualEEPROMReadRegister(t, r, i, x)
#elif defined(EEPROM_AVAILABLE)
    #define EEPROM_WRITE_SREG(t, r, i, x) EEPROMWriteRegister(t, r, i, x)
    #define EEPROM_READ_SREG(t, r, i, x) EEPROMReadRegister(t, r, i, x)
#endif



/****************************************************************************
 *    EXTERN VARIABLEA
 ***************************************************************************/
extern const tSREGInstanceMap SREGInstanceMap[];
extern const tSREG8InstanceMap SREG8InstanceMap[];
extern const tSREG16InstanceMap SREG16InstanceMap[];
extern const tSREG32InstanceMap SREG32InstanceMap[];


/****************************************************************************
 *    PRIVATE VARIABLE
 ***************************************************************************/
uint8_t SREG8Context[NUM_OF_SREGS8];
uint16_t SREG16Context[NUM_OF_SREGS16];
uint32_t SREG32Context[NUM_OF_SREGS32];

eError sregWrite(SREG sreg, uint32_t value);

/****************************************************************************
 *    PRIVATE FUNCTION PROTOTYPE
 ***************************************************************************/
#if defined(VIRTUAL_EEPROM_AVAILABLE) || defined(EEPROM_AVAILABLE)
tTableSRegMapNVM SregMapNVM[] =
{
		{ EEPROM_SREG_FACTORY, 		NVM_SREG_FACTORY_MAP_VERSION		},
		{ EEPROM_SREG_CALIBRATION, 	NVM_SREG_CALIBRATION_MAP_VERSION	},
		{ EEPROM_SREG_USER,			NVM_SREG_USER_MAP_VERSION			},
//		{ SREG_NO_STORAGE,			NULL								}
};
#endif // EEPROM_SREG_SUPPORT

/****************************************************************************
 *    PUBLIC FUNCTIONS
 ***************************************************************************/

/*************************************************************************//**
 * @brief   Initialize all SREG.
 * @param	None
 * @return  None  
 ****************************************************************************/
void initSregAll( void )
{
    tSREGParam sregParam;
    SREG sreg;

#if defined(VIRTUAL_EEPROM_AVAILABLE) || defined(EEPROM_AVAILABLE)
    uint32_t value;
    /* Sregs are divided in tree independents NVM areas on Eeprom.  */
    uint8_t numNVMmap = 0;
    uint8_t updateNVMsreg[NUM_OF_NVM_SREG_MAPS] = { FALSE, FALSE, FALSE };

    for ( numNVMmap = 0; numNVMmap < NUM_OF_NVM_SREG_MAPS; numNVMmap++ )
    {
    	/* Reg 0 of TypeNvm is the MAP of the area */
    	EEPROM_READ_SREG(SregMapNVM[numNVMmap].eepromSregType, 0,  UNIQUE_INSTANCE, &value);
    	if ( value != SregMapNVM[numNVMmap].nvmSregMap)
		{
    		updateNVMsreg[numNVMmap] = TRUE;
		}
    }
#endif

    /* Initialize each SREG group */
    for ( sreg = 0; sreg < NUM_OF_SREGS; sreg++ )
    {
#if defined(VIRTUAL_EEPROM_AVAILABLE) || defined(EEPROM_AVAILABLE)
		if ( SREGInstanceMap[sreg].Nvm != NVM_NOP)
		{
			/* Set sreg value to init value if it is not linked with EEPROM or EEPROM has
			 * no correct map.
			 */
			if ( updateNVMsreg[SREGInstanceMap[sreg].TypeNvm] == TRUE )
			{
				sregReadParam(sreg, &sregParam);
				EEPROM_WRITE_SREG(SregMapNVM[SREGInstanceMap[sreg].TypeNvm].eepromSregType, SREGInstanceMap[sreg].Nvm, UNIQUE_INSTANCE, sregParam.Init);
			}
			else
			{
				EEPROM_READ_SREG(SregMapNVM[SREGInstanceMap[sreg].TypeNvm].eepromSregType, SREGInstanceMap[sreg].Nvm, UNIQUE_INSTANCE, &sregParam.Init);
			}
		}
		else
		{
			sregReadParam(sreg, &sregParam);
		}
#else
		sregReadParam(sreg, &sregParam);
#endif
		/* Save on SREG RAM the value */
		sregWrite(sreg, sregParam.Init);
    }


#if defined(VIRTUAL_EEPROM_AVAILABLE) || defined(EEPROM_AVAILABLE)
    /* Update Map byte on NVM */
    for ( numNVMmap = 0; numNVMmap < NUM_OF_NVM_SREG_MAPS; numNVMmap++ )
    {
    	if ( updateNVMsreg[numNVMmap] == TRUE )
    	{
    		EEPROM_WRITE_SREG(SregMapNVM[numNVMmap].eepromSregType, 0,  UNIQUE_INSTANCE, SregMapNVM[numNVMmap].nvmSregMap);
    	}
    }
#endif //EEPROM_HREG_SUPPORT
}


/*************************************************************************//**
 * @brief   Writes sreg value requested from shell.
 * @param	index of SREG.
 * @param   new value for SREG.
 * @return  RET_OK or RET_FAIL.
 ****************************************************************************/
eError writeShellSreg( uint16_t sregIndex , uint32_t sregValue)
{	
	eError  result = RET_OK;

	result = writeSreg( (SREG)sregIndex, sregValue );

	return result;
}

/*************************************************************************//**
 * @brief   Writes sreg value.
 * @param	index of SREG.
 * @param   new value for SREG.
 * @return  RET_OK or RET_FAIL.
 ****************************************************************************/
eError writeSreg( SREG sreg, uint32_t value )
{
	return writeSregTableStd( sreg, value );
}

/*************************************************************************//**
 * @brief   Reads sreg value.
 * @param	index of SREG.
 * @return  value of SREG.  
 ****************************************************************************/
uint32_t readSreg( SREG sreg )
{
	return readSregTableStd(sreg);
}


/*************************************************************************//**
 * @brief   Reads sreg value requested from shell.
 * @param	index of SREG.
 * @return  value of SREG.  
 ****************************************************************************/
uint32_t readShellSreg( uint16_t sregIndex )
{	
	uint32_t sregValue = 0;

	sregValue = readSreg( (SREG)sregIndex );

	return sregValue;
}

/*************************************************************************//**
 * @brief   Returns number of SREGs.
 * @param	None.
 * @return  number of SREGs.  
 ****************************************************************************/
uint8_t getNumOfSregs( void )
{
	return NUM_OF_SREGS;
}


/*************************************************************************//**
 * @brief   Standard SREG Read function.
 * @param	To the target SREG
 * @return  read value.  
 ****************************************************************************/
uint32_t readSregTableStd( SREG sreg )
{
	eError success = RET_OK;
    uint32_t ret_data = 0xffffffff;
	tSREGParam sregParam;

	success = sregReadParam(sreg, &sregParam);
	if ( success == RET_OK )
	{
		ret_data = sregParam.Data;
	}

    return ret_data;
}

/*************************************************************************//**
 * @brief   Standard SREG Write function.
 * @param	SREG.
 * @param	data value to write.
 * @return  Success or not.  
 ****************************************************************************/
eError writeSregTableStd( SREG sreg, uint32_t value)
{
	eError  result = RET_OK;
	tSREGParam sregParam;

	result = sregReadParam(sreg, &sregParam);
	if ( value > sregParam.Max )
	{
		value = sregParam.Max;
		//printShellString((int8_t *)"SREG value adjusted to maximum\n", VERBOSE_WARNING);
	}
	else if ( value < sregParam.Min )
	{
		value = sregParam.Min;
		//printShellString((int8_t *)"SREG value adjusted to minimum\n", VERBOSE_WARNING);
	}


	if ( result == RET_OK )
    {
		sregWrite(sreg, value);

#if defined(VIRTUAL_EEPROM_AVAILABLE) || defined(EEPROM_AVAILABLE)
		/* write sreg to eeprom */
	    if ( SREGInstanceMap[sreg].Nvm != NVM_NOP )
	    {
	    	EEPROM_WRITE_SREG( SregMapNVM[SREGInstanceMap[sreg].TypeNvm].eepromSregType, SREGInstanceMap[sreg].Nvm, UNIQUE_INSTANCE, value);
	    }
#endif

#ifdef MON_SHELL_COMMAND_VERBOSE_REGS
		/* Check if new value should be sent to Shell */
		if ( sreg < NUM_OF_SREGS )
		{
			shellEvaluateSREGVerboseStatus(sreg, value);
		}
#endif // MON_SHELL_COMMAND_VERBOSE_REGS
	}

    return result;
}



/*****************************************************************************
 * @brief   write on RAM the sreg value
 * @param	sreg
 * @return  RET_OK or RET_FAIL
 ****************************************************************************/
eError sregWrite(SREG sreg, uint32_t value)
{
	eError success = RET_OK;
	uint8_t index;

	index = SREGInstanceMap[sreg].indexSREGType;
	switch(SREGInstanceMap[sreg].type)
	{
		case SREG_UINT8:
			SREG8Context[index] = (uint8_t)value;
			break;
		case SREG_UINT16:
			SREG16Context[index] = (uint16_t)value;
			break;
		case SREG_UINT32:
			SREG32Context[index] = (uint32_t)value;
			break;
		default:
			success = RET_INVALID_PARAMETER;
			break;
	}

	return success;
}



/*****************************************************************************
 * @brief   Return a SREG struct with Max, Min and Init value of the InstanceMap
 * @param	sreg
 * @param	sregParam struct
 * @return  RET_OK or RET_FAIL
 ****************************************************************************/
eError sregReadParam(SREG sreg, tSREGParam *sregParam)
{
	eError success = RET_OK;
	uint8_t index;

	index = SREGInstanceMap[sreg].indexSREGType;
	switch(SREGInstanceMap[sreg].type)
	{
		case SREG_UINT8:
			sregParam->Data = (uint32_t)SREG8Context[index];
			sregParam->Init = (uint32_t)(SREG8InstanceMap[index].Init);
			sregParam->Max  = (uint32_t)(SREG8InstanceMap[index].Max);
			sregParam->Min  = (uint32_t)(SREG8InstanceMap[index].Min);
			break;
		case SREG_UINT16:
			sregParam->Data  = (uint32_t)SREG16Context[index];
			sregParam->Init  = (uint32_t)(SREG16InstanceMap[index].Init);
			sregParam->Max   = (uint32_t)(SREG16InstanceMap[index].Max);
			sregParam->Min   = (uint32_t)(SREG16InstanceMap[index].Min);
			break;
		case SREG_UINT32:
			sregParam->Data  = (uint32_t)SREG32Context[index];
			sregParam->Init  = (uint32_t)(SREG32InstanceMap[index].Init);
			sregParam->Max   = (uint32_t)(SREG32InstanceMap[index].Max);
			sregParam->Min   = (uint32_t)(SREG32InstanceMap[index].Min);
			break;
		default:
			success = RET_INVALID_PARAMETER;
			break;
	}

	return success;
}


/*****************************************************************************
 * @brief   Set the Init value on SREG value of type SREG specified (Factory, calibration or user).
 * @param	typeNVM: SREG_FACTORY, SREG_CALIBRATION or SREG_USER.
 * @return	RET_OK or RET_FAIL
 ****************************************************************************/
eError sregResetNVMValues(tSregTypeNVM typeNVM)
{
	eError success = RET_OK;
	SREG indexSreg;
	tSREGParam sregParam;

	for (indexSreg = 0; indexSreg < NUM_OF_SREGS; indexSreg++ )
	{
		if ( SREGInstanceMap[indexSreg].Nvm != NVM_NOP && SREGInstanceMap[indexSreg].TypeNvm == typeNVM )
		{
			success = sregReadParam(indexSreg, &sregParam);
			if (success == RET_OK)
			{
				writeSreg(indexSreg, sregParam.Init);
			}
		}
	}
	return success;
}



#endif /* _SREG_C_ */

/****************************************************************************
 *    End of file
 ***************************************************************************/
