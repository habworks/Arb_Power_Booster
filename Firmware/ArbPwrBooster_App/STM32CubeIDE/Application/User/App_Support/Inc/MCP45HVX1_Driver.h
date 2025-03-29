 /*****************************************************************************************************
 * @file 			MCP45HVX1_Driver.h
 * @brief			This is the Header file used to support MCP45HVX1_Driver.h.c
 * ****************************************************************************************************
 * @author original Hab Collector (habco)\n
 *
 * @version         See Main_Support.h: FW_MAJOR_REV, FW_MINOR_REV, FW_TEST_REV
 *
 * @param Development_Environment \n
 * Hardware:        <IC PN>  \n
 * IDE:             STMCubeIDE VER 1.17.0 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            <Add user notes starting here>
 *
 *                  This is an embedded application
 *                  It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *                    Schematic: <Schematic PN>
 *				      STM32F746G_DISCO
 *                    IMR
 *
 * @copyright       IMR Engineering, LLC
 ******************************************************************************************************/

#ifndef MCP45HVX1_DRIVER_H_
#define MCP45HVX1_DRIVER_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "Hab_Types.h"
#include "stm32f7xx_hal.h"


// DEFINES
// I2C_BASE_ADDRESS
#define A1A0_EXTERNAL_ADDR_CH1          ((uint8_t)0x00)
#define A1A0_EXTERNAL_ADDR_CH2          ((uint8_t)0x01)
#define MCP45HVX1_FIXED_ADDRESS         ((uint8_t)0x78)
// REGISTER ADDRESS
#define MCP45HVX1_VOLATILE_WIPER_0_ADDR ((uint8_t)0x00)
#define MCP45HVX1_VOLATILE_TCON_0_ADDR  ((uint8_t)0x04)
// COMMANDS
#define MCP45HVX1_WRITE_COMMAND         ((uint8_t)0x00)
#define MCP45HVX1_READ_COMMAND          ((uint8_t)0x03)


// FUNCTION PROTOTYPES
bool MCP45HVX1_SetWiperValue(I2C_HandleTypeDef *I2C_Handle, uint8_t A1A0_ExternalAddress, uint8_t Value);
bool MCP45HVX1_ReadWiperValue(I2C_HandleTypeDef *I2C_Handle, uint8_t A1A0_ExternalAddress, uint8_t *Value);


#ifdef __cplusplus
}
#endif
#endif /* MCP45HVX1_DRIVER_H_ */
