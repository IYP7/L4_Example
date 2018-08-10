/***********************************************************************
 * @file sreg_dat.h
 * 
 * @brief Base template for the definition of SREG registers.
 * 
 * @author generate by MonGenesis
 * 
 * @date xx/xx/2016 
 * 
 **********************************************************************/

#ifndef _SREG_DAT_H_
#define _SREG_DAT_H_

/***********************************************************************
* COMMON INCLUDE
***********************************************************************/

/***********************************************************************
* ARCHITECTURE INCLUDES
***********************************************************************/
#include "Common.h"
#include "sreg.h"

/***********************************************************************
* DEFINES
***********************************************************************/

typedef enum eSREG 
{
	SREG_HW_ID,
	SREG_FW_MAJOR_VERSION,
	SREG_FW_MINOR_VERSION,
	SREG_FW_PATCH_VERSION,
	SREG_SYSTEM_STATE,
	SREG_SYSTEM_MW_NOTIF,
	SREG_NUMBER_OF_POWER_UPS,
	SREG_WORKING_HOURS,
	SREG_ALARM_I2C,

	NUM_OF_SREGS
}SREG;

typedef enum eSreg8
{
	SREG8_HwId,
	SREG8_FwMajorVersion,
	SREG8_FwMinorVersion,
	SREG8_FwPatchVersion,
	SREG8_SystemState,
	SREG8_SystemMwNotif,
	SREG8_AlarmI2c,
	NUM_OF_SREGS8,
}tSreg8;

typedef enum eSreg16
{
	NUM_OF_SREGS16,
}tSreg16;

typedef enum eSreg32
{
	SREG32_NumberOfPowerUps,
	SREG32_WorkingHours,
	NUM_OF_SREGS32,
}tSreg32;

#endif /* _SREG_DAT_H_ */
