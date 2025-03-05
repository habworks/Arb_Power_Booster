/** ****************************************************************************************************
 * @file 			Main_Support.c
 * @brief			Functions that support the Arbitrary Power Booster Hardware
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
#include "Main_Support.h"
#include "ADC_Support.h"


Type_ArbPwrBoosterStatus ArbPwrBoosterStatus;


//HAB TODO: A lot of work to do here - may want to return a pointer I think would be best
void Init_ArbPwrBoosterHardware(void)
{
    Init_ADC_Hardware();
}



void Init_ArbPwrBooster(void)
{
    ArbPwrBoosterStatus.Screen = SPLASH_SCREEN;
    // Channel 1 Init
    ArbPwrBoosterStatus.CH1.InputImpedance = ONE_MEG_OHM;
    ArbPwrBoosterStatus.CH1.CurrentLimit = 1.25432;
    ArbPwrBoosterStatus.CH1.OutputSwitch = OFF;
    // Channel 2 Init
    ArbPwrBoosterStatus.CH2.InputImpedance = FIFTY_OHM;
    ArbPwrBoosterStatus.CH2.CurrentLimit = 12.125;
    ArbPwrBoosterStatus.CH2.OutputSwitch = OFF;
}

