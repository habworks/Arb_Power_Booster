/** ****************************************************************************************************
 * @file 			ADC_Support.h
 * @brief			This is the Header file used to support ADC_Support.c
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

#ifndef APPLICATION_USER_ADC_SUPPORT_H_
#define APPLICATION_USER_ADC_SUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "Hab_Types.h"


// DEFINES
// ADC CHANNEL
#define ADC1_NUMBER_OF_CHANNELS 2U
#define ADC3_NUMBER_OF_CHANNELS 6U
// ADC CONVERSION RELATED
#define ADC_12BIT_FULL_COUNT    4096
#define LSB_12BIT_VALUE         244.14E-6
#define ADC_REFERENCE_VOLTAGE   3.300
#define ADC_REF_VOLTAGE_COUNT   1500        // This value is approximated it will be slightly different for each uC IC based on internal factory calibration value of VREFINT_CAL_ADDR
#define SYSTEM_VS_DIVIDER       9.0645      // External divider ratio = 0.1103 with an equivalence of 3.3V (reference) to 30V external
#define AMP_MONITOR_GAIN        1.0 //50.0    // Value in V/V this is the Gain of the current sense Amplifier
#define AMP_SENSE_RESISTOR      1.0 //0.0033  // Value in ohms
#define VOLT_MON_DIVIDER        4.333       // External divider ration = .2308 with an equivlence of 3.3V (reference) to 14.3V external
// VREFINT RELATED (SYSTEM 3.3V)
#define VREFINT_CAL_ADDR        ((uint16_t*) ((uint32_t) 0x1FF0F44A)) //0x1FFF75AA)
// TEMP SENSOR RELATED
#define VOLTAGE_AT_25C          0.76
#define AVG_TEMP_SLOPE          2.50
#define TEMP_25C                25.0
// ACCEPTABLE CONVERSION LIMITS
#define SYSTEM_POS_VS_LIMIT     15.5
#define SYSTEM_POS_VS_HIGH      15.3
#define SYSTEM_POS_VS_LOW       14.5
#define SYSTEM_NEG_VS_LIMIT     -15.5
#define SYSTEM_NEG_VS_HIGH      -14.5
#define SYSTEM_NEG_VS_LOW       -15.3
#define SYSTEM_3V3_UPPER_LIMIT  3.350
#define SYSTEM_3V3_LOWER_LIMIT  3.250
#define SYSTEM_TEMP_UPPER_LIMIT 38
#define SYSTEM_TEMP_LOWER_LIMIT 2
// FIFO BUFFERS
#define SYSTEM_3V3_BUFFER_SIZE  16U     // Expected to be DC value
#define SYSTEM_TEMP_BUFFER_SIZE 16U     // Expected to be slow moving temperature value
#define CH1_AMP_MON_BUFFER_SIZE 16U     // Should account for a frequency of 10KHz 2x Nyquist
#define CH2_AMP_MON_BUFFER_SIZE 16U     // Should account for a frequency of 10KHz 2x Nyquist
#if (CH1_AMP_MON_BUFFER_SIZE != CH2_AMP_MON_BUFFER_SIZE)
#warning Hab the buffersize of the the current monitor channels should be the same
#endif
#define SYSTEM_PVS_BUFFER_SIZE 16U     // Expected DC Value
#define SYSTEM_NVS_BUFFER_SIZE 16U     // Expected DC Value
#define CH1_VOLT_MON_BUFFER_SIZE 16U
#define CH2_VOLT_MON_BUFFER_SIZE 16U
#if (CH1_VOLT_MON_BUFFER_SIZE != CH2_VOLT_MON_BUFFER_SIZE)
#warning Hab the buffersize of the the voltage monitor channels should be the same
#endif
//#define RMS_BUFFER_SIZE         16U     // For use with the average RMS value over time
// FACTORY CALIBRATION VALUES AT SPECIFIC ADDRESS
//#define TS_CAL1_ADDR            ((uint16_t*)0x1FFF75A8)     // Factory V_25 (25°C)
//#define TS_CAL2_ADDR            ((uint16_t*)0x1FFF75CA)     // Factory V_110 (110°C)
//#define TS_CAL1_ADDR            ((uint16_t*) 0x1FF0F44C)  // ADC value at 30°C
//#define TS_CAL2_ADDR            ((uint16_t*) 0x1FF0F44E)  // ADC value at 110°C
//#define ADC_BASE_ADDR   0x40012000  // ADC register base address
//#define ADC_CCR_OFFSET  0x308       // Offset for ADC_CCR register
//#define ADC_CCR_ADDR    (ADC_BASE_ADDR + ADC_CCR_OFFSET)
// RMS SPECIFIC
#define RMS_WINDOW_SIZE         1024
// MISC
#define MONITOR_UPDATE_RATE         10U


// TYPEDEFS AND ENUMS
typedef enum
{
    CONFIG_POS_VS_MASK = 0,
    CONFIG_NEG_VS_MASK,
    CONFIG_TEMP_MASK,
    CONFIG_VREF_MASK
}Type_ConfigErrorMask;

typedef enum
{
    CONFIG_NO_ERROR = 0,
    CONFIG_POS_VS_ERROR = (1 << CONFIG_POS_VS_MASK),
    CONFIG_NEG_VS_ERROR = (1 << CONFIG_NEG_VS_MASK),
    CONFIG_TEMP_ERROR = (1 << CONFIG_TEMP_MASK),
    CONFIG_VREF_ERROR = (1 << CONFIG_VREF_MASK)
}Type_ConfigError;

typedef enum
{
    RANK_1 = 0,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6
}Type_ADC_ChannelRank;

typedef struct
{
    bool            FirstOverFlow;
    uint32_t        Sum;
    uint16_t *      Buffer;
    uint16_t *      WritePointer;
    uint8_t         Depth;
}Type_16b_FIFO;


// FUNCTION PROTOTYPES
void Init_ADC_Hardware(void);
void ADC13_StartConversion(void);
void ADC1_StartConversion(void);
bool systemMeasureWithinLimits(char *ErrorDescription, uint8_t *ErrorNumber);
double update_CH1_RMS_Current(double NewSampleValue);
double update_CH2_RMS_Current(double NewSampleValue);
double update_CH1_RMS_Voltage(double NewSampleValue);
double update_CH2_RMS_Voltage(double NewSampleValue);
void monitorTaskInit(void);
void monitorTaskActions(void);

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_USER_ADC_SUPPORT_H_ */
