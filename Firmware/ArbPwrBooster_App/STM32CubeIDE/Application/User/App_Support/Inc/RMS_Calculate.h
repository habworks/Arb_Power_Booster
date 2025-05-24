 /*****************************************************************************************************
 * @file 			RMS.h
 * @brief			This is the Header file used to support RMS_Calculate.c
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

#ifndef RMS_CALCULATE_H_
#define RMS_CALCULATE_H_
#ifdef __cplusplus
extern"C" {
#endif

#include <stdint.h>

// TYPEDEFS AND ENUMS
typedef struct
{
    uint16_t    Index;
    uint16_t    Count;
    uint16_t    WindowSize;
    double      SumOfSquares;
    float *     BufferWindow;       // Making this a float as to make it a double would require too much memory
}Type_RMS;


// FUNCTION PROTOTYPES
Type_RMS *Init_RMS_Class(float *Buffer, uint16_t BufferLength);
double update_RMS_Value(Type_RMS *RMS_Object, double NewSampleValue);

#ifdef __cplusplus
}
#endif
#endif /* RMS_CALCULATE_H_ */
