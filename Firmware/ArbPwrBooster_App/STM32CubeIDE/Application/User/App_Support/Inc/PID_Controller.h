 /*****************************************************************************************************
 * @file 			PID_Controller.h
 * @brief			This is the Header file used to support PID_Controller.c
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

#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "Hab_Types.h"

// TYPEDEFS AND ENUMS
typedef struct
{
    float   ProportionalGain;
    float   IntegralGain;
    float   DerivativeGain;
    float   PreviousError;
    float   Integral;
    float   Output;
}Type_PID_Controller;


// MACROS
#define MIN_MAX_CLAMP(Value, Min, Max)  (Value < Min)? Min : (Value > Max)? Max : Value


// FUNCTION PROTOTYPES
Type_PID_Controller * Init_PID_Controller(float Kp, float Ki, float Kd);
uint8_t PID_updateDigitalPot(Type_PID_Controller *PID, float MeasuredCurrent, float CurrentLimit, float DeltTime, uint8_t PotMaxValue);


#ifdef __cplusplus
}
#endif
#endif /* PID_CONTROLLER_H_ */
