/** ****************************************************************************************************
 * @file            ADC_Support.h
 * @brief           This is the Header file used to support ADC_Support.c
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
#include "Main_Support.h"


// DEFINES
// PRE-PROCESSOR                            // Un-comment to use
//#define CAL_MIN_MAX_VOLTAGE
//#define PRINT_CC_PID_UPDATES
// ADC CHANNEL
#define ADC1_NUMBER_OF_CHANNELS 2U
#define ADC3_NUMBER_OF_CHANNELS 6U
// ADC CONVERSION RELATED
#define ADC_12BIT_FULL_COUNT    4096
#define LSB_12BIT_VALUE         244.14E-6
#define ADC_REFERENCE_VOLTAGE   3.300
#define ADC_REF_VOLTAGE_COUNT   1500        // This value is approximated it will be slightly different for each uC IC based on internal factory calibration value of VREFINT_CAL_ADDR
#define SYSTEM_VS_DIVIDER       9.0645      // External divider ratio = 0.1103 with an equivalence of 3.3V (reference) to 30V external
#define AMP_MONITOR_GAIN        50          // Value in V/V this is the Gain of the current sense Amplifier
#define AMP_SENSE_RESISTOR      6.5E-3      // Value in ohms
#define VOLT_MON_DIVIDER        4.3333      // External divider ration = .2308 with an equivlence of 3.3V (reference) to 14.3V external
#define CURRENT_MON_ZERO_OFFSET 1.6500      // Value in volts this is the mid value (0A) reference voltage
// ADC GAIN AND OFFSET ERRORS: ****NOTE THESE ARE CALIBRATED RESULTS FROM TESTING - MAY NEED TO BE UNIQUE PER BOARD
#define ADC_PVS_OFFSET_ERROR    0           // Value is in ADC Counts
#define ADC_PVS_GAIN_ERROR      0.9971
#define ADC_NVS_OFFSET_ERROR    0           // Value is in ADC Counts
#define ADC_NVS_GAIN_ERROR      1.0010
#define ADC_VMON1_OFFSET_ERROR  0           // Value is in ADC Counts
#define ADC_VMON1_GAIN_ERROR    1.0000
#define ADC_VMON2_OFFSET_ERROR  0           // Value is in ADC Counts
#define ADC_VMON2_GAIN_ERROR    1.0000
#define ADC_IMON1_OFFSET_ERROR  2           // Value is in ADC Counts
#define ADC_IMON1_GAIN_ERROR    0.9948
#define ADC_IMON2_OFFSET_ERROR  4
#define ADC_IMON2_GAIN_ERROR    0.9910
#define RMS_TO_MEAN_THREHOLD    0.030       // Value in Amps
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
#define SYSTEM_PVS_BUFFER_SIZE  16U     // Expected DC Value
#define SYSTEM_NVS_BUFFER_SIZE  16U     // Expected DC Value
#define CH1_VOLT_MON_BUFFER_SIZE 16U
#define CH2_VOLT_MON_BUFFER_SIZE 16U
#if (CH1_VOLT_MON_BUFFER_SIZE != CH2_VOLT_MON_BUFFER_SIZE)
#warning Hab the buffersize of the the voltage monitor channels should be the same
#endif
// RMS SPECIFIC
#define RMS_WINDOW_SIZE         1024
// PID RELATED
#define PID_Pk                  100
#define PID_Pi                  2500
#define PID_Pd                  0
// TASK UPDATE RATE
#define MONITOR_UPDATE_RATE     10U     // Value in ms
// POWER CALCULATION RELATED
#define MAX_PD_WITHOUT_HEATSINK 3.5     // Value in Watts max allowable power dissipation by the OPA549s without a heat sink
#define NOMINAL_SUPPLY_RAIL_V   15.0    // Nominal supply rail voltage - assumption is made power is taken from one not both supply rails - the output voltage is either totally positive or negative not both
#define FAN_ON_COUNTER_30S      (30 / (MONITOR_UPDATE_RATE * 1E-3))



// TYPEDEFS AND ENUMS
typedef enum
{
    CONFIG_POS_VS_LIMIT_MASK = 0,
    CONFIG_NEG_VS_LIMIT_MASK,
    CONFIG_TEMP_MASK,
    CONFIG_VREF_MASK,
    CONFIG_VS_CRITICAL_ERROR_MASK
}Type_ConfigErrorMask;

typedef enum
{
    CONFIG_NO_ERROR = 0,
    CONFIG_POS_VS_ERROR = (1 << CONFIG_POS_VS_LIMIT_MASK),
    CONFIG_NEG_VS_ERROR = (1 << CONFIG_NEG_VS_LIMIT_MASK),
    CONFIG_TEMP_ERROR = (1 << CONFIG_TEMP_MASK),
    CONFIG_VREF_ERROR = (1 << CONFIG_VREF_MASK),
    CONFIG_VS_CRITICAL_ERROR = (1 << CONFIG_VS_CRITICAL_ERROR_MASK)
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
void monitorTaskInit(void);
void monitorTaskActions(void);
bool updateAmpMonitorZeroVoltage(Type_Channel Channel);

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_USER_ADC_SUPPORT_H_ */
