/*************************************************************************/

#include "CheckCalc.h"

/*************************************************************************//**
 * @brief
 *
 * @param
 *
 * @return
 *
 ****************************************************************************/
void ResetCheckCalc(void* checkVar, tCheckType checkType)
{
	switch(checkType)
	{
		case CHECKSUM_8:	(*(uint8_t*)checkVar) = 0;	break;
		case CHECKSUM_16:	(*(uint16_t*)checkVar) = 0;	break;
		case CHECKSUM_32:	(*(uint32_t*)checkVar) = 0;	break;
		case CRC_32:		break;
	}
}

/*************************************************************************//**
 * @brief
 *
 * @param
 *
 * @return
 *
 ****************************************************************************/
void AddCheckCalc(void* checkVar, uint32_t checkData, tCheckType checkType)
{
	switch(checkType)
	{
		case CHECKSUM_8:	(*(uint8_t*)checkVar)+= (uint8_t)checkData;		break;
		case CHECKSUM_16:	(*(uint16_t*)checkVar)+= (uint16_t)checkData;	break;
		case CHECKSUM_32:	(*(uint32_t*)checkVar)+= (uint32_t)checkData;	break;
		case CRC_32:		break;
	}
}

/*************************************************************************//**
 * @brief
 *
 * @param
 *
 * @return
 *
 ****************************************************************************/
uint32_t GetCheckCalc(void* checkVar, tCheckType checkType)
{
	uint32_t result = 0;
	switch(checkType)
	{
		case CHECKSUM_8:	result = (*(uint8_t*)checkVar);		break;
		case CHECKSUM_16:	result = (*(uint16_t*)checkVar);	break;
		case CHECKSUM_32:	result = (*(uint32_t*)checkVar);	break;
		case CRC_32:		break;
	}
	return result;
}
