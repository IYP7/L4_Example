/***********************************************************************
 * @file     sreg_dat.c
 * 
 * @brief    Definition of all SREG registers.
 * 
 * @author   generate by MonGenesis
 * 
 * @date     05/07/2016 
 * 
 **********************************************************************/

#ifndef _SREG_DAT_C_
#define _SREG_DAT_C_

/***********************************************************************
* ARCHITECTURE INCLUDE
***********************************************************************/
#include "Common.h"
#include "sreg.h"
#include "sreg_dat.h"
#include "System_app.h"
#include "System_mw.h"
#ifdef EEPROM_SREG_SUPPORT
#include "VirtualEEPROM.h"
#endif 

/***********************************************************************
* PUBLIC VARIABLES
***********************************************************************/


/***********************************************************************
* PRIVATE VARIABLES
***********************************************************************/
const tSREGInstanceMap SREGInstanceMap[] = 
{
	 { SREG_UINT8,   SREG8_HwId,                                        EEPROM_SREG_HW_ID,                            SREG_USER },
	 { SREG_UINT8,   SREG8_FwMajorVersion,                              EEPROM_SREG_FW_MAJOR_VERSION,                 SREG_USER },
	 { SREG_UINT8,   SREG8_FwMinorVersion,                              EEPROM_SREG_FW_MINOR_VERSION,                 SREG_USER },
	 { SREG_UINT8,   SREG8_FwPatchVersion,                              NVM_NOP,                                      SREG_NO_STORAGE },   
	 { SREG_UINT8,   SREG8_SystemState,                                 NVM_NOP,                                      SREG_NO_STORAGE },   
	 { SREG_UINT8,   SREG8_SystemMwNotif,                               NVM_NOP,                                      SREG_NO_STORAGE },   
	 { SREG_UINT32,  SREG32_NumberOfPowerUps,                           NVM_NOP,                                      SREG_NO_STORAGE },   
	 { SREG_UINT32,  SREG32_WorkingHours,                               NVM_NOP,                                      SREG_NO_STORAGE },   
	 { SREG_UINT8,   SREG8_AlarmI2c,                                    NVM_NOP,                                      SREG_NO_STORAGE },   
};

const tSREG8InstanceMap SREG8InstanceMap[] = 
{

	{ 	/* SREG_HW_ID */
		/* Max     */	0xFF,
		/* Min     */	0,
		/* Init    */	HW_VERSION,
	},
	{ 	/* SREG_FW_MAJOR_VERSION */
		/* Max     */	0xFF,
		/* Min     */	0,
		/* Init    */	FW_MAJOR_VERSION,
	},
	{ 	/* SREG_FW_MINOR_VERSION */
		/* Max     */	0xFF,
		/* Min     */	0,
		/* Init    */	FW_MINOR_VERSION,
	},
	{ 	/* SREG_FW_PATCH_VERSION */
		/* Max     */	0xFF,
		/* Min     */	0,
		/* Init    */	FW_RC_VERSION,
	},
	{ 	/* SREG_SYSTEM_STATE */
		/* Max     */	SYSTEM_APP_MAX_STATES,
		/* Min     */	SYSTEM_APP_INIT_SHELL,
		/* Init    */	SYSTEM_APP_INIT_SHELL,
	},
	{ 	/* SREG_SYSTEM_MW_NOTIF */
		/* Max     */	NOTIF_SYSTEM_MW_MAX_NOTIF,
		/* Min     */	NOTIF_SYSTEM_MW_NO_REQUEST,
		/* Init    */	NOTIF_SYSTEM_MW_NO_REQUEST,
	},
	{ 	/* SREG_ALARM_I2C */
		/* Max     */	1,
		/* Min     */	0,
		/* Init    */	0,
	},
};

const tSREG16InstanceMap SREG16InstanceMap[] = 
{

};

const tSREG32InstanceMap SREG32InstanceMap[] = 
{

	{ 	/* SREG_NUMBER_OF_POWER_UPS */
		/* Max     */	0xFFFFFFFF,
		/* Min     */	0,
		/* Init    */	0,
	},
	{ 	/* SREG_WORKING_HOURS */
		/* Max     */	0xFFFFFFFF,
		/* Min     */	0,
		/* Init    */	0,
	},
};


#endif /* _SREG_DAT_C_ */
