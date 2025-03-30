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
#include "IO_Support.h"
#include "UART_Support.h"
#include "Debug_Port.h"
#include "Terminal_Emulator_Support.h"
#include "MCP45HVX1_Driver.h"
#include "cmsis_os2.h"
#include <String.h>
#include <stdlib.h>
#include <stdio.h>

extern osSemaphoreId_t DisplayUpdateSemaphoreHandle;

// GLOBALS
Type_ArbPwrBoosterStatus ArbPwrBooster;



/********************************************************************************************************
* @brief Init of the Arbitrary Power Booster hardware peripherals and drivers for use in POR state
*
* @author original: Hab Collector \n
*
* STEP 1: Init hardware peripherals
* STEP 2: Init drivers
********************************************************************************************************/
void Init_ArbPwrBoosterHardware(void)
{
    // STEP 1: Init hardware peripherals
    Init_GPIO_Hardware();
    Init_ADC_Hardware();
    Init_UART_Hardware();

    // STEP 2: Init drivers
    Init_MCP45HVX1();

} // END OF Init_ArbPwrBoosterHardware



/********************************************************************************************************
* @brief Init of the Arbitrary Power Booster class - set member values to defaults for POR operation
*
* @author original: Hab Collector \n
*
* STEP 1: Set members to default POR value
********************************************************************************************************/
void Init_ArbPwrBoosterClass(void)
{
    // STEP 1: Set members to default POR value
    ArbPwrBooster.ActiveChannel = CHANNEL_1;
    ArbPwrBooster.Screen = SPLASH_SCREEN;
    // Channel 1 Init
    ArbPwrBooster.CH1.InputImpedance = ONE_MEG_OHM;
    ArbPwrBooster.CH1.OutputSwitch = OFF;
    ArbPwrBooster.CH1.Limit.Enable = false;
    ArbPwrBooster.CH1.Limit.Current = -10.25432;
    ArbPwrBooster.CH1.Measure.MaxCurrent = 0;
    ArbPwrBooster.CH1.Measure.MinCurrent = 0;
    ArbPwrBooster.CH1.Measure.RMS_UpdateFunctionPointer = update_CH1_RMS;
    // Channel 2 Init
    ArbPwrBooster.CH2.InputImpedance = ONE_MEG_OHM;
    ArbPwrBooster.CH2.OutputSwitch = OFF;
    ArbPwrBooster.CH2.Limit.Enable = true;
    ArbPwrBooster.CH2.Limit.Current = 5.125;
    ArbPwrBooster.CH2.Measure.MaxCurrent = 0;
    ArbPwrBooster.CH2.Measure.MinCurrent = 0;
    ArbPwrBooster.CH2.Measure.RMS_UpdateFunctionPointer = update_CH2_RMS;

} // END OF Init_ArbPwrBoosterClass



/********************************************************************************************************
* @brief Convert the a rational number in the form of A.xyz into its unique components of A, x, y, z
*   A = Integer
*   x = Tenths
*   y = Hundredths
*   z = Thousandths
* Example Rational number 3.1429 becomes A = 3, x = 1, y = 4, z = 3
*
* @author original: Hab Collector \n
*
* @note: Values are rounded up to the nearest thousandths
* @note: Integer, Tenths, Hundredths, Thousandths are returned by reference
*
* @param RationalNumber: Value to be converted
* @param Integer: Said place holder within a rational number
* @param Tenths: Said place holder within a rational number
* @param Hundredths: Said place holder within a rational number
* @param Thousandths: Said place holder within a rational number
*
* STEP 1: Locate the decimal by pointer
* STEP 2: Extract Integer Part
* STEP 3: Extract Tenths
* STEP 4: Extract Hundredths
* STEP 5: Extract Thousandths
********************************************************************************************************/
void digitsFromDouble(double RationalNumber, int8_t *Integer, uint8_t *Tenths, uint8_t *Hundredths, uint8_t *Thousandths)
{
    char ValueString[10] = {0};
    char ExtractedString[3] = {0};
    char *DecimalLocation;

    // STEP 1: Locate the decimal by pointer
    sprintf(ValueString, "%2.3f", RationalNumber);
    DecimalLocation = strstr(ValueString, ".");

    // STEP 2: Extract Integer Part
    uint8_t StringLength = strlen(ValueString);
    for (uint8_t Index = 0; Index < StringLength; Index++)
    {
        if (ValueString[Index] == '.')
            break;
        ExtractedString[Index] = ValueString[Index];
    }
    *Integer = atoi(ExtractedString);

    // STEP 3: Extract Tenths
    memset(ExtractedString, 0x00, sizeof(ExtractedString));
    char *ValueLocationTenths = DecimalLocation + 1;
    ExtractedString[0] = *ValueLocationTenths;
    *Tenths = atoi(ExtractedString);

    // STEP 4: Extract Hundredths
    char *ValueLocationHundredths = DecimalLocation + 2;
    ExtractedString[0] = *ValueLocationHundredths;
    *Hundredths = atoi(ExtractedString);

    // STEP 5: Extract Thousandths
    char *ValueLocationThousandths = DecimalLocation + 3;
    ExtractedString[0] = *ValueLocationThousandths;
    *Thousandths = atoi(ExtractedString);

} // END OF digitsFromDouble



/********************************************************************************************************
* @brief Convert the integer values of A, x, y, z to a rational number of value A.xyz where
*   A = Integer
*   x = Tenths
*   y = Hundredths
*   z = Thousandths
* Example digits A = 3, x = 1, y = 4, z = 3 becomes rational number 3.1429
* @author original: Hab Collector \n
*
* @param Integer: Said place holder within a rational number
* @param Tenths: Said place holder within a rational number
* @param Hundredths: Said place holder within a rational number
* @param Thousandths: Said place holder within a rational number
*
* @return Rational Number
*
* STEP 1: Concatenate values into a rational number
********************************************************************************************************/
double digitsToDouble(int8_t *Integer, uint8_t *Tenths, uint8_t *Hundredths, uint8_t *Thousandths)
{
    // STEP 1: Concatenate values into a rational number
    double Value = (double)abs(*Integer);
    Value += *Tenths * 0.100;
    Value += *Hundredths * 0.010;
    Value += *Thousandths * 0.001;
    if (*Integer < 0)
        Value *= -1.0;

    return(Value);

} // END OF digitsToDouble



/********************************************************************************************************
* @brief Error handler for critical errors ONLY.  A critical error has no recovery.  The error handler is
* used to trap the system so the call stack and offending condition can be evaluated.
*
* @author original: Hab Collector \n
*
* @param FileName: Filename where error occur - calling function must use __FILE__
* @param FileLineNumber: Line number within in the file were the error occurred - call function must use __LINE__
* @param ErrorNumber: An number value representing some condition about the error
* @param Description: A brief description of the error - do not include \r\n
*
* STEP 1: Print out error information
* STEP 2: Trap here forever
********************************************************************************************************/
void systemErrorHandler(char *FileName, int FileLineNumber, uint32_t ErrorNumber, char *Description)
{
    bool WaitHere = true;

    // STEP 1: Print out error information
//    printf("CRITICAL ERROR:");
//    printf("\tError Number:     %d\r\n", ErrorNumber);
//    printf("\tDescription:      %s\r\n", Description);
//    printf("\tFile Name:        %s\r\n". FileName);
//    printf("\tFile Line Number: %d", FileLineNumber);

    // STEP 2: Trap here forever
    while(WaitHere);

} // END OF systemErrorHandler



/********************************************************************************************************
* @brief These are the actions taken by thread mainUpdateTask().  Actions are primarily related to live
* updates of the Main and Config screens.  A secondary function of re-staring the non-contineous DMA ADC1
* is also performed.
*
* @author original: Hab Collector \n
*
* STEP 1: Restart ADC1 DMA conversion
* STEP 2: Set semaphore to update the active display
* STEP 3: Make task inactive for a period of time - based on desired GUI update speed
********************************************************************************************************/
void mainUpdateTaskActions(void)
{
    // STEP 1: Restart ADC1 DMA conversion
    ADC1_StartConversion();

    // STEP 2: Set semaphore to update the active display
    if ((ArbPwrBooster.Screen == MAIN_SCREEN) || (ArbPwrBooster.Screen == CONFIG_SCREEN))
        osSemaphoreRelease(DisplayUpdateSemaphoreHandle);

    // STEP 3: Make task inactive for a period of time - based on desired GUI update speed
    osDelay(GUI_UPDATE_RATE);

} // END OF mainUpdateTaskActions



/********************************************************************************************************
* @brief Pre-actions to be performed before mainUpdateTaskActions - These actions are performed only once.
*
* @author original: Hab Collector \n
*
* STEP 1: Clear the debug port terminal and display entry information
********************************************************************************************************/
void mainUpdateTaskInit(void)
{
    // STEP 1: Clear the debug port terminal and display entry information
    terminal_ClearScreen();
    printGreen("IMR Engineering, LLC\r\n");
    printf("  Hab Collector, Principal Engineer\r\n");
    printf("  www.imrengineering.com\r\n");
    printBrightRed("Arbitrary Power Booster\r\n");
    printf("  HW REV: %d\r\n", HW_REVISION);
    printf("  FW REV: %d.%d.%d\r\n\n", FW_MAJOR_REV, FW_MINOR_REV, FW_TEST_REV);
    printf("For assistance type Help or just ?\r\n");
    commandPrompt();
    fflush(stdout);

} // END OF mainUpdateTaskInit



