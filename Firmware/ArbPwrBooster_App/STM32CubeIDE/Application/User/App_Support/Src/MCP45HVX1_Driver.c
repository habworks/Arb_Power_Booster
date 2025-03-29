/******************************************************************************************************
 * @file 			MCP45HVX1_Driver.c
 * @brief			<Add a descrition of what the file does>
 * ****************************************************************************************************
 * @author original	Hab Collector (habco)\n
 *
 * @version       	See Main_Support.h: FIRMWARE_REV_MAJOR, FIRMWARE_REV_MINOR
 *
 * @param Development_Environment \n
 * Hardware:		<IC PN> \n
 * IDE:             STMCubeIDE VER 1.9.0 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            The associated header file provides MACRO functions for IO control
 *
 * 					This is an embedded application
 *		            It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *			          Schematic: <Schematic PN>
 *				      STM32F746G_DISCO
 *				      ACI Pittsburgh
 *
 * @copyright		Applied Concepts, Inc
 ******************************************************************************************************/

#include "MCP45HVX1_Driver.h"


bool MCP45HVX1_SetWiperValue(I2C_HandleTypeDef *I2C_Handle, uint8_t A1A0_ExternalAddress, uint8_t Value)
{
    uint8_t I2C_Address;
    uint8_t CommandDataBuffer[2] = {0x00};
    I2C_Address = MCP45HVX1_FIXED_ADDRESS | (A1A0_ExternalAddress << 1);
    CommandDataBuffer[0] = ((MCP45HVX1_VOLATILE_WIPER_0_ADDR & 0x0F) << 4) | ((MCP45HVX1_WRITE_COMMAND & 0x03) << 2);
    CommandDataBuffer[1] = Value;

    HAL_StatusTypeDef Status = HAL_I2C_Master_Transmit(I2C_Handle, I2C_Address, CommandDataBuffer, sizeof(CommandDataBuffer), HAL_MAX_DELAY);
    if (Status == HAL_OK)
        return(true);
    else
        return(false);
}


bool MCP45HVX1_ReadWiperValue(I2C_HandleTypeDef *I2C_Handle, uint8_t A1A0_ExternalAddress, uint8_t *Value)
{
    uint8_t I2C_Address;
    uint8_t Command = 0x00;
    I2C_Address = MCP45HVX1_FIXED_ADDRESS | (A1A0_ExternalAddress << 1);
    Command = ((MCP45HVX1_VOLATILE_WIPER_0_ADDR & 0x0F) << 4) | ((MCP45HVX1_READ_COMMAND & 0x03) << 2);

    if (HAL_I2C_Master_Transmit(I2C_Handle, I2C_Address, &Command, 1, HAL_MAX_DELAY) != HAL_OK)
        return(false);

    HAL_StatusTypeDef Status = HAL_I2C_Master_Receive(I2C_Handle, I2C_Address, Value, 1, HAL_MAX_DELAY);
    if (Status == HAL_OK)
        return(true);
    else
        return(false);

}
