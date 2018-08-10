/*************************************************************************//**
 * @file version.h
 * 
 * @brief Define current version related with all areas of project.
 * 
 * @author Rubén Rodríguez
 * 
 * @date 3/1/2012
 *
 ****************************************************************************/

#ifndef _VERSION_H_
#define _VERSION_H_



/****************************************************************************
 *   DEFINES
 ****************************************************************************/

/* SW Release type */
//#define TEST_VERSION

#define START_APPLICATION_MESSAGE "MONGENESIS APP"

#define HW_VERSION				0
#define FW_MAJOR_VERSION        0
#define FW_MINOR_VERSION        1
#define FW_RC_VERSION			0

#define FW_TEST_VERSION			0 /* This value will be added to ASCII value of character 'A' */

#define NVM_HREG_MAP_VERSION  		1

#define NVM_SREG_FACTORY_MAP_VERSION		1
#define NVM_SREG_CALIBRATION_MAP_VERSION	1
#define NVM_SREG_USER_MAP_VERSION			1

#define NVM_DATA_MAP_VERSION				1
 
#endif /*_VERSION_H_*/

/****************************************************************************
 *    End of file
 ****************************************************************************/
