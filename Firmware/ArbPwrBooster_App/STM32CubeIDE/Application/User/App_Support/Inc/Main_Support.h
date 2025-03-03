/** ****************************************************************************************************
 * @file 			Main_Support.h
 * @brief			This is the Header file used to support Main_Support.h.c
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

#ifndef APPLICATION_USER_APP_SUPPORT_INC_MAIN_SUPPORT_H_
#define APPLICATION_USER_APP_SUPPORT_INC_MAIN_SUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

// DEFINES
// Revisions
#define FW_MAJOR_REV            0U
#define FW_MINOR_REV            0U
#define FW_TEST_REV             1U
#define HW_REVISION             2U


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

typedef struct
{
    double                  CurrentLimit;
    Type_InputImpedance     InputImpedance;
    Type_RelaySwitch        OutputSwitch;
}Type_ChannelConfig;

typedef struct
{
    Type_ScreenType     Screen;
    Type_Channel        ActiveChannel;
    Type_ChannelConfig  CH1;
    Type_ChannelConfig  CH2;
}Type_ArbPwrBoosterStatus;


// EXTERNS
extern Type_ArbPwrBoosterStatus ArbPwrBoosterStatus;


// FUNCTION PROTOTYES
void Init_ArbPwrBooster(void);

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_USER_APP_SUPPORT_INC_MAIN_SUPPORT_H_ */
