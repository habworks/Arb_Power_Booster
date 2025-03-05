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
 *                    IMR
 *
 * @copyright       IMR Engineering, LLC
 ********************************************************************************************************/

#ifndef APPLICATION_USER_ADC_SUPPORT_H_
#define APPLICATION_USER_ADC_SUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

// DEFINES
// ADC CHANNEL
#define ADC1_NUMBER_OF_CHANNELS 3U
#define ADC3_NUMBER_OF_CHANNELS 3U
// ADC CONVERSION RELATED
#define LSB_12BIT_VALUE         244.1E-6
#define ADC_REFERENCE_VOLTAGE   3.300
#define POS_20V_DIVIDER         1.0   //0.110
#define NEG_20V_DIVIDER         1.0   //0.110
// FACTORY CALIBRATION VALUES AT SPECIFIC ADDRESS
#define TS_CAL1_ADDR            ((uint16_t*)0x1FFF75A8)     // Factory V_25 (25°C)
#define TS_CAL2_ADDR            ((uint16_t*)0x1FFF75CA)     // Factory V_110 (110°C)
#define VREFINT_CAL_ADDR        ((uint16_t*)0x1FFF75AA)     // Factor preset VerfInt
//#define ADC_BASE_ADDR   0x40012000  // ADC register base address
//#define ADC_CCR_OFFSET  0x308       // Offset for ADC_CCR register
//#define ADC_CCR_ADDR    (ADC_BASE_ADDR + ADC_CCR_OFFSET)


// TYPEDEFS AND ENUMS
typedef enum
{
    CH1_AMP_MON = 0,
    CH2_AMP_MON,
    POS_20V_MON,
    NEG_20V_MON,
    TEMP_SENSOR,
    INTERNAL_REF,
    NUMBER_OF_CHANNELS
}Type_ADC_Channel;

typedef enum
{
    RANK_1 = 0,
    RANK_2,
    RANK_3
}Type_ADC_ChannelRank;


// FUNCTION PROTOTYPES
void Init_ADC_Hardware(void);



#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_USER_ADC_SUPPORT_H_ */
