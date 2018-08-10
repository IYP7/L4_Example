/*************************************************************************//**
 * @file
 *
 * @author Jordi Castells
 *
 * @date 4/05/2016
 * @brief Set of shell commands that make use of smbUtils to read/write 8/16
 * bits of information from different devices directly from the shell
 *
 * @addtogroup Architecture
 * @{
 * @addtogroup ShellCommands-i2c
 * @{
 * @brief a set of commands to access the I2C bus directly from the shell
 * @detailed this commands give more features to higher level Montseny shells
 * like montseny serialSHell
 *
 ****************************************************************************/

#ifndef _MON_SHELL_I2C_COMMANDS_
#define _MON_SHELL_I2C_COMMANDS_

/****************************************************************************
 * INCLUDE
 ***************************************************************************/
#include "shell_commands_map.h"

#ifdef MON_SHELL_I2C_COMMANDS //Define to Add or Remove this from the Accepted shell commands

#include "shell.h"      //shell uitls
#include "i2c_map.h"    //Access the I2C Map
#include "smbusUtils.h" //Small 8,16 bit functions to get data via I2C


extern const tI2cPeripheralMap I2CPeripheralMap[NUM_OF_I2C_DEVICES];


/*************************************************************************//**
 * @brief       Read a value from an I2C device register. Uint 16
 * @param[in]	command Pointer to command
 * @return      None.
 ****************************************************************************/
void runShellCommandReadI2c16(tParsedCommand* command)
{
    uint16_t val;

    uint16_t i2cIndex;
    uint8_t  i2cRegister;

    i2cIndex    = command->params[0].value;
    i2cRegister = command->params[1].value;

    if(i2cIndex > NUM_OF_I2C_DEVICES){
        SHELL_PRINT_ERROR(SHELL_INVALID_PARAMETER);
        return;
    }

    if(smbRdU16(i2cIndex, i2cRegister, &val) != RET_OK){
        SHELL_PRINT_ERROR(SHELL_READ_ERROR);
        return;
    }

    SHELL_PRINTLN_HEX_VALUE(val);
}

/*************************************************************************//**
 * @brief       Write a register to an I2C Device
 * @param[in]	command Pointer to command
 * @return      None.
 ****************************************************************************/
void runShellCommandWriteI2c16(tParsedCommand* command)
{
    uint16_t i2cIndex;
    uint16_t i2cValue;
    uint8_t  i2cRegister;

    i2cIndex    = command->params[0].value;
    i2cRegister = command->params[1].value;
    i2cValue    = command->params[2].value;

    if(i2cIndex > NUM_OF_I2C_DEVICES){
        SHELL_PRINT_ERROR(SHELL_INVALID_PARAMETER);
        return;
    }

    if(smbWrU16(i2cIndex, i2cRegister, i2cValue) != RET_OK){
        SHELL_PRINT_ERROR(SHELL_READ_ERROR);
        return;
    }

    SHELL_PRINTLN_STR("OK");
}


/*************************************************************************//**
 * @brief       Read a value from an I2C device register. Uint 8
 * @param[in]	command Pointer to command
 * @return      None.
 ****************************************************************************/
void runShellCommandReadI2c8(tParsedCommand* command)
{
    uint8_t val;

    uint16_t i2cIndex;
    uint8_t  i2cRegister;

    i2cIndex    = command->params[0].value;
    i2cRegister = command->params[1].value;

    if(i2cIndex > NUM_OF_I2C_DEVICES){
        SHELL_PRINT_ERROR(SHELL_INVALID_PARAMETER);
        return;
    }

    if(smbRdU8(i2cIndex, i2cRegister, &val) != RET_OK){
        SHELL_PRINT_ERROR(SHELL_READ_ERROR);
        return;
    }

    SHELL_PRINTLN_HEX_VALUE(val);
}

/*************************************************************************//**
 * @brief       Write a register to an I2C Device
 * @param[in]	command Pointer to command
 * @return      None.
 ****************************************************************************/
void runShellCommandWriteI2c8(tParsedCommand* command)
{
    uint16_t i2cIndex;
    uint8_t i2cValue;
    uint8_t  i2cRegister;

    i2cIndex    = command->params[0].value;
    i2cRegister = command->params[1].value;
    i2cValue    = command->params[2].value;

    if(i2cIndex > NUM_OF_I2C_DEVICES){
        SHELL_PRINT_ERROR(SHELL_INVALID_PARAMETER);
        return;
    }

    if(smbWrU8(i2cIndex, i2cRegister, i2cValue) != RET_OK){
        SHELL_PRINT_ERROR(SHELL_READ_ERROR);
        return;
    }

    SHELL_PRINTLN_STR("OK");
}


/*************************************************************************//**
 * @brief       List I2c Devices Addresses
 * @param[in]	command Pointer to command
 * @return      None.
 ****************************************************************************/
void runShellCommandLsI2c(tParsedCommand* command)
{
    uint8_t i;

    for(i=0;i<NUM_OF_I2C_DEVICES;i++){
        SHELL_PRINT_DEC_VALUE(i);
        SHELL_PRINT_STR(": ");
        SHELL_PRINT_DEC_VALUE(I2CPeripheralMap[i].port);
        SHELL_PRINT_STR(" ");
        SHELL_PRINT_HEX_VALUE(I2CPeripheralMap[i].address);
        SHELL_PRINT_NEWLINE();
        HAL_Delay(10);
    }
}

#endif //MON_SHELL_I2C_COMMANDS

#endif //_MON_SHELL_I2C_COMMANDS_
/**@}*/
/**@}*/
/****************************************************************************
 *    End of file
 ***************************************************************************/


