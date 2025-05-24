/******************************************************************************************************
 * @file 			MCP45HVX1_Driver.c
 * @brief			Supports the I2C interface of Microchip MCP45HVX1 family of products
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
#include "Terminal_Emulator_Support.h"


/********************************************************************************************************
* @brief Init of the MCP45HVX1 for use on the board.  Note there are two channels to be init.  Enable the
* device and set the output to max span on the POT
*
* @author original: Hab Collector \n
*
* STEP 1: Init Channel 1
* STEP 2: Init Channel 2
********************************************************************************************************/
void Init_MCP45HVX1(void)
{
    // STEP 1: Init Channel 1
    MCP45HVX1_ENABLE_CH1();
#ifdef USING_WIPER_I2C_LATCH
    MCP45HVX1_I2C_PASS_CH1();
#endif
    MCP45HVX1_WriteWiperVerify(&hi2c1, A1A0_EXTERNAL_ADDR_CH1, MCP45HVX1_POT_FULL_RESOLUTION);

    // STEP 2: Init Channel 2

} // END OF Init_MCP45HVX1



/********************************************************************************************************
* @brief Write the wiper value to the device.
*
* @author original: Hab Collector \n
*
* @note: Note the max pot value is defined by the USING_MCP45HV31 or USING_MCP45H531
*
* @param I2C_Handle: I2C Handle
* @param A1A0_ExternalAddress: The hardware set address of A1 and A0 of the chip
* @param Value: The wiper value - max value depends on chip in use - see notes
*
* @return True if I2C HAL transmitted OK
*
* STEP 1: Build the I2C Address
* STEP 2: Load the command and the pot wiper value to data
* STEP 3: Transmit the command
********************************************************************************************************/
bool MCP45HVX1_WriteWiperValue(I2C_HandleTypeDef *I2C_Handle, uint8_t A1A0_ExternalAddress, uint8_t Value)
{
    uint8_t I2C_Address;
    uint8_t CommandDataBuffer[2] = {0x00};

    // STEP 1: Build the I2C Address
    I2C_Address = MCP45HVX1_FIXED_ADDRESS | (A1A0_ExternalAddress << 1);

    // STEP 2: Load the command and the pot wiper value to data
    CommandDataBuffer[0] = ((MCP45HVX1_VOLATILE_WIPER_0_ADDR & 0x0F) << 4) | ((MCP45HVX1_WRITE_COMMAND & 0x03) << 2);
    CommandDataBuffer[1] = Value;

    // STEP 3: Transmit the command
    HAL_StatusTypeDef Status = HAL_I2C_Master_Transmit(I2C_Handle, I2C_Address, CommandDataBuffer, sizeof(CommandDataBuffer), HAL_MAX_DELAY);
    if (Status == HAL_OK)
        return(true);
    else
        return(false);

} // END OF MCP45HVX1_WriteWiperValue



/********************************************************************************************************
* @brief Read the wiper value from the device.  From the datasheet this would be referred to as a random
* read command since it consists of a write to set the command followed by a read
*
* @author original: Hab Collector \n
*
* @note: Note the max pot value is defined by the USING_MCP45HV31 or USING_MCP45H531
*
* @param I2C_Handle: I2C Handle
* @param A1A0_ExternalAddress: The hardware set address of A1 and A0 of the chip
* @param Value: The wiper value (returned by reference) - max value depends on chip in use - see notes
*
* @return True if I2C HAL transmitted / Read OK
*
* STEP 1: Build the I2C Address and command
* STEP 2: Transmit the command
* STEP 3: Read the value via I2C - note two bytes are read
********************************************************************************************************/
bool MCP45HVX1_ReadWiperValue(I2C_HandleTypeDef *I2C_Handle, uint8_t A1A0_ExternalAddress, uint8_t *Value)
{
    uint8_t I2C_Address;
    uint8_t Command = 0x00;

    // STEP 1: Build the I2C Address and command
    I2C_Address = MCP45HVX1_FIXED_ADDRESS | (A1A0_ExternalAddress << 1);
    Command = ((MCP45HVX1_VOLATILE_WIPER_0_ADDR & 0x0F) << 4) | ((MCP45HVX1_READ_COMMAND & 0x03) << 2);

    // STEP 2: Transmit the command
    if (HAL_I2C_Master_Transmit(I2C_Handle, I2C_Address, &Command, 1, HAL_MAX_DELAY) != HAL_OK)
        return(false);

    // STEP 3: Read the value via I2C - note two bytes are read
    uint8_t ReadBytes[2] = {0};
    HAL_StatusTypeDef Status = HAL_I2C_Master_Receive(I2C_Handle, I2C_Address, ReadBytes, 2, HAL_MAX_DELAY);
    *Value = ReadBytes[1];
    if (Status == HAL_OK)
        return(true);
    else
    {
#ifdef MCP45HVX1_USES_PRINTF
        printRed("ERROR: MCP45HVX1 Failed to read\r\n");
#endif
        return(false);
    }

} // END OF MCP45HVX1_ReadWiperValue



/********************************************************************************************************
* @brief Write the wiper value to the device.  After write verify it is correct.  Use up to 3 attempts to
* set the value to what is requested.
*
* @author original: Hab Collector \n
*
* @note: Note the max pot value is defined by the USING_MCP45HV31 or USING_MCP45H531
*
* @param I2C_Handle: I2C Handle
* @param A1A0_ExternalAddress: The hardware set address of A1 and A0 of the chip
* @param Value: The wiper value - max value depends on chip in use - see notes
*
* @return True if I2C HAL transmitted OK
*
* STEP 1: Write the value requested to pot
* STEP 2: Read back pot value
* STEP 3: Compare - if it match return success if not try again until max attempts reached
********************************************************************************************************/
bool MCP45HVX1_WriteWiperVerify(I2C_HandleTypeDef *I2C_Handle, uint8_t A1A0_ExternalAddress, uint8_t Value)
{
    uint8_t MaxAttempts = 3;
    uint8_t Attempt = 0;

    do
    {
        // STEP 1: Write the value requested to pot
        if (!MCP45HVX1_WriteWiperValue(I2C_Handle, A1A0_ExternalAddress, Value))
            return(false);

        // STEP 2: Read back pot value
        uint8_t PotWiperValue = 0;
        if (!MCP45HVX1_ReadWiperValue(I2C_Handle, A1A0_ExternalAddress, &PotWiperValue))
            return(false);

        // STEP 3: Compare - if it match return success if not try again until max attempts reached
        if (Value == PotWiperValue)
            return(true);
        Attempt++;
    } while(Attempt < MaxAttempts);

    // If it gets this far then there was a failure to write the value
#ifdef MCP45HVX1_USES_PRINTF
        printRed("ERROR: MCP45HVX1 Failed to write\r\n");
#endif
    return(false);

} // END OF MCP45HVX1_WriteWiperVerify
