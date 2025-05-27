 /*****************************************************************************************************
 * @file            MCP45HVX1_Driver.h
 * @brief           This is the Header file used to support MCP45HVX1_Driver.h.c
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
 *                    STM32F746G_DISCO
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
#include "main.h"
#include "stm32f7xx_hal.h"


// DEFINES
// PRE-PROCESSOR
// USE PRINTF OR NOT IN DRIVER - COMMENT OUT IF NOT USED
#define MCP45HVX1_USES_PRINTF
// PART NUMBER IN USE DEFINE ONLY ONE
//#define USING_MCP45HV31                 // POT TAPS 127
#define USING_MCP45HV51                 // POT TAPS 255
#ifdef USING_MCP45HV31
#define MCP45HVX1_POT_FULL_RESOLUTION   ((uint8_t)0x7F)
#else
#define MCP45HVX1_POT_FULL_RESOLUTION   ((uint8_t)0xFF)
#endif
#if defined(USING_MCP45HV31) && defined(USING_MCP45HV51)
#error User must define either USING_MCP45HV31 or USING_MCP45HV51 not both
#endif
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


// MACROS
// SHUTDOWN CONTROL
#define MCP45HVX1_ENABLE_CH1()          HAL_GPIO_WritePin(R_SHDN_1_GPIO_Port, R_SHDN_1_Pin, GPIO_PIN_SET)
#define MCP45HVX1_DISABLE_CH1()         HAL_GPIO_WritePin(R_SHDN_1_GPIO_Port, R_SHDN_1_Pin, GPIO_PIN_RESET)
#define MCP45HVX1_STATUS_CH1()          HAL_GPIO_ReadPin(R_SHDN_1_GPIO_Port, R_SHDN_1_Pin)

#define MCP45HVX1_ENABLE_CH2()          HAL_GPIO_WritePin(R_SHDN_2_GPIO_Port, R_SHDN_2_Pin, GPIO_PIN_SET)
#define MCP45HVX1_DISABLE_CH2()         HAL_GPIO_WritePin(R_SHDN_2_GPIO_Port, R_SHDN_2_Pin, GPIO_PIN_RESET)
#define MCP45HVX1_STATUS_CH2()          HAL_GPIO_ReadPin(R_SHDN_2_GPIO_Port, R_SHDN_2_Pin)
// I2C WIPER LATCH
//#define USING_WIPER_I2C_LATCH           // Comment out if not using latch in firmware - latch pulled down in hardware - USER MUST DEFINE IO
#ifdef USING_WIPER_I2C_LATCH
#define MCP45HVX1_I2C_LATCH_CH1()       HAL_GPIO_WritePin(R_WLAT_1_GPIO_Port, R_WLAT_1_Pin, GPIO_PIN_SET)
#define MCP45HVX1_I2C_PASS_CH1()        HAL_GPIO_WritePin(R_WLAT_1_GPIO_Port, R_WLAT_1_Pin, GPIO_PIN_RESET)
#define MCP45HVX1_I2C_STATUS_CH1()      HAL_GPIO_ReadPin(R_WLAT_1_GPIO_Port, R_WLAT_1_Pin)
#define MCP45HVX1_I2C_LATCH_CH2()       HAL_GPIO_WritePin(R_WLAT_2_GPIO_Port, R_WLAT_2_Pin, GPIO_PIN_SET)
#define MCP45HVX1_I2C_PASS_CH2()        HAL_GPIO_WritePin(R_WLAT_2_GPIO_Port, R_WLAT_2_Pin, GPIO_PIN_RESET)
#define MCP45HVX1_I2C_STATUS_CH2()      HAL_GPIO_ReadPin(R_WLAT_2_GPIO_Port, R_WLAT_2_Pin)
#endif


// EXTERNS
extern I2C_HandleTypeDef hi2c1;


// FUNCTION PROTOTYPES
void Init_MCP45HVX1(void);
bool MCP45HVX1_WriteWiperValue(I2C_HandleTypeDef *I2C_Handle, uint8_t A1A0_ExternalAddress, uint8_t Value);
bool MCP45HVX1_ReadWiperValue(I2C_HandleTypeDef *I2C_Handle, uint8_t A1A0_ExternalAddress, uint8_t *Value);
bool MCP45HVX1_WriteWiperVerify(I2C_HandleTypeDef *I2C_Handle, uint8_t A1A0_ExternalAddress, uint8_t Value);

#ifdef __cplusplus
}
#endif
#endif /* MCP45HVX1_DRIVER_H_ */
