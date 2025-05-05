/** ****************************************************************************************************
 * @file            ADC_Support.c
 * @brief           A collection of functions relevant to the ADC peripheral channels and their conversions
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

#include "ADC_Support.h"
#include "Main_Support.h"
#include "IO_Support.h"
#include "Terminal_Emulator_Support.h"
#include "MCP45HVX1_Driver.h"
#include "RMS_Calculate.h"
#include "PID_Controller.h"
#include "stm32f7xx_hal_adc.h"
#include "cmsis_os2.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

// Global
uint16_t ADC1_CountValue[ADC1_NUMBER_OF_CHANNELS];
uint16_t ADC3_CountValue[ADC3_NUMBER_OF_CHANNELS];
static double System_ADC_Reference = ADC_REFERENCE_VOLTAGE;     // Init condition - it will be updated later in calculation of ADC1 3V3

// Static Function Declarations
static Type_16b_FIFO *Init_FIFO_Buffer(uint16_t *Buffer, uint8_t BufferLength, uint16_t InitLoadValue);
static void writeTo_FIFO_Buffer(Type_16b_FIFO *Buffer, uint16_t Value);
static double calculateSystem_3V3(void);
static double calculateSystemTemp(void);
static double calculateSystemSupply(Type_16b_FIFO *FIFO_SupplyRail);
static void calculateChannelCurrent(double ADC_AmpMonitorCount, Type_16b_FIFO *FIFO_AmpMon, Type_ChannelConfig *ChannelMeasure, Type_RMS *RMS_AmpMon);
static void calculateChannelVoltage(double ADC_VoltMonitorCount, Type_16b_FIFO *FIFO_VoltMon, Type_ChannelMeasure *VoltageMeasure, Type_RMS *RMS_VoltMon);

// Static FIFO Declarations
// System 3V3
static uint16_t BufferSystem_3V3[SYSTEM_3V3_BUFFER_SIZE] = {0x0000};
static Type_16b_FIFO *FIFO_System_3V3;
// System Temperature
static uint16_t BufferSystemTemp[SYSTEM_TEMP_BUFFER_SIZE] = {0x0000};
static Type_16b_FIFO *FIFO_SystemTemp;
// Positive VS Supply Rail Monitor
static uint16_t Buffer_PVS_Supply[SYSTEM_PVS_BUFFER_SIZE] = {0x0000};
static Type_16b_FIFO *FIFO_PVS_Supply;
// Negative VS Supply Rail Monitor
static uint16_t Buffer_NVS_Supply[SYSTEM_NVS_BUFFER_SIZE] = {0x0000};
static Type_16b_FIFO *FIFO_NVS_Supply;
// CH1 Current Monitor
static uint16_t Buffer_CH1_AmpMon[CH1_AMP_MON_BUFFER_SIZE] = {0x0000};
static Type_16b_FIFO *FIFO_CH1_AmpMon;
// CH2 Current Monitor
static uint16_t Buffer_CH2_AmpMon[CH2_AMP_MON_BUFFER_SIZE] = {0x0000};
static Type_16b_FIFO *FIFO_CH2_AmpMon;
// CH1 Current Monitor
static uint16_t Buffer_CH1_VoltMon[CH1_VOLT_MON_BUFFER_SIZE] = {0x0000};
static Type_16b_FIFO *FIFO_CH1_VoltMon;
// CH2 Current Monitor
static uint16_t Buffer_CH2_VoltMon[CH2_VOLT_MON_BUFFER_SIZE] = {0x0000};
static Type_16b_FIFO *FIFO_CH2_VoltMon;

// Static RMS Struct Declarations
static float CH1_AmpMonBuffer[RMS_WINDOW_SIZE];
static Type_RMS *RMS_CH1_AmpMon;
static float CH2_AmpMonBuffer[RMS_WINDOW_SIZE];
static Type_RMS *RMS_CH2_AmpMon;
static float CH1_VoltMonBuffer[RMS_WINDOW_SIZE];
static Type_RMS *RMS_CH1_VoltMon;
static float CH2_VoltMonBuffer[RMS_WINDOW_SIZE];
static Type_RMS *RMS_CH2_VoltMon;

// Static PID Controller Declarations
static Type_PID_Controller *PID_CH1_DigitalPot;


/********************************************************************************************************
* @brief Init the ADC for ADC1 (2 channels) and ADC3 (3 channels).
* ADC1 handles:
*    Temperature
*    3.3V measurement
* ADC3 handles:
*    -VS Supply Rail
*    +VS Supply Rail
*    CH1 Current Monitor
*    CH2 Current Monitor
*    CH1 Voltage Monitor
*    CH2 Voltage Monitor
*
* @author original: Hab Collector \n
*
* @note: ADC1 is set in DMA single shot conversion mode - software start required for new conversion
* @note: ADC3 is set in DMA continuous conversion mode - continuous non-stop conversions once started
* @note: See the ADC DMA callback HAL_ADC_ConvCpltCallback
* @note: The STM32F series makes no provision for a calibration as it is trimmed by the factory
* @note: See function ADC13_StartConversion() - which actually starts the conversion
* @note: See also ArbPwrBooster member Ready
*
* STEP 1: Enable Internal reference and temperature measure
* STEP 2: Init FIFO Buffers for use with ADC1: Micro-Temp Sensor, System 3.3V
* STEP 3: Init FIFO Buffers for use with ADC3: ±VS rail, CH1 & CH2 Amp Monitor
* STEP 4: Init RMS structures
********************************************************************************************************/
void Init_ADC_Hardware(void)
{
    // STEP 1: Enable Internal reference and temperature measure
    ADC->CCR |= ADC_CCR_TSVREFE_Msk;

    // STEP 2: Init FIFO Buffers for use with ADC1: Micro-Temp Sensor, System 3.3V
    FIFO_SystemTemp = Init_FIFO_Buffer(BufferSystemTemp, SYSTEM_TEMP_BUFFER_SIZE, 0);
    FIFO_System_3V3 = Init_FIFO_Buffer(BufferSystem_3V3, SYSTEM_3V3_BUFFER_SIZE, ADC_REF_VOLTAGE_COUNT);

    // STEP 3: Init FIFO Buffers for use with ADC3: ±VS rail, CH1 & CH2 Amp Monitor
    FIFO_NVS_Supply = Init_FIFO_Buffer(Buffer_NVS_Supply, SYSTEM_NVS_BUFFER_SIZE, 0);
    FIFO_PVS_Supply = Init_FIFO_Buffer(Buffer_PVS_Supply, SYSTEM_PVS_BUFFER_SIZE, 0);
    FIFO_CH1_AmpMon = Init_FIFO_Buffer(Buffer_CH1_AmpMon, CH1_AMP_MON_BUFFER_SIZE, 0);
    FIFO_CH2_AmpMon = Init_FIFO_Buffer(Buffer_CH2_AmpMon, CH2_AMP_MON_BUFFER_SIZE, 0);
    FIFO_CH1_VoltMon = Init_FIFO_Buffer(Buffer_CH1_VoltMon, CH1_VOLT_MON_BUFFER_SIZE, 0);
    FIFO_CH2_VoltMon = Init_FIFO_Buffer(Buffer_CH2_VoltMon, CH2_VOLT_MON_BUFFER_SIZE, 0);

    // STEP 4: Init RMS structures
    RMS_CH1_AmpMon = Init_RMS_Class(CH1_AmpMonBuffer, RMS_WINDOW_SIZE);
    RMS_CH2_AmpMon = Init_RMS_Class(CH2_AmpMonBuffer, RMS_WINDOW_SIZE);
    RMS_CH1_VoltMon = Init_RMS_Class(CH1_VoltMonBuffer, RMS_WINDOW_SIZE);
    RMS_CH2_VoltMon = Init_RMS_Class(CH2_VoltMonBuffer, RMS_WINDOW_SIZE);

    // STEP 5: Init PID structures
    PID_CH1_DigitalPot = Init_PID_Controller(100, 100, 0);

} // END OF Init_ADC_Hardware



/********************************************************************************************************
* @brief Start Conversions for both ADC1 and ADC3.  Note this function should be call only once as part of
* the start up process.  Though ADC1 is configured in single shot mode and the start can be called on the
* completion of every ADC1 conversion, ADC3 is configured in circular mode and does not (should not) be called
* repeatedly as the conversions will automatically restart.
*
* @author original: Hab Collector \n
*
* @note: ADC1 is configured in single shot mode and its conversion rate is based on the rate at which it is called
* @note: ADC3 is configured in circular mode and will automatically restart the conversions
*
* STEP 1: Start ADC 1 & ADC 3 conversion all channels
********************************************************************************************************/
void ADC13_StartConversion(void)
{
    HAL_StatusTypeDef ADC_InitStatus = HAL_OK;

    // STEP 1: Start ADC 1 & ADC 3 conversion all channels
    ADC_InitStatus |= HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC1_CountValue, ADC1_NUMBER_OF_CHANNELS);
    ADC_InitStatus |= HAL_ADC_Start_DMA(&hadc3, (uint32_t *)ADC3_CountValue, ADC3_NUMBER_OF_CHANNELS);
    if (ADC_InitStatus != HAL_OK)
        systemErrorHandler(__FILE__, __LINE__, 0, "Failed to init ADC peripheral");

} // END OF ADC13_StartConversion



/********************************************************************************************************
* @brief Redefinition of the weak callback function for ADC conversion complete.  ADC1 and ADC3 values
* have been acquired this function updates the corresponding FIFO buffers and calls a unique function that
* performs the channel conversion.
*
* ADC1 handles:
*    Temperature
*    3.3V measurement
* ADC3 handles:
*    -VS Supply Rail
*    +VS Supply Rail
*    CH1 Current Monitor
*    CH2 Current Monitor
*    CH1 Voltage Monitor
*    CH2 Voltage Monitor
*
* ADC1 Configuration
* RANK  ADC_CH  Port    Cycles  Net Name
* 1     Temp    NA      480     NA - Internal to IC
* 2     Vref    NA      480     NA - Internal to IC
*
* ADC3 Configuration:
* RANK  ADC_CH  Port    Cycles  Net Name
* 1     6       PF8     144      -VS_MON
* 2     7       PF9     144      +VS-MON
* 3     0       PA0     144     IO_MON_2
* 4     8       PF10    144     IO_MON_1
* 5     4       PF6     144     VI_MON_2
* 6     5       PF7     144     VI_MON_1
*
* ADC uses PCLK2 (108MHz) with a divisor of 8
*
* @author original: Hab Collector \n
*
* @note: This is redefinition - this function is define weak in the HAL but implemented here
* @note: The Rate toggle outputs are used to verify ADC conversion rate.  Conversion rate is calculated as:
* PCLK2 / ADC_Clock_Presclar * Total Conversion Cycles For All ADC_Channels
* @note: ADC3 conversion time = 64.00us or 15.63KHz (This is the sampling frequency)
* @note: ADC1 conversion time = 71.11us but the sample rate is governed by mainUpdateTaskActions() / GUI_UPDATE_RATE
* @note: As the sampling frequency is 15.63KHz the max frequency of the input signal (nyquist) 7.813KHz
* @note: The sampling frequency was limited by the Touch GFX - As this computation must take place periodically and
* outside of a task, faster sampling frequencies (for example 20KHz) could not be sustained without crashing the TouchGFX
* That said there is some room to possibly tweak this a little higher.  If only the channel current is being
* calculated for RMS you can definitely run at 20KHz sampling.  However the 15.63KHz sample is adjusted for
* both channels current and voltage monitoring RMS.
*
* @param hadc: Pointer to the ADC handler
*
* STEP 1: ADC1 Conversions: Micro-Temp Sensor, System 3.3V
* STEP 2: ADC3 Conversions: ±VS rail, CH1 & CH2 Amp Monitor
********************************************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // STEP 1: ADC1 Conversions: Micro-Temp Sensor, System 3.3V
    if (hadc == &hadc1)
    {
        // System Temperature: See section 15.10 of STM32F746 Reference Manual (RM0385)
        writeTo_FIFO_Buffer(FIFO_SystemTemp, ADC1_CountValue[RANK_1]);
        ArbPwrBooster.SystemMeasure.TempDegreeC = calculateSystemTemp();
        // System 3.3V
        writeTo_FIFO_Buffer(FIFO_System_3V3, ADC1_CountValue[RANK_2]);
        System_ADC_Reference = calculateSystem_3V3();
        ArbPwrBooster.SystemMeasure.VDD_VDREF = System_ADC_Reference;
        // Stop the one shot ADC - it must be started
        HAL_ADC_Stop_DMA(&hadc1);
        // Toggle at the end of conversion - for testing: ADC1 actual conversion rate versus calculated
        ADC1_C_RATE_TOGGLE();
    }

    // STEP 2: ADC3 Conversions: ±VS rail, CH1 & CH2 Amp Monitor
    if (hadc == &hadc3)
    {
        // -VS Supply Rail
        writeTo_FIFO_Buffer(FIFO_NVS_Supply, ADC3_CountValue[RANK_1]);
        ArbPwrBooster.SystemMeasure.Negative_VS = calculateSystemSupply(FIFO_NVS_Supply) * -1.0;
        // +VS Supply Rail
        writeTo_FIFO_Buffer(FIFO_PVS_Supply, ADC3_CountValue[RANK_2]);
        ArbPwrBooster.SystemMeasure.Positive_VS = calculateSystemSupply(FIFO_PVS_Supply);
        // Current Monitor CH1
        writeTo_FIFO_Buffer(FIFO_CH1_AmpMon, ADC3_CountValue[RANK_4]);
        calculateChannelCurrent(ADC3_CountValue[RANK_4], FIFO_CH1_AmpMon, &ArbPwrBooster.CH1, RMS_CH1_AmpMon);
        // Current Monitor CH2
        writeTo_FIFO_Buffer(FIFO_CH2_AmpMon, ADC3_CountValue[RANK_3]);
        calculateChannelCurrent(ADC3_CountValue[RANK_3], FIFO_CH2_AmpMon, &ArbPwrBooster.CH2, RMS_CH2_AmpMon);
        // Voltage Monitor CH1
        writeTo_FIFO_Buffer(FIFO_CH1_VoltMon, ADC3_CountValue[RANK_6]);
        calculateChannelVoltage(ADC3_CountValue[RANK_6], FIFO_CH1_VoltMon, &ArbPwrBooster.CH1.Measure, RMS_CH1_VoltMon);
        // Voltage Monitor CH2
        writeTo_FIFO_Buffer(FIFO_CH2_VoltMon, ADC3_CountValue[RANK_5]);
        calculateChannelVoltage(ADC3_CountValue[RANK_5], FIFO_CH2_VoltMon, &ArbPwrBooster.CH2.Measure, RMS_CH2_VoltMon);
        // Toggle at the end of conversion - for testing: ADC3 actual conversion rate versus calculated
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
* @param InitLoadValue: Init value of the buffer
*
* @return: Pointer to the FIFO buffer structure
*
* STEP 1: Set the buffer values to 0 and allocate memory for the struct pointer
* STEP 2: Assign buffer pointer, length and set write location to buffer index
* STEP 3: Load the buffer with the init value
********************************************************************************************************/
static Type_16b_FIFO *Init_FIFO_Buffer(uint16_t *Buffer, uint8_t BufferLength, uint16_t InitLoadValue)
{
    // STEP 1: Set the buffer values to 0 and allocate memory for the struct pointer
    memset(Buffer, 0x00, (sizeof(uint16_t) * BufferLength));
    Type_16b_FIFO *FIFO_16Bit = (Type_16b_FIFO *)malloc(sizeof(Type_16b_FIFO));
    if (FIFO_16Bit == NULL)
        systemErrorHandler(__FILE__, __LINE__, 0, "Failed to allocate memory for FIFO");

    // STEP 2: Assign buffer pointer, length and set write location to buffer index
    FIFO_16Bit->Buffer = Buffer;
    FIFO_16Bit->Depth = BufferLength;
    FIFO_16Bit->WritePointer = &Buffer[0];
    FIFO_16Bit->Sum = 0;
    FIFO_16Bit->FirstOverFlow = false;

    // STEP 3: Load the buffer with the init value
    for (uint8_t Index = 0; Index < BufferLength; Index++)
    {
        writeTo_FIFO_Buffer(FIFO_16Bit, InitLoadValue);
    }

    return(FIFO_16Bit);

} // END OF Init_FIFO_Buffer



/********************************************************************************************************
* @brief Write a value to the FIFO buffer.  The write pointer is advance automatically.  The write pointer
* will loop back to the index to avoid memory leak.  A sum is calculated by means of adding the newest and
* removing the previous value - quicker than incrementally summing buffer values externally.
*
* @author original: Hab Collector \n
*
* @note: See Init_FIFO_Buffer function for additional details
* @note: Sum and depth members can be used for a quick external calculation of mean
*
* @param FIFO: Pointer to the 16b FIFO
* @param NewValue: Value to be written into FIFO - replaces oldest value
*
* STEP 1: Calculate buffer sum
* STEP 2: Write Value to the present location
* STEP 3: Advance write pointer to next address - loop back when end of write locations reached
********************************************************************************************************/
static void writeTo_FIFO_Buffer(Type_16b_FIFO *FIFO, uint16_t NewValue)
{
    // STEP 1: Calculate buffer sum
    if (!FIFO->FirstOverFlow)
    {
        FIFO->Sum = FIFO->Sum + NewValue;
    }
    else
    {
        uint16_t LastValue = *FIFO->WritePointer;
        FIFO->Sum = FIFO->Sum + NewValue;
        FIFO->Sum = FIFO->Sum - LastValue;
    }

    // STEP 2: Write Value to the present location
    *FIFO->WritePointer = NewValue;

    // STEP 3: Advance write pointer to next address - loop back when end of write locations reached
    uint16_t *NewWritePointer = FIFO->WritePointer + 1;
    if (NewWritePointer >= (FIFO->Buffer + FIFO->Depth))
    {
        FIFO->WritePointer = FIFO->Buffer;
        FIFO->FirstOverFlow = true;
    }
    else
        FIFO->WritePointer = NewWritePointer;

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
    Mean_3V3_Count = (double)((double)FIFO_System_3V3->Sum / FIFO_System_3V3->Depth);

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
    MeanTempCount = (double)((double)FIFO_SystemTemp->Sum / FIFO_SystemTemp->Depth);

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
* @note: Divider ratio for ±VS is the same
* @note: Nominal supply voltage is 15V
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
    MeanSupplyRailCount = (double)((double)FIFO_SupplyRail->Sum / FIFO_SupplyRail->Depth);

    // STEP 2: Using the mean value calculate supply rail according to input divider ratio
    double SupplyRailVoltage = MeanSupplyRailCount * LSB_12BIT_VALUE * System_ADC_Reference * SYSTEM_VS_DIVIDER;
    return(SupplyRailVoltage);

} // END OF calculateSystemSupply



/********************************************************************************************************
* @brief Calculates the CHANNEL mean, max, min and RMS current.
*
* @author original: Hab Collector \n
*
* @note: TODO MORE NOTES
* @note: TODO MORE NOTES
* @note: SYSTEM refers to quantities that impact the entire device - CHANNEL impacts on CH1 or CH2
*
* @param ADC_AmpMonitorCount: ADC Count from ADC channel for the specific current monitor
* @param FIFO_AmpMon: FIFO structure associated with the specific Amp Monitor channel
* @param ChannelMeasure: ArbPwrBooster current measure struct associated with the specific channel
* @param RMS_AmpMon: Associated RMS structure
*
* STEP 1: Calculate the mean of the Current Monitor
* STEP 2: Calculate the instantaneous current
* STEP 3: Update Min current or reset min if so desired
* STEP 4: Update Max current or reset max if so desired
* STEP 5: Calculate the RMS current
********************************************************************************************************/
static void calculateChannelCurrent(double ADC_AmpMonitorCount, Type_16b_FIFO *FIFO_AmpMon, Type_ChannelConfig *ChannelMeasure, Type_RMS *RMS_AmpMon)
{
    double AmpMonitorCurrent = 0;
    ChannelMeasure->Measure.MeanCurrent = 0;
    ChannelMeasure->Measure.RMS_Current = 0;
    if (ChannelMeasure->OutputSwitch == ON)
    {
        // STEP 1: Calculate the mean of the Current Monitor
        double MeanCount_ADC = (double)FIFO_AmpMon->Sum / FIFO_AmpMon->Depth;
        double MeanDiffVolt = ((MeanCount_ADC * LSB_12BIT_VALUE * System_ADC_Reference) - (System_ADC_Reference / 2.0)) / AMP_MONITOR_GAIN;
        ChannelMeasure->Measure.MeanCurrent = MeanDiffVolt / AMP_SENSE_RESISTOR;

        // STEP 2: Calculate the instantaneous current
        double DiffVolt = ((ADC_AmpMonitorCount * LSB_12BIT_VALUE * System_ADC_Reference) - (System_ADC_Reference / 2.0)) / AMP_MONITOR_GAIN;
        AmpMonitorCurrent = DiffVolt / AMP_SENSE_RESISTOR;
    }
    else
    {
        return;
    }

    // STEP 3: Update Min current or reset min if so desired
    if (ChannelMeasure->Measure.ResetCurrentMinMax)
        ChannelMeasure->Measure.MinCurrent = AmpMonitorCurrent;
    else if (AmpMonitorCurrent < ChannelMeasure->Measure.MinCurrent)
        ChannelMeasure->Measure.MinCurrent = AmpMonitorCurrent;

    // STEP 4: Update Max current or reset max if so desired
    if (ChannelMeasure->Measure.ResetCurrentMinMax)
    {
        ChannelMeasure->Measure.MaxCurrent = AmpMonitorCurrent;
        ChannelMeasure->Measure.ResetCurrentMinMax = false;
    }
    else if (AmpMonitorCurrent > ChannelMeasure->Measure.MaxCurrent)
    {
        ChannelMeasure->Measure.MaxCurrent = AmpMonitorCurrent;
    }

    // STEP 5: Calculate the RMS current
    ChannelMeasure->Measure.RMS_Current = ChannelMeasure->Measure.RMS_CurrentFunction(RMS_AmpMon, AmpMonitorCurrent);

} // END OF calculateChannelCurrent



/********************************************************************************************************
* @brief Calculates the CHANNEL mean, max, min and RMS voltage.
*
* @author original: Hab Collector \n
*
* @note: TODO MORE NOTES
* @note: TODO MORE NOTES
* @note: SYSTEM refers to quantities that impact the entire device - CHANNEL impacts on CH1 or CH2
*
* @param ADC_VoltMonitorCount: ADC Count from ADC channel for the specific voltage monitor
* @param FIFO_VoltMon: FIFO structure associated with the specific Volt Monitor channel
* @param Measure: ArbPwrBooster measure struct associated with the specific channel
* @param RMS_VoltMon: Associated RMS structure
*
* STEP 1: Calculate the mean of the Voltage Monitor
* STEP 2: Calculate the instantaneous voltage
* STEP 3: Update Min voltage or reset min if so desired
* STEP 4: Update Max voltage or reset max if so desired
* STEP 5: Calculate the RMS voltage
********************************************************************************************************/
static void calculateChannelVoltage(double ADC_VoltMonitorCount, Type_16b_FIFO *FIFO_VoltMon, Type_ChannelMeasure *Measure, Type_RMS *RMS_VoltMon)
{
    // TODO Think about a periodic min max reset
    // STEP 1: Calculate the mean of the Voltage Monitor
    double MeanVoltageCount = (double)FIFO_VoltMon->Sum / FIFO_VoltMon->Depth;
    Measure->MeanVoltage = MeanVoltageCount * LSB_12BIT_VALUE * System_ADC_Reference * VOLT_MON_DIVIDER;

    // STEP 2: Calculate the instantaneous voltage
    double VoltMonitorVoltage = ADC_VoltMonitorCount * LSB_12BIT_VALUE * System_ADC_Reference * VOLT_MON_DIVIDER;

    // STEP 3: Update Min voltage or reset min if so desired
#ifdef CAL_MIN_MAX_VOLTAGE
    if (Measure->ResetVoltageMinMax)
        Measure->MinVoltage = VoltMonitorVoltage;
    else if (VoltMonitorVoltage < Measure->MinVoltage)
        Measure->MinVoltage = VoltMonitorVoltage;

    // STEP 4: Update Max voltage or reset max if so desired
    if (Measure->ResetVoltageMinMax)
    {
        Measure->MaxVoltage = VoltMonitorVoltage;
        Measure->ResetVoltageMinMax = false;
    }
    else if (VoltMonitorVoltage > Measure->MaxVoltage)
    {
        Measure->MaxVoltage = VoltMonitorVoltage;
    }
#endif

    // STEP 5: Calculate the RMS voltage
    Measure->RMS_Voltage = Measure->RMS_VoltageFunction(RMS_VoltMon, VoltMonitorVoltage);

} // END OF calculateChannelVoltage



void monitorTaskInit(void)
{
    DO_NOTHING();
}


void monitorTaskActions(void)
{
    static bool CriticalSystemError = false;
    // STEP 1: Do nothing until introduction splash screen completed
    if (ArbPwrBooster.Ready)
    {
        // STEP 2: Do nothing if a system error has occurred
        if (!CriticalSystemError)
        {
            // STEP 3: Check that VS supply within limits - if not disable input
            char NotUsedStatusMsg[25] = {0x00};
            uint8_t ConfigError = 0;
            systemMeasureWithinLimits(NotUsedStatusMsg, &ConfigError);
            // TODO: Hab needs work - IR losses on VS input lines causes this to fail - adjust VS limit error
            if ((ConfigError & CONFIG_POS_VS_ERROR) || (ConfigError & CONFIG_NEG_VS_ERROR))
            {
//                CH1_INPUT_DISABLE();  TODO: Hab this is temporary patch - allows max output power on channel 1
                CH2_INPUT_DISABLE();
            }
            else
            {
                CH1_INPUT_ENABLE();
                CH2_INPUT_ENABLE();
            }

            // STEP 4: Check for current limit conditions CH 1
//            if ((ArbPwrBooster.CH1.OutputSwitch = ON) && (ArbPwrBooster.CH1.Limit.Enable))
//            {
//                uint8_t DigitalPotValue_CH1 = PID_updateDigitalPot(PID_CH1_DigitalPot, ArbPwrBooster.CH1.Measure.RMS_Current, ArbPwrBooster.CH1.Limit.Current, (MONITOR_UPDATE_RATE * 1E-3), MCP45HVX1_POT_FULL_RESOLUTION);
//                if (DigitalPotValue_CH1 != 0)
//                {
//                    if (!MCP45HVX1_WriteWiperVerify(&hi2c1, A1A0_EXTERNAL_ADDR_CH1, DigitalPotValue_CH1))
//                        printYellow("WARNING: Failed to adjust digital pot control\r\n");
//                    else
//                    {
//                        char PrintMsg[100];
//                        sprintf(PrintMsg, "CH1 Pot: %d\r\n", DigitalPotValue_CH1);
//                        printGreen(PrintMsg);
//                    }
//                }
//            }

            // STEP 5: Check for current limit conditions CH 2


            // STEP 6: Check for critical system error conditions
            // Over Voltage on ±VS supply
            if ((ArbPwrBooster.SystemMeasure.Positive_VS >= SYSTEM_POS_VS_LIMIT) || (ArbPwrBooster.SystemMeasure.Negative_VS <= SYSTEM_NEG_VS_LIMIT))
            {
                CriticalSystemError = true;
                MAIN_PWR_OFF();
                printBrightRed("\r\nERROR: Critical on VS supply rail\r\n");
                printBrightRed("Correct supply rail then reset\r\n");
            }
            // Over Temperature
            if (ArbPwrBooster.SystemMeasure.TempDegreeC >= SYSTEM_TEMP_UPPER_LIMIT)
            {
                CriticalSystemError = true;
                MAIN_PWR_OFF();
                printBrightRed("\r\nERROR: Critical over temperature\r\n");
                printBrightRed("Cool device then reset\r\n");
            }
        }
    }

    // STEP 7: Make task inactive for a period of time
    osDelay(MONITOR_UPDATE_RATE);
}



/********************************************************************************************************
* @brief ADC1 is configured for DMA single shot conversion mode.  At the end of each conversion the ADC must
* be restarted for the next conversion.  Issue this command to start a conversion.  When the conversion is
* completed the results will be handled in HAL_ADC_ConvCpltCallback().  As this is single shot, you must call
* this function at the end of conversion if you want another conversion - and so on and so on.
*
* @author original: Hab Collector \n
*
* @note: SYSTEM refers to quantities that impact the entire device - CHANNEL impacts on CH1 or CH2
*
* STEP 1: Start ADC 1 conversion all channels in single shot DMA mode
********************************************************************************************************/
void ADC1_StartConversion(void)
{
    // STEP 1: Start ADC 1 conversion all channels in single shot DMA mode
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC1_CountValue, ADC1_NUMBER_OF_CHANNELS);

}  // END OF ADC1_StartConversion



/********************************************************************************************************
* @brief Checks the system values of +VS, -VS, Temperature and Vref to see if they are within the specified
* limits of operation.  All errors are OR together to the error variable, but only the first error description
* is returned.
*
* @author original: Hab Collector \n
*
* @note: SYSTEM refers to quantities that impact the entire device - CHANNEL impacts on CH1 or CH2
*
* STEP 1: Check System +VS
* STEP 2: Check System -VS
* STEP 3: Check System Vref
* STEP 4: Check System Temperature
* STEP 5: For no error
********************************************************************************************************/
bool systemMeasureWithinLimits(char *ErrorDescription, uint8_t *ErrorNumber)
{
    bool FirstErrorFound = false;
    *ErrorNumber = CONFIG_NO_ERROR;

    // STEP 1: Check System +VS
    if ((ArbPwrBooster.SystemMeasure.Positive_VS > SYSTEM_POS_VS_HIGH) || (ArbPwrBooster.SystemMeasure.Positive_VS < SYSTEM_POS_VS_LOW))
    {
        *ErrorNumber |= (1 << CONFIG_POS_VS_MASK);
        if (!FirstErrorFound)
        {
            sprintf(ErrorDescription, "+VS: %3.1fV to %3.1fV", SYSTEM_POS_VS_LOW, SYSTEM_POS_VS_HIGH);
            FirstErrorFound = true;
        }
    }

    // STEP 2: Check System -VS
    if ((ArbPwrBooster.SystemMeasure.Negative_VS > SYSTEM_NEG_VS_HIGH) || (ArbPwrBooster.SystemMeasure.Negative_VS < SYSTEM_NEG_VS_LOW))
    {
        *ErrorNumber |= (uint8_t)(1 << CONFIG_NEG_VS_MASK);
        if (!FirstErrorFound)
        {
            sprintf(ErrorDescription, "-VS: %3.1fV to %3.1fV", SYSTEM_NEG_VS_LOW, SYSTEM_NEG_VS_HIGH);
            FirstErrorFound = true;
        }
    }

    // STEP 3: Check System Vref
    if ((ArbPwrBooster.SystemMeasure.VDD_VDREF > SYSTEM_3V3_UPPER_LIMIT) || (ArbPwrBooster.SystemMeasure.VDD_VDREF < SYSTEM_3V3_LOWER_LIMIT))
    {
        *ErrorNumber |= (uint8_t)(1 << CONFIG_VREF_MASK);
        if (!FirstErrorFound)
        {
            sprintf(ErrorDescription, "VREF: %4.3fV to %4.3fV", SYSTEM_3V3_LOWER_LIMIT, SYSTEM_3V3_UPPER_LIMIT);
            FirstErrorFound = true;
        }
    }

    // STEP 4: Check System Temperature
    if ((ArbPwrBooster.SystemMeasure.TempDegreeC >= SYSTEM_TEMP_UPPER_LIMIT) || (ArbPwrBooster.SystemMeasure.TempDegreeC <= SYSTEM_TEMP_LOWER_LIMIT))
    {
        *ErrorNumber |= (uint8_t)(1 << CONFIG_TEMP_MASK);
        if (!FirstErrorFound)
        {
            sprintf(ErrorDescription, "%dC < Temp < %dC", SYSTEM_TEMP_LOWER_LIMIT, SYSTEM_TEMP_UPPER_LIMIT);
            FirstErrorFound = true;
        }
    }

    // STEP 5: For no error
    if (*ErrorNumber == CONFIG_NO_ERROR)
    {
        sprintf(ErrorDescription, "I'm good how about you.");
        return(true);
    }
    else
    {
        return(false);
    }

} // END OF systemMeasureWithinLimits





//// OLD RMS STUFF TO BE DELETED
//// Circular buffer for storing recent ADC values
//static float BufferWindow_CH1[RMS_WINDOW_SIZE] = {0};
//// Function to update RMS in real-time
//double update_CH1_RMS_Current(double NewSampleValue)
//{
//    static int Index = 0;
//    static int Count = 0;
//    static double SumOfSquares = 0.0;
//
//    double RMS_CurrentValue;
//    // Remove the oldest value from sum_squares if buffer is full
//    if (Count >= RMS_WINDOW_SIZE)
//        SumOfSquares -= BufferWindow_CH1[Index] * BufferWindow_CH1[Index];  // Remove old squared value
//    else
//        Count++;
//
//    // Store new value and update sum of squares
//    BufferWindow_CH1[Index] = NewSampleValue;
//    SumOfSquares += NewSampleValue * NewSampleValue;
//
//    // Update RMS
//    RMS_CurrentValue = sqrt(SumOfSquares / Count);
//
//    // Move buffer index (circular)
//    Index = (Index + 1) % RMS_WINDOW_SIZE;
//
//    return(RMS_CurrentValue);
//}
//
//
//static float BufferWindow_CH2[RMS_WINDOW_SIZE] = {0};
//double update_CH2_RMS_Current(double NewSampleValue)
//{
//    static int Index = 0;
//    static int Count = 0;
//    static double SumOfSquares = 0.0;
//
//    double RMS_CurrentValue;
//    // Remove the oldest value from sum_squares if buffer is full
//    if (Count >= RMS_WINDOW_SIZE)
//        SumOfSquares -= BufferWindow_CH2[Index] * BufferWindow_CH2[Index];  // Remove old squared value
//    else
//        Count++;
//
//    // Store new value and update sum of squares
//    BufferWindow_CH2[Index] = NewSampleValue;
//    SumOfSquares += NewSampleValue * NewSampleValue;
//
//    // Update RMS
//    RMS_CurrentValue = sqrt(SumOfSquares / Count);
//
//    // Move buffer index (circular)
//    Index = (Index + 1) % RMS_WINDOW_SIZE;
//
//    return(RMS_CurrentValue);
//}
//
//
//static float BufferWindowVoltage_CH1[RMS_WINDOW_SIZE] = {0};
//double update_CH1_RMS_Voltage(double NewSampleValue)
//{
//    static int Index = 0;
//    static int Count = 0;
//    static double SumOfSquares = 0.0;
//
//    double RMS_CurrentValue;
//    // Remove the oldest value from sum_squares if buffer is full
//    if (Count >= RMS_WINDOW_SIZE)
//        SumOfSquares -= BufferWindowVoltage_CH1[Index] * BufferWindowVoltage_CH1[Index];  // Remove old squared value
//    else
//        Count++;
//
//    // Store new value and update sum of squares
//    BufferWindowVoltage_CH1[Index] = NewSampleValue;
//    SumOfSquares += NewSampleValue * NewSampleValue;
//
//    // Update RMS
//    RMS_CurrentValue = sqrt(SumOfSquares / Count);
//
//    // Move buffer index (circular)
//    Index = (Index + 1) % RMS_WINDOW_SIZE;
//
//    return(RMS_CurrentValue);
//}
//
//
//static float BufferWindowVoltage_CH2[RMS_WINDOW_SIZE] = {0};
//double update_CH2_RMS_Voltage(double NewSampleValue)
//{
//    static int Index = 0;
//    static int Count = 0;
//    static double SumOfSquares = 0.0;
//
//    double RMS_CurrentValue;
//    // Remove the oldest value from sum_squares if buffer is full
//    if (Count >= RMS_WINDOW_SIZE)
//        SumOfSquares -= BufferWindowVoltage_CH2[Index] * BufferWindowVoltage_CH2[Index];  // Remove old squared value
//    else
//        Count++;
//
//    // Store new value and update sum of squares
//    BufferWindowVoltage_CH2[Index] = NewSampleValue;
//    SumOfSquares += NewSampleValue * NewSampleValue;
//
//    // Update RMS
//    RMS_CurrentValue = sqrt(SumOfSquares / Count);
//
//    // Move buffer index (circular)
//    Index = (Index + 1) % RMS_WINDOW_SIZE;
//
//    return(RMS_CurrentValue);
//}
//
//
//
