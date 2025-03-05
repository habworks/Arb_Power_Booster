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
#include "IO_Support.h"
#include "stm32f7xx_hal_adc.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

// Global
uint16_t ADC1_CountValue[ADC1_NUMBER_OF_CHANNELS];
uint16_t ADC3_CountValue[ADC3_NUMBER_OF_CHANNELS];
double ADC_RawConvertedResult[NUMBER_OF_CHANNELS];


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
*
* STEP 1: Calibrate ADCs
* STEP 2: Enable Internal reference and temperature measure
* STEP 3: Start ADC1 and ADC3 in DMA continuous conversion
********************************************************************************************************/
void Init_ADC_Hardware(void)
{
    // STEP 1: Calibrate ADCs
    // HAB TODO: This not workinig
//    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
//    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);

    // STEP 2: Enable Internal reference and temperature measure
    // HAB TODO: This not workinig
    //    HAL_ADCEx_EnableVREFINT();
    ADC->CCR |= ADC_CCR_TSVREFE_Msk;

    // STEP 3: Start ADC1 and ADC3 in DMA continuous conversion
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC1_CountValue, ADC1_NUMBER_OF_CHANNELS);
    HAL_ADC_Start_DMA(&hadc3, (uint32_t *)ADC3_CountValue, ADC3_NUMBER_OF_CHANNELS);
}



/********************************************************************************************************
* @brief Redefinition of the weak callback function for ADC conversion complete.
*
* @author original: Hab Collector \n
*
* @note: This is redefinition - this function is define weak in the HAL but implemented here
* @note: The Rate toggle output are used to verify ADC conversion rate.  Conversion rate is calculated as:
* PCLK2 / ADC_Clock_Presclar * ConversionCycles * NumberOfChannels
*
* STEP 1: ADC1 Conversions
* STEP 2: ADC3 Conversions
********************************************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    // STEP 1: ADC1 Conversions
    if (hadc == &hadc1)
    {
        // Current Monitor CH1
        ADC_RawConvertedResult[CH1_AMP_MON] = ADC1_CountValue[RANK_1] * LSB_12BIT_VALUE * ADC_REFERENCE_VOLTAGE;
        // System 3.3V
        double VRefInternalCal = (double)*VREFINT_CAL_ADDR;
        ADC_RawConvertedResult[INTERNAL_REF] = (VRefInternalCal * ADC_REFERENCE_VOLTAGE) / ADC1_CountValue[RANK_2];

        // Toggle at the end of conversion - for testing ADC conversion speed only
        ADC1_C_RATE_TOGGLE();
    }

    // STEP 2: ADC3 Conversions
    if (hadc == &hadc3)
    {
        // Current Monitor CH2
        ADC_RawConvertedResult[NEG_20V_MON] = ADC3_CountValue[RANK_1] * LSB_12BIT_VALUE * ADC_REFERENCE_VOLTAGE;
        ADC_RawConvertedResult[POS_20V_MON] = ADC3_CountValue[RANK_2] * LSB_12BIT_VALUE * ADC_REFERENCE_VOLTAGE;
        ADC_RawConvertedResult[CH2_AMP_MON] = ADC3_CountValue[RANK_3] * LSB_12BIT_VALUE * ADC_REFERENCE_VOLTAGE;

        // Toggle at the end of conversion - for testing ADC conversion speed only
        ADC3_C_RATE_TOGGLE();
    }
} // END OF HAL_ADC_ConvCpltCallback
