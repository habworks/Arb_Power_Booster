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
#include "RMS_Calculate.h"
#include "stm32f7xx_hal.h"

// DEFINES
// Revisions
#define FW_MAJOR_REV            0U
#define FW_MINOR_REV            0U
#define FW_TEST_REV             3U
#define HW_REVISION             2U
// BASE COLOR CH1
#define CH1_BASE_COLOR          0xD4, 0xC7, 0x19
#define CH1_BASE_COLOR_RED      ((uint8_t)(0xD4))
#define CH1_BASE_COLOR_GRN      ((uint8_t)(0xC7))
#define CH1_BASE_COLOR_BLU      ((uint8_t)(0x19))
// BASE COLOR CH2
#define CH2_BASE_COLOR          0xFF, 0x33, 0xFF
#define CH2_BASE_COLOR_RED      ((uint8_t)(0xFF))
#define CH2_BASE_COLOR_GRN      ((uint8_t)(0x33))
#define CH2_BASE_COLOR_BLU      ((uint8_t)(0xFF))
// DISABLE COLOR
#define DISABLE_BASE_COLOR      0x96, 0x96, 0x96
#define DISABLE_BASE_COLOR_RED  ((uint8_t)(0x96))
#define DISABLE_BASE_COLOR_GRN  ((uint8_t)(0x96))
#define DISABLE_BASE_COLOR_BLU  ((uint8_t)(0x96))
// CONFIG ICON BACK COLOR
#define CONFIG_ICON_OK_COLOR    0x5B, 0xAD, 0x2F
#define CONFIG_ICON_ERROR_COLOR 0xAB, 0x14, 0x19
// ALPHA FADING
#define ALPAH_FULL_VISIBLE      ((uint8_t)(0xFF))
#define ALPHA_ZERO_VISIBLE      ((uint8_t)(0x00))
#define ALPHA_50_VISIBLE        ((uint8_t)(0x32))
// STATUS MSG TEXT COLOR
#define STATUS_MSG_OK_COLOR     0xFF, 0xFF, 0xFF
#define STATUS_MSG_ERROR_COLOR  0xFF, 0x14, 0x19
// LIMIT SCREEN SPECIFIC:
// LIMIT SCREEN DIGIT 0 BOX SETPOSITION PARAMETERS
#define INT_0_POSITION_1_DIGIT  112, 97, 31, 39
#define INT_0_POSITION_2_DIGIT  85, 97, 60, 39
#define INT_0_POSITION_3_DIGIT  66, 97, 77, 39
// LIMIT SCREEN DIGIT 0 BOX SETBOXWITHBORDERPOSITION PARAMETERS
#define INT_0_SETBOX_1_DIGIT    0, 0, 31, 39
#define INT_0_SETBOX_2_DIGIT    0, 0, 60, 39
#define INT_0_SETBOX_3_DIGIT    0, 0, 77, 39
// MAIN SCREEN OUTPUT ON / OFF BOX AND TEXT
#define OUTPUT_ON_BOX_COLOR     0x5E, 0xEB, 0x13
#define OUTPUT_ON_TXT_COLOR     0xFF, 0xFF, 0xFF
#define OUTPUT_OFF_BOX_COLOR    0xA7, 0xC4, 0xB2
#define OUTPUT_OFF_TXT_COLOR    0x70, 0x70, 0x70
// MISC
#define GUI_UPDATE_RATE         200U
#define CONFIG_FILENAME         "ArbPwrBoosterConfig.bin"


// TYPEDEFS AND ENUMS
typedef enum
{
    CONFIG_LOAD_OK = 0,
    CONFIG_CREATE_NEW,
    CONFIG_SAVE_OK,
    CONFIG_FILE_ERROR
}Type_ConfigParameterStatus;

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

typedef double (*Type_RMS_Calculate)(Type_RMS *, double);
typedef struct
{
    bool                    ResetCurrentMinMax;
    double                  MinCurrent;
    double                  MaxCurrent;
    double                  MeanCurrent;
    double                  RMS_Current;
    Type_RMS_Calculate      RMS_CurrentFunction;
    bool                    ResetVoltageMinMax;
    double                  MinVoltage;
    double                  MaxVoltage;
    double                  MeanVoltage;
    double                  RMS_Voltage;
    Type_RMS_Calculate      RMS_VoltageFunction;
}Type_ChannelMeasure;

typedef struct
{
    double                  Positive_VS;
    double                  Negative_VS;
    double                  VDD_VDREF;
    double                  TempDegreeC;
}Type_SystemMeasure;

typedef struct
{
    Type_SetLimits          Limit;
    Type_InputImpedance     InputImpedance;
    Type_RelaySwitch        OutputSwitch;
    Type_ChannelMeasure     Measure;
}Type_ChannelConfig;

typedef struct
{
    bool                Ready;
    Type_ScreenType     Screen;
    Type_SetLimitDigit  SetLimitDigit;
    Type_SystemMeasure  SystemMeasure;
    Type_Channel        ActiveChannel;
    Type_ChannelConfig  CH1;
    Type_ChannelConfig  CH2;
}Type_ArbPwrBoosterStatus;


// MACROS
#define NOT_USED(x)             (void)(x)
#define DO_NOTHING()            __NOP()
#define DO_NOTHING_X_TIMES(x)   for (uint32_t I = 0; I < x; I++) DO_NOTHING()


// EXTERNS
extern Type_ArbPwrBoosterStatus ArbPwrBooster;


// FUNCTION PROTOTYES
void Init_ArbPwrBoosterClass(void);
void Init_ArbPwrBoosterHardware(void);
void digitsFromDouble(double RationalNumber, int8_t *Integer, uint8_t *Tenths, uint8_t *Hundredths, uint8_t *Thousandths);
double digitsToDouble(int8_t *Integer, uint8_t *Tenths, uint8_t *Hundredths, uint8_t *Thousandths);
void systemErrorHandler(char *FileName, int FileLineNumber, uint32_t ErrorNumber, char *Description);
void mainUpdateTaskActions(void);
void mainUpdateTaskInit(void);
Type_ConfigParameterStatus loadConfigParameters(void);
Type_ConfigParameterStatus saveConfigParameters(void);

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_USER_APP_SUPPORT_INC_MAIN_SUPPORT_H_ */
