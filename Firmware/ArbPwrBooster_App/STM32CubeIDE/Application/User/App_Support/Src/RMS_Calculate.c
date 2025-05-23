/******************************************************************************************************
 * @file 			RMS_Calculate.c
 * @brief			Method for calculating RMS value
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

#include "RMS_Calculate.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>


/********************************************************************************************************
* @brief Init of a RMS struct for use.  The base memory location for the buffer is cleared and memory for
* type is allocated.  The buffer is init with 0 values on count and index
*
* @author original: Hab Collector \n
*
* @note: This memory is allocated and should never be freed
*
* @param Buffer: Pointer to memory that will hold the buffer
* @param BufferLength: The total number of uint16_t
*
* @return: Pointer to the FIFO buffer structure
*
* STEP 1: Set the buffer values to 0 and allocate memory for the struct pointer
* STEP 2: Assign reset values
********************************************************************************************************/
Type_RMS *Init_RMS_Class(float *Buffer, uint16_t BufferLength)
{
    // STEP 1: Set the buffer values to 0 and allocate memory for the struct pointer
    memset(Buffer, 0x00, (sizeof(float) * BufferLength));
    Type_RMS *RMS_Object = (Type_RMS *)malloc(sizeof(Type_RMS));
    if (RMS_Object == NULL)
        systemErrorHandler(__FILE__, __LINE__, 0, "Failed to allocate memory for RMS");

    // STEP 2 Assign reset values
    RMS_Object->Index = 0;
    RMS_Object->Count = 0;
    RMS_Object->SumOfSquares = 0;
    RMS_Object->WindowSize = BufferLength;
    RMS_Object->BufferWindow = Buffer;

    return(RMS_Object);

} // END OF Init_RMS_Class



/********************************************************************************************************
* @brief RMS calculate function.  This function refers to updating as the RMS value is based on a buffer
* window.  As the buffer is updated with new sample values the RMS output will be updated.  This is a true
* RMS calculation as it computes the square root of the mean sum of squares.  The window size must be
* adjusted - theoretically it must accommodate the slowest frequency you expect to measure.  Slower frequency
* requires a larger windows size than higher frequency.  Measuring a 10Hz (Period = .1s) signal at a sample rate of Ts samples
* per second will require a minimum Buffer Window (BW) size of BW = .1/Ts.  The more samples the better but
* it is limited by the available memory that can be allocated for windows.  The algorithm computational
* speed is BW size independent - it calculates a BW size of 64 in the same speed of one 1024 - as such larger
* windows create more accurate RMS measurements (filter out noise and anomalies better) but the limiting factor
* will be memory.
*
* @author original: Hab Collector \n
*
* @note: True RMS calculation
* @note: The RMS is calculated by the formula RMS = sqrt(1/N * Sum(x*x))
* @note: At the frequency of measure the Window size is too large and can be reduced - a project for later
* @note: TODO: Hab Adjust the buffer window size once the final sample rate speed is known
*
* @param RMS_Object: The RMS Object to be operated on
* @param NewSampleValue: New sample for which to update the RMS value
*
* @return: The RMS updated value
*
* STEP 1: Remove the oldest value from sum_squares if buffer is full
* STEP 2: Store new value and update sum of squares
* STEP 3: Calculate the updated RMS
* STEP 4: Move buffer index (circular)
********************************************************************************************************/
double update_RMS_Value(Type_RMS *RMS_Object, double NewSampleValue)
{
    // STEP 1: Remove the oldest value from sum_squares if buffer is full
    if (RMS_Object->Count >= RMS_Object->WindowSize)
        RMS_Object->SumOfSquares = RMS_Object->SumOfSquares - (RMS_Object->BufferWindow[RMS_Object->Index] * RMS_Object->BufferWindow[RMS_Object->Index]);  // Remove old squared value
    else
        RMS_Object->Count = RMS_Object->Count + 1;

    // STEP 2: Store new value and update sum of squares
    RMS_Object->BufferWindow[RMS_Object->Index] = NewSampleValue;
    RMS_Object->SumOfSquares = RMS_Object->SumOfSquares + (NewSampleValue * NewSampleValue);

    // STEP 3: Calculate the updated RMS
    double RMS_CurrentValue = sqrt(RMS_Object->SumOfSquares / RMS_Object->Count);

    // STEP 4: Move buffer index (circular)
    RMS_Object->Index = (RMS_Object->Index + 1) % RMS_Object->WindowSize;

    return(RMS_CurrentValue);

} // END OF update_RMS_Value
