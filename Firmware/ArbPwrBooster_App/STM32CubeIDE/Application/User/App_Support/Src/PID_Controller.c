/******************************************************************************************************
 * @file 			PID_Controller.c
 * @brief			Proportional-Integral-Derivative (PID) controller functions
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
 *                    IMR Engineering
 *
 * @copyright       IMR Engineering, LLC
 ********************************************************************************************************/

#include "PID_Controller.h"
#include "Main_Support.h"
#include <stdlib.h>


/********************************************************************************************************
* @brief Init of a PID struct for use.  Allocate the memory for use with the PID, assign the gains and set
* the init conditions
*
* @author original: Hab Collector \n
*
* @note: This memory is allocated and should never be freed
*
* @param Kp: Proportional Gain
* @param Ki: Integral Gain
* @param Kd: Derivative Gain
* @param MaxStepValue: Digital Pot reset value in steps see MCP45HVX1 driver
*
* @return: Pointer to the PID Controller structure
*
* STEP 1: Allocate the memory
* STEP 2: Assign gain constants and init conditions
********************************************************************************************************/
Type_PID_Controller * Init_PID_Controller(float Kp, float Ki, float Kd, uint8_t MaxStepValue)
{
    // STEP 1: Allocate the memory
    Type_PID_Controller *PID = (Type_PID_Controller *)malloc(sizeof(Type_PID_Controller));
    if (PID == NULL)
        systemErrorHandler(__FILE__, __LINE__, 0, "Failed to allocate memory for PID");

    // STEP 2: Assign gain constants and init conditions
    PID->Enable = false;
    PID->ProportionalGain = Kp;
    PID->IntegralGain = Ki;
    PID->DerivativeGain = Kd;
    PID->PreviousError = 0;
    PID->Integral = 0;
    PID->Output = 0;
    PID->PotStep = 0;
    PID->MaxStepValue = MaxStepValue;

    return(PID);

} // END OF Init_PID_Controller



/********************************************************************************************************
* @brief Update the digital pot to control the current set point.  The digital pot is a divider to the output
* signal.  When the current limit is surpassed, the digital pot is used to limit the output voltage.  By
* limiting the output voltage, the output current is reduced.  When the output voltage is reduced to the point
* the output current matches the current limit - this is in effect constant current mode.
*
* @author original: Hab Collector \n
*
* @note: This memory is allocated and should never be freed
*
* @param PID: PID Controller Handle
* @param MeasuredCurrent: Present current value
* @param CurrentLimit: current limit allowed
* @param DeltTime: Time between current samples
*
* @return: Pot value
*
* STEP 1: Calculate the present error
* STEP 2: Take action only if Measured current is more than the limit
* STEP 3: Calculate the Integral
* STEP 4: Calculate the derivative
* STEP 5: Calculate the output by summing the P, I, D errors
* STEP 6: The output is adjusted for the pot - must be of type uint8_t and clamped to match the pot min max
********************************************************************************************************/
Type_PID_Status PID_updateDigitalPot(Type_PID_Controller *PID, float MeasuredCurrent, float CurrentLimit, float DeltaTime)
{
    // STEP 1: Calculate the present error
    float PresentError = MeasuredCurrent - CurrentLimit;

    // STEP 2: Take action only if Measured current is more than the limit
    if (PresentError <= 0)
    {
        PID->PreviousError = 0;
        PID->Integral = 0;
        return(PID_NO_UPDATE);
    }

    // PID CALCULATE:
    // STEP 3: Calculate the Integral
    PID->Integral += PresentError * DeltaTime;

    // STEP 4: Calculate the derivative
    float Derivative = (PresentError - PID->PreviousError) / DeltaTime;
    PID->PreviousError = PresentError;

    // STEP 5: Calculate the output by summing the P, I, D errors
    PID->Output = (PID->ProportionalGain * PresentError) + (PID->IntegralGain * PID->Integral) + (PID->DerivativeGain * Derivative);

    // STEP 6: The output is adjusted for the pot - must be of type uint8_t and clamped to match the pot min max
    uint8_t PotValue = (uint8_t)(PID->Output + INTEGER_ROUNDING);
    PotValue = PID->MaxStepValue - PotValue;
    PotValue = MIN_MAX_CLAMP(PotValue, 0, PID->MaxStepValue);
    PID->PotStep = PotValue;

    return(PID_UPDATE);

} // END OF PID_updateDigitalPot
