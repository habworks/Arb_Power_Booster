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
#include "Hab_Types.h"
#include <String.h>
#include <stdlib.h>
#include <stdio.h>


Type_ArbPwrBoosterStatus ArbPwrBoosterStatus;


//HAB TODO: A lot of work to do here - may want to return a pointer I think would be best
void Init_ArbPwrBoosterHardware(void)
{
    Init_GPIO_Hardware();
    Init_ADC_Hardware();
}


void Init_ArbPwrBoosterClass(void)
{
    ArbPwrBoosterStatus.Screen = SPLASH_SCREEN;
    // Channel 1 Init
    ArbPwrBoosterStatus.CH1.InputImpedance = ONE_MEG_OHM;
    ArbPwrBoosterStatus.CH1.Limit.Enable = false;
    ArbPwrBoosterStatus.CH1.Limit.Current = -10.25432;
    ArbPwrBoosterStatus.CH1.OutputSwitch = OFF;
    // Channel 2 Init
    ArbPwrBoosterStatus.CH2.InputImpedance = FIFTY_OHM;
    ArbPwrBoosterStatus.CH2.Limit.Enable = true;
    ArbPwrBoosterStatus.CH2.Limit.Current = 5.125;
    ArbPwrBoosterStatus.CH2.OutputSwitch = OFF;
}


void digitsFromDouble(double RationalNumber, int8_t *Integer, uint8_t *Tenths, uint8_t *Hundredths, uint8_t *Thousandths)
{
    char ValueString[10] = {0};
    char ExtractedString[3] = {0};
    char *DecimalLocation;

    sprintf(ValueString, "%2.3f", RationalNumber);
    DecimalLocation = strstr(ValueString, ".");

    // Extract Integer Part
    uint8_t StringLength = strlen(ValueString);
    for (uint8_t Index = 0; Index < StringLength; Index++)
    {
        if (ValueString[Index] == '.')
            break;
        ExtractedString[Index] = ValueString[Index];
    }
    *Integer = atoi(ExtractedString);

    // Extract Tenths
    memset(ExtractedString, 0x00, sizeof(ExtractedString));
    char *ValueLocationTenths = DecimalLocation + 1;
    ExtractedString[0] = *ValueLocationTenths;
    *Tenths = atoi(ExtractedString);

    // Extract Hundredths
    char *ValueLocationHundredths = DecimalLocation + 2;
    ExtractedString[0] = *ValueLocationHundredths;
    *Hundredths = atoi(ExtractedString);

    // Extract Thousandths
    char *ValueLocationThousandths = DecimalLocation + 3;
    ExtractedString[0] = *ValueLocationThousandths;
    *Thousandths = atoi(ExtractedString);
}


double digitsToDouble(int8_t *Integer, uint8_t *Tenths, uint8_t *Hundredths, uint8_t *Thousandths)
{
    double Value = (double)abs(*Integer);
    Value += *Tenths * 0.100;
    Value += *Hundredths * 0.010;
    Value += *Thousandths * 0.001;
    if (*Integer < 0)
        Value *= -1.0;

    return(Value);
}

