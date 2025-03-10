/** ****************************************************************************************************
 * @file 			ADC_Support.c
 * @brief			<Add a descrition of what the file does>
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

#include "ADC_Support.h"
#include "Main_Support.h"
#include "IO_Support.h"
#include "stm32f7xx_hal_adc.h"
#include <string.h>
#include <stdlib.h>

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

// Global
uint16_t ADC1_CountValue[ADC1_NUMBER_OF_CHANNELS];
uint16_t ADC3_CountValue[ADC3_NUMBER_OF_CHANNELS];
double ADC_RawConvertedResult[NUMBER_OF_CHANNELS];

// Static Declarations
static Type_16b_FIFO *Init_FIFO_Buffer(uint16_t *Buffer, uint8_t BufferLength);
static void writeTo_FIFO_Buffer(Type_16b_FIFO *FIFO_Buffer, uint16_t Value);
static double calculateSystem_3V3(void);
static double calculateSystemTemp(void);
static double calculateSystemSupply(Type_16b_FIFO *FIFO_SupplyRail);
static void calculateChannelCurrent(Type_16b_FIFO *FIFO_AmpMon, Type_ChannelMeasure *CurrentMeasure);

static double System_ADC_Reference;
// System 3V3
static uint16_t BufferSystem_3V3[SYSTEM_3V3_BUFFER_SIZE];
static Type_16b_FIFO *FIFO_System_3V3;
// System Temperature
static uint16_t BufferSystemTemp[SYSTEM_TEMP_BUFFER_SIZE];
static Type_16b_FIFO *FIFO_SystemTemp;
// CH1 Current Monitor
static uint16_t Buffer_CH1_AmpMon[CH1_AMP_MON_BUFFER_SIZE];
static Type_16b_FIFO *FIFO_CH1_AmpMon;
// CH2 Current Monitor
static uint16_t Buffer_CH2_AmpMon[CH2_AMP_MON_BUFFER_SIZE];
static Type_16b_FIFO *FIFO_CH2_AmpMon;
// Positive 20V Supply Rail Monitor
static uint16_t Buffer_P20V_Supply[SYSTEM_P20V_BUFFER_SIZE];
static Type_16b_FIFO *FIFO_P20V_Supply;
// Negative 20V Supply Rail Monitor
static uint16_t Buffer_N20V_Supply[SYSTEM_N20V_BUFFER_SIZE];
static Type_16b_FIFO *FIFO_N20V_Supply;


/********************************************************************************************************
* @brief Init the ADC for ADC1 (3 channels) and ADC3 (3 channels).  ADC1 handles:
*    CH1 Current Monitor
*    Temperature
*    3.3V measurment
* ADC2 handles:
*    -20V Supply Rail
*    +20V Supply Rail
*    CH2 Current Monitor
*
* @author original: Hab Collector \n
*
* @note: ADC is set in DMA continuous conversion mode.
* @note: See the ADC DMA callback HAL_ADC_ConvCpltCallback
* @note: The STM32F series makes no provision for a calibration as it is trimmed by the factory
*
* STEP 1: Enable Internal reference and temperature measure
* STEP 2: Start ADC1 and ADC3 in DMA continuous conversion: CH1 & 2 Amp Monitor, Micro-Temp Sensor, System 3.3V, ±20V rail
* STEP 3: Init FIFO Buffers for use with ADC1: CH1 Amp Monitor, Micro-Temp Sensor, System 3.3V
* STEP 4: Init FIFO Buffers for use with ADC3: -20V rail, +20V rail, CH2 Amp Monitor
********************************************************************************************************/
void Init_ADC_Hardware(void)
{
    // STEP 1: Enable Internal reference and temperature measure
    ADC->CCR |= ADC_CCR_TSVREFE_Msk;

    // STEP 2: Start ADC1 and ADC3 in DMA continuous conversion: CH1 & 2 Amp Monitor, Micro-Temp Sensor, System 3.3V, ±20V rail
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC1_CountValue, ADC1_NUMBER_OF_CHANNELS);
    HAL_ADC_Start_DMA(&hadc3, (uint32_t *)ADC3_CountValue, ADC3_NUMBER_OF_CHANNELS);

    // STEP 3: Init FIFO Buffers for use with ADC1: CH1 Amp Monitor, Micro-Temp Sensor, System 3.3V
    FIFO_CH1_AmpMon = Init_FIFO_Buffer(Buffer_CH1_AmpMon, CH1_AMP_MON_BUFFER_SIZE);
    FIFO_SystemTemp = Init_FIFO_Buffer(BufferSystemTemp, SYSTEM_TEMP_BUFFER_SIZE);
    FIFO_System_3V3 = Init_FIFO_Buffer(BufferSystem_3V3, SYSTEM_3V3_BUFFER_SIZE);

    // STEP 4: Init FIFO Buffers for use with ADC3: -20V rail, +20V rail, CH2 Amp Monitor
    FIFO_N20V_Supply = Init_FIFO_Buffer(Buffer_N20V_Supply, SYSTEM_N20V_BUFFER_SIZE);
    FIFO_P20V_Supply = Init_FIFO_Buffer(Buffer_P20V_Supply, SYSTEM_P20V_BUFFER_SIZE);
    FIFO_CH2_AmpMon = Init_FIFO_Buffer(Buffer_CH2_AmpMon, CH2_AMP_MON_BUFFER_SIZE);

} // END OF Init_ADC_Hardware



/********************************************************************************************************
* @brief Redefinition of the weak callback function for ADC conversion complete.  ADC1 and ADC3 values
* have been acquired this function updates the corresponding FIFO buffers and calls a unique function that
* performs the channel conversion.
*
* @author original: Hab Collector \n
*
* @note: This is redefinition - this function is define weak in the HAL but implemented here
* @note: The Rate toggle output are used to verify ADC conversion rate.  Conversion rate is calculated as:
* PCLK2 / ADC_Clock_Presclar * ConversionCycles * NumberOfChannels
*
* STEP 1: ADC1 Conversions: CH1 Amp Monitor, Micro Temp Sensor, System 3.3V
* STEP 2: ADC3 Conversions: +20V Monitor, -20V Monitor, CH2 Amp Monitor
********************************************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // STEP 1: ADC1 Conversions: CH1 Amp Monitor, Micro Temp Sensor, System 3.3V
    if (hadc == &hadc1)
    {
        // System Temperature: See section 15.10 of STM32F746 Reference Manual (RM0385)
        writeTo_FIFO_Buffer(FIFO_SystemTemp, ADC1_CountValue[RANK_1]);
        ArbPwrBooster.SystemMeasure.TempDegreeC = calculateSystemTemp();
        // System 3.3V
        writeTo_FIFO_Buffer(FIFO_System_3V3, ADC1_CountValue[RANK_2]);
        System_ADC_Reference = calculateSystem_3V3();
        ArbPwrBooster.SystemMeasure.VDD_VDREF = System_ADC_Reference;
        // Toggle at the end of conversion - for testing: ADC1 actual conversion rate VS calculated
        ADC1_C_RATE_TOGGLE();
    }

    // STEP 2: ADC3 Conversions: +20V Monitor, -20V Monitor, CH2 Amp Monitor
    if (hadc == &hadc3)
    {
        // -20V Supply Rail
        writeTo_FIFO_Buffer(FIFO_N20V_Supply, ADC3_CountValue[RANK_1]);
        ArbPwrBooster.SystemMeasure.Negative_20V = calculateSystemSupply(FIFO_N20V_Supply) * -1.0;
        ADC_RawConvertedResult[NEG_20V_MON] = ADC3_CountValue[RANK_1] * LSB_12BIT_VALUE * ADC_REFERENCE_VOLTAGE;
        // +20V Supply Rail
        writeTo_FIFO_Buffer(FIFO_P20V_Supply, ADC3_CountValue[RANK_2]);
        ArbPwrBooster.SystemMeasure.Positive_20V = calculateSystemSupply(FIFO_P20V_Supply);
        ADC_RawConvertedResult[POS_20V_MON] = ADC3_CountValue[RANK_2] * LSB_12BIT_VALUE * ADC_REFERENCE_VOLTAGE;
        // Current Monitor CH1
        writeTo_FIFO_Buffer(FIFO_CH1_AmpMon, ADC3_CountValue[RANK_3]);
        calculateChannelCurrent(FIFO_CH1_AmpMon, &ArbPwrBooster.CH1.Measure);
        ADC_RawConvertedResult[CH1_AMP_MON] = ADC3_CountValue[RANK_3] * LSB_12BIT_VALUE * ADC_REFERENCE_VOLTAGE;
        // Current Monitor CH2
        writeTo_FIFO_Buffer(FIFO_CH2_AmpMon, ADC3_CountValue[RANK_4]);
        calculateChannelCurrent(FIFO_CH2_AmpMon, &ArbPwrBooster.CH2.Measure);
        ADC_RawConvertedResult[CH2_AMP_MON] = ADC3_CountValue[RANK_4] * LSB_12BIT_VALUE * ADC_REFERENCE_VOLTAGE;

        // Toggle at the end of conversion - for testing: ADC3 actual conversion rate VS calculated
        ADC3_C_RATE_TOGGLE();
    }
} // END OF HAL_ADC_ConvCpltCallback



/********************************************************************************************************
* @brief Init of a FIFO buffer for use.  The base memory location for the buffer is cleared and memory for
* type is allocated.  The buffer is init with the write position at index 0
*
* @author original: Hab Collector \n
*
* @note: The passed buffer must be of type uint16_t
* @note: This memory is allocated and should never be freed
*
* @param Buffer: Pointer to memory that will hold the buffer
* @param BufferLength: The total number of uint16_t
*
* @return: Pointer to the FIFO buffer structure
*
* STEP 1: Set the buffer values to 0 and allocate memory for the struct pointer
* STEP 2: Assign buffer pointer, length and set write location to buffer index
********************************************************************************************************/
static Type_16b_FIFO *Init_FIFO_Buffer(uint16_t *Buffer, uint8_t BufferLength)
{
    // STEP 1: Set the buffer values to 0 and allocate memory for the struct pointer
    memset(Buffer, 0x00, (sizeof(uint16_t) * BufferLength));
    Type_16b_FIFO *FIFO_Buffer = (Type_16b_FIFO *)malloc(sizeof(Type_16b_FIFO));

    // STEP 2: Assign buffer pointer, length and set write location to buffer index
    FIFO_Buffer->Buffer = Buffer;
    FIFO_Buffer->Depth = BufferLength;
    FIFO_Buffer->WritePointer = &Buffer[0];

    return(FIFO_Buffer);

} // END OF Init_FIFO_Buffer



/********************************************************************************************************
* @brief Write a value to the FIFO buffer.  The write pointer is advance automatically.  The write pointer
* will loop back to the index to avoid memory leak.
*
* @author original: Hab Collector \n
*
* @note: See Init_FIFO_Buffer function for additional details
*
* @param Buffer: Pointer to memory that will hold the buffer
* @param Value: Value to be written
*
* STEP 1: Write Value to the present location
* STEP 2: Advance write pointer to next address - loop back when end of write locations reached
********************************************************************************************************/
static void writeTo_FIFO_Buffer(Type_16b_FIFO *FIFO_Buffer, uint16_t Value)
{
    // STEP 1: Write Value to the present location
    *FIFO_Buffer->WritePointer = Value;

    // STEP 2: Advance write pointer to next address - loop back when end of write locations reached
    uint16_t *NewWritePointer = FIFO_Buffer->WritePointer + 1;
    if (NewWritePointer >= (FIFO_Buffer->Buffer + FIFO_Buffer->Depth))
        FIFO_Buffer->WritePointer = FIFO_Buffer->Buffer;
    else
        FIFO_Buffer->WritePointer = NewWritePointer;

} // END OF writeTo_FIFO_Buffer



/********************************************************************************************************
* @brief Calculate the SYSTEM 3V3 voltage using the internal reference.  Formula is:
*    VDD = (Factory_ReferenceCalibration * 3.3) / ADC_VrefInt
* The ADC_VrefInt is mean value of a FIFO buffer which is used to establish a filtered result
*
* @author original: Hab Collector \n
*
* @note: For the STM32F746G-DISCO The Vref and VDD are the same
* @note: SYSTEM refers to quantities that impact the entire device - CHANNEL impacts on CH1 or CH2
*
* @return System 3.3V value
*
* STEP 1: Write Value to the present location
* STEP 2: Advance write pointer to next address - loop back when end of write locations reached
********************************************************************************************************/
static double calculateSystem_3V3(void)
{
    // STEP 1: Calculate the mean of the ADC 3V3 counts
    double Mean_3V3_Count = 0.0;
    for (uint8_t Index = 0; Index < FIFO_System_3V3->Depth; Index++)
    {
        Mean_3V3_Count += FIFO_System_3V3->Buffer[Index];
    }
    Mean_3V3_Count /= FIFO_System_3V3->Depth;

    // STEP 2: Using the mean value calculate the System 3V3
    uint16_t VRefInternalCal = *VREFINT_CAL_ADDR;
    double System_3V3 = (VRefInternalCal * ADC_REFERENCE_VOLTAGE) / Mean_3V3_Count;
    return(System_3V3);

} // END OF calculateSystem_3V3



/********************************************************************************************************
* @brief Calculate the SYSTEM temperature.  The formula is taken from See section 15.10 of STM32F746 Reference
* Manual (RM0385).  Makes uses of the filtered value for Vref (System_ADC_Reference).
*
* @author original: Hab Collector \n
*
* @note: For the STM32F746G-DISCO The Vref and VDD are the same
* @note: SYSTEM refers to quantities that impact the entire device - CHANNEL impacts on CH1 or CH2
* @note: According to the datasheet this is not going to be a very accurate absolute measure because of the
* temperature offset.  However it should be very good at making relative measurements to ±1.5C°.
*
* @return System Temperature in °C as measured by the STM32F746 uC.
*
* STEP 1: Calculate the mean of the ADC Temperature counts
* STEP 2: Using the mean value calculate the System Temperature in °C
********************************************************************************************************/
static double calculateSystemTemp(void)
{
    // STEP 1: Calculate the mean of the ADC Temperature counts
    double MeanTempCount = 0.0;
    for (uint8_t Index = 0; Index < FIFO_SystemTemp->Depth; Index++)
    {
        MeanTempCount += FIFO_SystemTemp->Buffer[Index];
    }
    MeanTempCount /= FIFO_SystemTemp->Depth;

    // STEP 2: Using the mean value calculate the System Temperature in °C
    double VoltageTempSensor = (MeanTempCount * System_ADC_Reference) / ADC_12BIT_FULL_COUNT;
    double Temperature = ((VoltageTempSensor - VOLTAGE_AT_25C) / AVG_TEMP_SLOPE) + TEMP_25C;
    return(Temperature);

} // END OF calculateSystemTemp



/********************************************************************************************************
* @brief Calculates the SYSTEM ± supply rail voltage.  The negative supply rail is inverted externally so it is
* read here as a positive value.  If the calling function is passing the negative supply rail FIFO it must
* also multiply the return value by -1.
*
* @author original: Hab Collector \n
*
* @note: External divider ratio of .110 (30V to 3.3V)
* @note: Divider ratio for ±20V is the same
* @note: Nominal supply voltage is 20V
* @note: SYSTEM refers to quantities that impact the entire device - CHANNEL impacts on CH1 or CH2
*
* @return As a positive number only the supply rail voltage
*
* STEP 1: Calculate the mean of the ADC Supply rail counts
* STEP 2: Using the mean value calculate supply rail according to input divider ratio
********************************************************************************************************/
static double calculateSystemSupply(Type_16b_FIFO *FIFO_SupplyRail)
{
    // STEP 1: Calculate the mean of the ADC Supply rail counts
    double MeanSupplyRailCount = 0.0;
    for (uint8_t Index = 0; Index < FIFO_SupplyRail->Depth; Index++)
    {
        MeanSupplyRailCount += FIFO_SupplyRail->Buffer[Index];
    }
    MeanSupplyRailCount /= FIFO_SupplyRail->Depth;

    // STEP 2: Using the mean value calculate supply rail according to input divider ratio
    double SupplyRailVoltage = MeanSupplyRailCount * LSB_12BIT_VALUE * System_ADC_Reference * DIVIDER_20V_CONVERSION;
    return(SupplyRailVoltage);

} // END OF calculateSystemSupply



/********************************************************************************************************
* @brief Calculates the CHANNEL current.  TODO ADD MORE NOTES HERE
*
* @author original: Hab Collector \n
*
* @note: TODO MORE NOTES
* @note: TODO MORE NOTES
* @note: SYSTEM refers to quantities that impact the entire device - CHANNEL impacts on CH1 or CH2
*
* @return As a positive number only the supply rail voltage
*
* STEP 1:
* STEP 2:
********************************************************************************************************/
static void calculateChannelCurrent(Type_16b_FIFO *FIFO_AmpMon, Type_ChannelMeasure *CurrentMeasure)
{
    // STEP 1: Calculate the mean of the Current Monitor
    double MeanCurrentCount = 0.0;
    for (uint8_t Index = 0; Index < FIFO_AmpMon->Depth; Index++)
    {
        MeanCurrentCount += FIFO_AmpMon->Buffer[Index];
    }
    MeanCurrentCount /= FIFO_AmpMon->Depth;

    // STEP 2: Calculate the current
    double PresentCurrentValue = ((MeanCurrentCount * LSB_12BIT_VALUE * System_ADC_Reference) / AMP_MONITOR_GAIN) / AMP_SENSE_RESISTOR;
    CurrentMeasure->MeanCurrent.Mean = PresentCurrentValue;

    // STEP 3: Update Min current - If Min set to zero overwrite with present value
    if (CurrentMeasure->MinCurrent == 0)
        CurrentMeasure->MinCurrent = PresentCurrentValue;
    else if (PresentCurrentValue < CurrentMeasure->MinCurrent)
        CurrentMeasure->MinCurrent = PresentCurrentValue;

    // STEP 4: Update Max current - if Max set to zero overwrite with present value
    if (CurrentMeasure->MaxCurrent == 0)
        CurrentMeasure->MaxCurrent = PresentCurrentValue;
    else if (PresentCurrentValue > CurrentMeasure->MaxCurrent)
        CurrentMeasure->MaxCurrent = PresentCurrentValue;


}
