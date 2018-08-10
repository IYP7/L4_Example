/*************************************************************************//**
 * @file checkCalc.h
 *
 * @brief
 *
 * @author Marc Bernabé
 *
 * @date 20/03/2012
 *
 ****************************************************************************/

#ifndef _CHECKCALC_H
#define _CHECKCALC_H

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/

typedef enum eCheckType
{
    CHECKSUM_8 = 0   ,
    CHECKSUM_16      ,
    CHECKSUM_32      ,
    CRC_32           ,
}tCheckType;

void ResetCheckCalc(void* checkVar, tCheckType checkType);
void AddCheckCalc(void* checkVar, uint32_t checkData, tCheckType checkType);
uint32_t GetCheckCalc(void* checkVar, tCheckType checkType);


#endif // _CHECKCALC_H

/****************************************************************************
 *    End of file
 ***************************************************************************/