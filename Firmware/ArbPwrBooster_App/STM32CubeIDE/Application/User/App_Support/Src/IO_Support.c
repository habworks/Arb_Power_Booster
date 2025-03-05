/** ****************************************************************************************************
 * @file 			IO_Support.c
 * @brief			Support functions related to IO activity
 * ****************************************************************************************************
 * @author original	Hab Collector (habco)\n
 *
 * @version       	See Main_Support.h: FW_MAJOR_REV, FW_MINOR_REV, FW_TEST_REV
 *
 * @param Development_Environment \n
 * Hardware:		<STM32F764G-DISCO> \n
 * IDE:             STMCubeIDE VER 1.17.0 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            The associated header file provides MACRO functions for IO control
 *
 * 					This is an embedded application
 *		            It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *			          Schematic: IMR-005-SCH
 *				      STM32F746G_DISCO
 *				      IMR
 *
 * @copyright		IMR Engineering, LLC
 ********************************************************************************************************/

#include "IO_Support.h"

/********************************************************************************************************
* @brief Sets the IO in the default POR state
*
* @author original: Hab Collector \n
*
* STEP 1: Set all outputs to the default POR state - Everything off
********************************************************************************************************/
void Init_GPIO_Hardware(void)
{
    // STEP 1: Set all outputs to the default POR state - Everything off
    MAIN_PWR_OFF();
    CH1_OUTPUT_DISABLE();
    CH2_INPUT_50_DISABLE();
    CH2_OUTPUT_DISABLE();
    CH1_INPUT_50_DISABLE();
    ADC1_C_RATE_DISABLE();
    ADC3_C_RATE_DISABLE();

} // END OF Init_GPIO_Hardware
