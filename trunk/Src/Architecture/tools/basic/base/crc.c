/*************************************************************************//**
 * @file
 * @author Jordi Castells
  *
 * @addtogroup Architecture
 * @{
 * @addtogroup Tools
 * @{
 * @addtogroup Basics
 * @{
 *
 * @date 10/03/2016
 *
 ****************************************************************************/

#include <stdint.h>
#include "crc.h"

/**
 * Return CRC-8 of the data, using  the specified polynomial CRC8
 * Polynomial examples
 * x^8 + x^2 + x + 1  = 0x07
 * x^8 + x^4 + x + 1  = 0x13
 *
 * @param[in]  vptr Pointer to uint8_t data to create the CRC for
 * @param[in]  len length of the data
 * @param[in]  poly Polynomial. Assumes x^8 always 1, and defines the rest in
 *             an uint8_t.
 * @return The CRC8 of that data
 * */

uint8_t Crc8(const void* vptr, uint8_t len, uint8_t poly)
{
	const uint8_t *data = vptr;
	uint8_t crc = 0;
    uint8_t byte,bit;

    for(byte=0; byte < len; byte++){
        crc ^= *data;  /*XOR input byte*/
        for(bit=0;bit<8;bit++){
            if(crc & 0x80)
                crc = (crc << 1) ^ poly;
            else
                crc <<=1;
        }
        data++;
    }
    return crc;
}

/**@}*/
/**@}*/
/**@}*/
/****************************************************************************
 * END OF FILE
 ***************************************************************************/
