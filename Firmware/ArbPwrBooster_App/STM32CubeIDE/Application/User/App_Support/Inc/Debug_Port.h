 /*****************************************************************************************************
 * @file 			Debug_Port.h
 * @brief			This is the Header file used to support Debug_Port.c
 * ****************************************************************************************************
 * @author original Hab Collector (habco)\n
 *
 * @version         See Main_Support.h: FW_MAJOR_REV, FW_MINOR_REV, FW_TEST_REV
 *
 * @param Development_Environment \n
 * Hardware:        <IC PN>  \n
 * IDE:             STMCubeIDE VER 1.17.0 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            <Add user notes starting here>
 *
 *                  This is an embedded application
 *                  It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *                    Schematic: <Schematic PN>
 *				      STM32F746G_DISCO
 *                    IMR Engineering
 *
 * @copyright       IMR Engineering, LLC
 ******************************************************************************************************/

#ifndef DEBUG_PORT_H_
#define DEBUG_PORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "Hab_Types.h"

// DEFINES
#define DELAY_TIME_FAST_TYPER       150U
// Actionable Commands
#define DEBUG_CONSOLE_HELP          "Help"
#define WRITE_DIGITAL_POT_ATTEN     "Write Pot"
#define READ_DIGITAL_POT_ATTEN      "Read Pot"
#define CHANNEL_OUTPUT_ON           "Output On"
#define CHANNEL_OUTPUT_OFF          "Output Off"
#define DEVICE_STATUS               "Status"

// FUNCTION PROTOTYPES
void debugConsoleTaskInit(void);
void debugConsoleTaskActions(void);
void debugPortParser(char *CharacterBuffer, uint8_t NumberOfChars);
void commandPrompt(void);

#ifdef __cplusplus
}
#endif
#endif /* DEBUG_PORT_H_ */
