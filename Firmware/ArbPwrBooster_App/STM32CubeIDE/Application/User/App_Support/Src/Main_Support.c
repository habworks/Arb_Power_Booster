/** ****************************************************************************************************
 * @file 			Main_Support.c
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
 ********************************************************************************************************/
#include "Main_Support.h"

Type_ArbPwrBoosterStatus ArbPwrBoosterStatus;


//HAB TODO: A lot of work to do here - may want to return a pointer I think would be best
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
