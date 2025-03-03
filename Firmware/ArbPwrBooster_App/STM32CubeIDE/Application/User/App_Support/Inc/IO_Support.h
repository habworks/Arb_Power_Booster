/** ****************************************************************************************************
 * @file 			IO_Support.h
 * @brief			This is the Header file used to support IO_Support.h.c
 * ****************************************************************************************************
 * @author original	Hab Collector (habco) \n
 *
 * @version       	See Main_Support.h: FIRMWARE_REV_MAJOR, FIRMWARE_REV_MINOR
 *
 * @param Development_Environment \n
 * Hardware:		<IC PN> \n
 * IDE:             STMCubeIDE VER 1.9.0  \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            See source file for notes
 *
 * 					This is an embedded application
 *		            It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *			          Schematic <Schematic PN>
 *                    STM32F746G_DISCO
 *				      Design Document
 *
 * @copyright		Applied Concepts, Inc
 ****************************************************************************************************** */

#ifndef APPLICATION_USER_IO_SUPPORT_H_
#define APPLICATION_USER_IO_SUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "main.h"

#define MAIN_PWR_ON()           HAL_GPIO_WritePin(MAIN_PWR_EN_GPIO_Port, MAIN_PWR_EN_Pin, GPIO_PIN_SET)
#define MAIN_PWR_OFF()          HAL_GPIO_WritePin(MAIN_PWR_EN_GPIO_Port, MAIN_PWR_EN_Pin, GPIO_PIN_RESET)
#define MAIN_PWR_TOGGLE()       HAL_GPIO_TogglePin(MAIN_PWR_EN_GPIO_Port, MAIN_PWR_EN_Pin)
#define MAIN_PWR_STATUS()       HAL_GPIO_ReadPin(MAIN_PWR_EN_GPIO_Port, MAIN_PWR_EN_Pin)

#define CH1_OUTPUT_ENABLE()     HAL_GPIO_WritePin(EN_VO_1_GPIO_Port, EN_VO_1_Pin, GPIO_PIN_SET)
#define CH1_OUTPUT_DISABLE()    HAL_GPIO_WritePin(EN_VO_1_GPIO_Port, EN_VO_1_Pin, GPIO_PIN_RESET)
#define CH1_OUTPUT_TOGGLE()     HAL_GPIO_TogglePin(EN_VO_1_GPIO_Port, EN_VO_1_Pin)
#define CH1_OUTPUT_STATUS()     HAL_GPIO_ReadPin(EN_VO_1_GPIO_Port, EN_VO_1_Pin)

#define EN_50_Z_1_ON()          HAL_GPIO_WritePin(EN_50_Z_1_EN_GPIO_Port, EN_50_Z_1_EN_Pin, GPIO_PIN_SET)
#define EN_50_Z_1_OFF()         HAL_GPIO_WritePin(EN_50_Z_1_EN_GPIO_Port, EN_50_Z_1_EN_Pin, GPIO_PIN_RESET)
#define EN_50_Z_1_TOGGLE()      HAL_GPIO_TogglePin(EN_50_Z_1_EN_GPIO_Port, EN_50_Z_1_EN_Pin)
#define EN_50_Z_1_STATUS()      HAL_GPIO_ReadPin(EN_50_Z_1_EN_GPIO_Port, EN_50_Z_1_EN_Pin)

#define CH2_OUTPUT_ENABLE()     HAL_GPIO_WritePin(EN_VO_2_GPIO_Port, EN_VO_2_Pin, GPIO_PIN_SET)
#define CH2_OUTPUT_DISABLE()    HAL_GPIO_WritePin(EN_VO_2_GPIO_Port, EN_VO_2_Pin, GPIO_PIN_RESET)
#define CH2_OUTPUT_TOGGLE()     HAL_GPIO_TogglePin(EN_VO_2_GPIO_Port, EN_VO_2_Pin)
#define CH2_OUTPUT_STATUS()     HAL_GPIO_ReadPin(EN_VO_2_GPIO_Port, EN_VO_2_Pin)

#define EN_50_Z_2_ON()          HAL_GPIO_WritePin(EN_50_Z_2_EN_GPIO_Port, EN_50_Z_2_EN_Pin, GPIO_PIN_SET)
#define EN_50_Z_2_OFF()         HAL_GPIO_WritePin(EN_50_Z_2_EN_GPIO_Port, EN_50_Z_2_EN_Pin, GPIO_PIN_RESET)
#define EN_50_Z_2_TOGGLE()      HAL_GPIO_TogglePin(EN_50_Z_2_EN_GPIO_Port, EN_50_Z_2_EN_Pin)
#define EN_50_Z_2_STATUS()      HAL_GPIO_ReadPin(EN_50_Z_2_EN_GPIO_Port, EN_50_Z_2_EN_Pin)



#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_USER_APP_SUPPORT_INC_MAIN_SUPPORT_H_ */
