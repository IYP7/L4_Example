/*************************************************************************//**
 * @file tools.c
 *
 * @brief Support functions library.
 *
 * @author Rubén Rodríguez
 * @author Jose Antonio Garcia Peiro
 *
 * @date 4/6/2012
 *
 ****************************************************************************/

#ifndef _TOOLS_C_
#define _TOOLS_C_

/****************************************************************************
 *  COMMON INCLUDE
 ***************************************************************************/
#include "common.h"

/****************************************************************************
 *  ARCHITECTURE INCLUDES
 ***************************************************************************/

/****************************************************************************
 *  INCLUDES
 ***************************************************************************/
#include "tools.h"
#ifdef CAN_TRACE
#include "ProtocolSlave.h"
#endif

/****************************************************************************
 *  PRIVATE VARIABLES
 ***************************************************************************/


/****************************************************************************
 *  PRIVATE FUNCTIONS PROTOTYPE
 ***************************************************************************/

static void vprint(eVerboseLevels level,const char *fmt, va_list argp);
static void vprintTrace(uint8_t channel,const char *fmt, va_list argp);
static void vprintTraceBin(uint8_t channel,uint8_t *buffer,uint8_t size);

/****************************************************************************
 *  PUBLIC FUNCTIONS
 ***************************************************************************/


/*************************************************************************//**
 * @brief   Converts current VALUE into HEX format.
 *
 * @param   Value to convert.
 * @param   buffer to place conversion.
 *
 * @return  None.
 *
 ****************************************************************************/
uint8_t dtoh( int32_t in, int8_t* out, uint8_t length)
{
    uint8_t i, index = 0;
	uint8_t nibbleData;

	for(i=length; i>0; i--)
	{
		nibbleData = MON_GET_NIBBLE(in, i-1);
		if (nibbleData < 10)
        {
			out[index++] = nibbleData + '0';
        }
        else
        {
			out[index++] = nibbleData - 10 + 'A';
        }
	}
	return index;
}

//****************************************************************************
// Description: converts form hexadecimal character to decimal value
// Params: hexadecimal character
// Return: decimal value
//****************************************************************************
eError htod(int8_t *in, void *out, uint8_t length)
{
	eError ret = RET_OK;
	uint8_t i;
	uint8_t nibbleData;
	uint32_t temp = 0;

	for(i=length; i>0; i--)
	{
		nibbleData = in[i-1];
		if( (nibbleData >= '0') && (nibbleData <= '9') )
		{
			temp = MON_SET_NIBBLE(temp, i-1, nibbleData - '0');
		}
		else if( (nibbleData >= 'A') && (nibbleData <= 'F') )
		{
			temp = MON_SET_NIBBLE(temp, i-1, 10 + (nibbleData - 'A'));
		}
		else
		{
			ret = RET_FAIL;
		}
	}

	if(ret == RET_OK)
	{
		if(length <= 2)
		{
			*((int8_t *)out) = temp;
		}
		else if(length <= 4)
		{
			*((int16_t *)out) = temp;
		}
		else if(length <= 8)
		{
			*((int32_t *)out) = temp;
		}
	}
	return ret;
}

/*************************************************************************//**
 * @brief   Converts current VALUE to CHAR.
 *
 * @param	Value to convert.
 * @param   Value type.
 * @param   buffer to place conversion.
 *
 * @return  None.
 *
 ****************************************************************************/
void itos( int32_t value, uint8_t type, int8_t* buffer )
{
    int8_t i = 0, j = 0;
    int32_t sign = value;
    int8_t str[12];

    if(sign < 0)
    {
        value= -value;
    }

	switch(type)
	{
		case TYPE_HEX:		/* Hexadecimal case */
							j = dtoh(value, buffer, 8);
							/* String end*/
							buffer[j] = '\0';
							break;
							
		case TYPE_DECIMAL:	/* Decimal case */
							do
							{
								str[i++] = value % 10 + '0';
							} while ((value /= 10) > 0);

							if (sign < 0)
							{
								str[i++] = '-';
							}
							for(i--,j=0;i>=0;i--,j++)
							{
								buffer[j] = str[i];
							}
							/* String end*/
							buffer[j]  = '\0';
							break;
    }
}

/*************************************************************************//**
 * @brief   alphanumeric to integer util (base 10).
 *
 * @param	alphanumeric string.
 * @param   converted integer.
 * @param   number of digits.
 *
 * @return  None.
 *
 ****************************************************************************/
void _atoi(const void *buf, uint32_t *result, uint8_t *length)
{
    int8_t *pointer = (int8_t *)buf;
    *result = 0;

    while(*length > 0)
    {
        if ((*pointer >= '0') && (*pointer <= '9'))
        {
            *result *= 10;
            *result += *pointer - '0';
            pointer++;
            (*length)--;
        }
        else
        {
            break;
        }
    }
}

/*************************************************************************//**
 * @brief   mem compare util.
 *
 * @param	First buffer to compare.
 * @param   Second buffer to compare.
 * @param   number of bytes to compare
 *
 * @return  0 if both strings are equal.
 *
 ****************************************************************************/
uint8_t memCompare(const void *buf1, const void *buf2, uint8_t count)
{
    if(!count)
    {
             return 0;
    }

    while(--count && *(int8_t*)buf1 == *(int8_t*)buf2 )
    {
            buf1 = (int8_t*)buf1 + 1;
            buf2 = (int8_t*)buf2 + 1;
    }

    return (*((uint8_t*)buf1) - *((uint8_t*)buf2));
}

uint8_t SearchIntoDelimitedTable(tTokenDelimitedTable table, uint8_t dataToSearch)
{
	uint8_t i = 0;
	uint8_t j = 0;
	bool bFound = false;

	while( (i < table.size) && (bFound == false) )
	{
		if(table.buffer[i] == dataToSearch)
		{
			bFound = true;
		}
		else if(table.buffer[i] == table.token)
		{
			j++;
		}
		i++;
	}

	if(bFound == true)
	{
		return j;
	}
	else
	{
		return 0xFF;
	}
}



void my_printf(tBool enable,uint8_t level,const char *fmt, ...) // custom printf() function
{
	if ((enable) || (level != VERBOSE_GENERAL))
	{
		va_list argp;
		va_start(argp, fmt);
		vprint(level, fmt, argp);
		va_end(argp);
	}
}

void my_printTrace(uint8_t channel,const char *fmt, ...) // custom printf() function
{
    va_list argp;
    va_start(argp, fmt);
    vprintTrace(channel,fmt, argp);
    va_end(argp);
}

void my_printTraceBin(uint8_t channel,uint8_t *buffer,uint8_t size)
{
    vprintTraceBin(channel,buffer, size);
}


/**********************************************************************
 * @brief   Prints assert fail debug info.
 * @param	sFile: Name of file with failed assert.
 * @param	sFunction: Name of function with failed assert.
 * @param	line: Number of line with failed assert.
 * @param	sCond: Assert condition failed.
 **********************************************************************/
#ifdef	__DEBUG__
void myAssertFail(const char *sFile, const char *sFunction, uint16_t line,
        const char *sCond)
{
    printf("%s:%d In function '%s': assert '%s' failed.\n", sFile, line,
            sFunction, sCond);
    while (1);
}
#endif

/****************************************************************************
 *  PRIVATE FUNCTIONS
 ***************************************************************************/

static void vprint(eVerboseLevels level,const char *fmt, va_list argp)
{
    char string[500];
    if(0 < vsprintf(string,fmt,argp)) // build string
    {
    	//printShellString((int8_t*)string,level);
    }
}

static void vprintTrace(uint8_t channel,const char *fmt, va_list argp)
{
    char string[500];
    if(0 < vsprintf(string,fmt,argp)) // build string
    {
#ifdef UART_TRACE
    	WRITE_HREG(UART_TRACE, HREG_UART_BUFFER_SIZE, strlen((char* )string));
    	WRITE_HREG(UART_TRACE, HREG_UART_DATA, string);
#endif
#ifdef CAN_TRACE
        ProtocolTraceSend((uint8_t *)string,channel,strlen((char* )string));
#endif
    }
}

static void vprintTraceBin(uint8_t channel,uint8_t *buffer,uint8_t size)
{
    if(size > 0)
    {
#ifdef UART_TRACE
    	WRITE_HREG(UART_TRACE, HREG_UART_BUFFER_SIZE, size));
    	WRITE_HREG(UART_TRACE, HREG_UART_DATA, buffer);
#endif
#ifdef CAN_TRACE
    	ProtocolTraceSendBin(buffer,channel,size);
#endif
    }
}
#endif /* _TOOLS_C_ */

/****************************************************************************
 *    End of file
 ****************************************************************************/
