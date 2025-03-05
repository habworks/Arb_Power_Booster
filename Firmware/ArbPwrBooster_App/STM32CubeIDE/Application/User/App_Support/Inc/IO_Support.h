/** ****************************************************************************************************
 * @file 			IO_Support.h
 * @brief			This is the Header file used to support IO_Support.c
 * ****************************************************************************************************
 * @author original Hab Collector (habco)\n
 *
 * @version         See Main_Support.h: FW_MAJOR_REV, FW_MINOR_REV, FW_TEST_REV
 *
 * @param Development_Environment \n
 * Hardware:        <STM32F764G-DISCO> \n
 * IDE:             STMCubeIDE VER 1.17.0 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            The associated header file provides MACRO functions for IO control
 *
 *                  This is an embedded application
 *                  It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *                    Schematic: IMR-005-SCH
 *                    STM32F746G_DISCO
 *                    IMR
 *
 * @copyright       IMR Engineering, LLC
 ********************************************************************************************************/

#ifndef APPLICATION_USER_IO_SUPPORT_H_
#define APPLICATION_USER_IO_SUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "main.h"

// DEFINES
#define MAIN_PWR_ON()           HAL_GPIO_WritePin(MAIN_PWR_EN_GPIO_Port, MAIN_PWR_EN_Pin, GPIO_PIN_SET)
#define MAIN_PWR_OFF()          HAL_GPIO_WritePin(MAIN_PWR_EN_GPIO_Port, MAIN_PWR_EN_Pin, GPIO_PIN_RESET)
#define MAIN_PWR_TOGGLE()       HAL_GPIO_TogglePin(MAIN_PWR_EN_GPIO_Port, MAIN_PWR_EN_Pin)
#define MAIN_PWR_STATUS()       HAL_GPIO_ReadPin(MAIN_PWR_EN_GPIO_Port, MAIN_PWR_EN_Pin)

#define CH1_OUTPUT_ENABLE()     HAL_GPIO_WritePin(EN_VO_1_GPIO_Port, EN_VO_1_Pin, GPIO_PIN_SET)
#define CH1_OUTPUT_DISABLE()    HAL_GPIO_WritePin(EN_VO_1_GPIO_Port, EN_VO_1_Pin, GPIO_PIN_RESET)
#define CH1_OUTPUT_TOGGLE()     HAL_GPIO_TogglePin(EN_VO_1_GPIO_Port, EN_VO_1_Pin)
#define CH1_OUTPUT_STATUS()     HAL_GPIO_ReadPin(EN_VO_1_GPIO_Port, EN_VO_1_Pin)

#define CH1_INPUT_50_ENABLE()   HAL_GPIO_WritePin(EN_50_Z_1_GPIO_Port, EN_50_Z_1_Pin, GPIO_PIN_SET)
#define CH1_INPUT_50_DISABLE()  HAL_GPIO_WritePin(EN_50_Z_1_GPIO_Port, EN_50_Z_1_Pin, GPIO_PIN_RESET)
#define CH1_INPUT_50_TOGGLE()   HAL_GPIO_TogglePin(EN_50_Z_1_GPIO_Port, EN_50_Z_1_Pin)
#define CH1_INPUT_50_STATUS()   HAL_GPIO_ReadPin(EN_50_Z_1_GPIO_Port, EN_50_Z_1_Pin)

#define CH2_OUTPUT_ENABLE()     HAL_GPIO_WritePin(EN_VO_2_GPIO_Port, EN_VO_2_Pin, GPIO_PIN_SET)
#define CH2_OUTPUT_DISABLE()    HAL_GPIO_WritePin(EN_VO_2_GPIO_Port, EN_VO_2_Pin, GPIO_PIN_RESET)
#define CH2_OUTPUT_TOGGLE()     HAL_GPIO_TogglePin(EN_VO_2_GPIO_Port, EN_VO_2_Pin)
#define CH2_OUTPUT_STATUS()     HAL_GPIO_ReadPin(EN_VO_2_GPIO_Port, EN_VO_2_Pin)

#define CH2_INPUT_50_ENABLE()   HAL_GPIO_WritePin(EN_50_Z_2_GPIO_Port, EN_50_Z_2_Pin, GPIO_PIN_SET)
#define CH2_INPUT_50_DISABLE()  HAL_GPIO_WritePin(EN_50_Z_2_GPIO_Port, EN_50_Z_2_Pin, GPIO_PIN_RESET)
#define CH2_INPUT_50_TOGGLE()   HAL_GPIO_TogglePin(EN_50_Z_2_GPIO_Port, EN_50_Z_2_Pin)
#define CH2_INPUT_50_STATUS()   HAL_GPIO_ReadPin(EN_50_Z_2_GPIO_Port, EN_50_Z_2_Pin)

#define ADC1_C_RATE_ENABLE()   HAL_GPIO_WritePin(ADC1_CRATE_GPIO_Port, ADC1_CRATE_Pin, GPIO_PIN_SET)
#define ADC1_C_RATE_DISABLE()  HAL_GPIO_WritePin(ADC1_CRATE_GPIO_Port, ADC1_CRATE_Pin, GPIO_PIN_RESET)
#define ADC1_C_RATE_TOGGLE()   HAL_GPIO_TogglePin(ADC1_CRATE_GPIO_Port, ADC1_CRATE_Pin)
#define ADC1_C_RATE_STATUS()   HAL_GPIO_ReadPin(ADC1_CRATE_GPIO_Port, ADC1_CRATE_Pin)

#define ADC3_C_RATE_ENABLE()   HAL_GPIO_WritePin(ADC3_CRATE_GPIO_Port, ADC3_CRATE_Pin, GPIO_PIN_SET)
#define ADC3_C_RATE_DISABLE()  HAL_GPIO_WritePin(ADC3_CRATE_GPIO_Port, ADC3_CRATE_Pin, GPIO_PIN_RESET)
#define ADC3_C_RATE_TOGGLE()   HAL_GPIO_TogglePin(ADC3_CRATE_GPIO_Port, ADC3_CRATE_Pin)
#define ADC3_C_RATE_STATUS()   HAL_GPIO_ReadPin(ADC3_CRATE_GPIO_Port, ADC3_CRATE_Pin)

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_USER_IO_SUPPORT_H_ */
