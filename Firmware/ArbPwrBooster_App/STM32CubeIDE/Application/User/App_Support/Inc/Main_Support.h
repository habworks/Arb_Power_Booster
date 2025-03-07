/** ****************************************************************************************************
 * @file 			Main_Support.h
 * @brief			This is the Header file used to support Main_Support.c
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

#ifndef APPLICATION_USER_APP_SUPPORT_INC_MAIN_SUPPORT_H_
#define APPLICATION_USER_APP_SUPPORT_INC_MAIN_SUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "Hab_Types.h"

// DEFINES
// Revisions
#define FW_MAJOR_REV            0U
#define FW_MINOR_REV            0U
#define FW_TEST_REV             1U
#define HW_REVISION             2U
// BASE COLOR CH1
#define CH1_BASE_COLOR          0xD4, 0xC7, 0x19
#define CH1_BASE_COLOR_RED      0xD4
#define CH1_BASE_COLOR_GRN      0xC7
#define CH1_BASE_COLOR_BLU      0x19
// BASE COLOR CH2
#define CH2_BASE_COLOR          0x17, 0x94, 0xC2
#define CH2_BASE_COLOR_RED      0x17
#define CH2_BASE_COLOR_GRN      0x94
#define CH2_BASE_COLOR_BLU      0xC2
// DISABLE COLOR
#define DISABLE_BASE_COLOR      0x96, 0x96, 0x96
#define DISABLE_BASE_COLOR_RED  0x96
#define DISABLE_BASE_COLOR_GRN  0x96
#define DISABLE_BASE_COLOR_BLU  0x96
// ALPHA FADING
#define ALPAH_FULL_VISIBLE      0xFF
#define ALPHA_ZERO_VISIBLE      0x00


// TYPEDEFS AND ENUMS
typedef enum
{
    SPLASH_SCREEN = 0,
    MAIN_SCREEN,
    CONFIG_SCREEN,
    SET_SCREEN
}Type_ScreenType;

typedef enum
{
    CHANNEL_1 = 0,
    CHANNEL_2
}Type_Channel;

typedef enum
{
    OFF = 0,
    ON = 1
}Type_RelaySwitch;

typedef enum
{
    ONE_MEG_OHM = 0,
    FIFTY_OHM
}Type_InputImpedance;

typedef enum
{
    INTEGER = 0,
    TENTHS,
    HUNDREDTHS,
    THOUSANDTHS
}Type_SetLimitDigit;

typedef struct
{
    bool                    Enable;
    double                  Current;
}Type_SetLimits;

typedef struct
{
    Type_SetLimits          Limit;
    Type_InputImpedance     InputImpedance;
    Type_RelaySwitch        OutputSwitch;
}Type_ChannelConfig;

typedef struct
{
    Type_ScreenType     Screen;
    Type_SetLimitDigit  SetLimitDigit;
    Type_Channel        ActiveChannel;
    Type_ChannelConfig  CH1;
    Type_ChannelConfig  CH2;
}Type_ArbPwrBoosterStatus;


// EXTERNS
extern Type_ArbPwrBoosterStatus ArbPwrBoosterStatus;


// FUNCTION PROTOTYES
void Init_ArbPwrBoosterClass(void);
void Init_ArbPwrBoosterHardware(void);
void digitsFromDouble(double RationalNumber, int8_t *Integer, uint8_t *Tenths, uint8_t *Hundredths, uint8_t *Thousandths);
double digitsToDouble(int8_t *Integer, uint8_t *Tenths, uint8_t *Hundredths, uint8_t *Thousandths);

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_USER_APP_SUPPORT_INC_MAIN_SUPPORT_H_ */
