/******************************************************************************************************
 * @file 			Debug_Port.c
 * @brief			Functions to support the Arb Power Booster debug port interface
 * ****************************************************************************************************
 * @author original	Hab Collector (habco)\n
 *
 * @version       	See Main_Support.h: FIRMWARE_REV_MAJOR, FIRMWARE_REV_MINOR
 *
 * @param Development_Environment \n
 * Hardware:		<IC PN> \n
 * IDE:             STMCubeIDE VER 1.9.0 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            The associated header file provides MACRO functions for IO control
 *
 * 					This is an embedded application
 *		            It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *			          Schematic: <Schematic PN>
 *				      STM32F746G_DISCO
 *				      IMR Engineering
 *
 * @copyright		IMR Engineering, LLC
 ******************************************************************************************************/

#include "Debug_Port.h"
#include "UART_Support.h"
#include "Terminal_Emulator_Support.h"
#include "MCP45HVX1_Driver.h"
#include "cmsis_os2.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

// Static Function Declarations
static void callDebugFunction(char *CommandToProcess);
static void resetDebugCommandSearch(void);
// Actionable Commands
static void printDebugConsoleHelp(void);
static void writeDigitalPotAttenuation(char *CommandLine);
static void readDigitalPotAttenuation(char *CommandLine);
// Global
static char CharsToProcessBuffer[10] = {0};
static char DebugCommand[100] = {0};
static char LastDebugCommand[100] = {0};
static uint8_t DebugCommandIndex = 0;


/********************************************************************************************************
* @brief Pre-actions to be performed before debugConsoleTaskActions - These actions are performed only once.
*
* @author original: Hab Collector \n
*
* STEP 1: Set the last debug command to help
********************************************************************************************************/
void debugConsoleTaskInit(void)
{
    strcpy(LastDebugCommand, "Help");

} // END OF debugConsoleTaskInit



/********************************************************************************************************
* @brief These are the actions taken by thread debugConsoleTask().  Actions are solely related to the processing
* of user input from the debug port.  On entry from the debug port by user action is taken to execute the
* user command.
*
* @author original: Hab Collector \n
*
* STEP 1: Check if there is something to do
* STEP 2: Load the characters to process and call the debug console parser
* STEP 3: Delay based on how fast a person can type
********************************************************************************************************/
void debugConsoleTaskActions(void)
{
    // STEP 1: Check if there is something to do
    if (UART_DebugPort.Rx_FIFO_Count == 0)
        return;

    // STEP 2: Load the characters to process and call the debug console parser
    memset(CharsToProcessBuffer, 0x00, sizeof(CharsToProcessBuffer));
    memcpy(CharsToProcessBuffer, UART_DebugPort.Rx_FIFO, UART_DebugPort.Rx_FIFO_Count);
    uint8_t CharsToProcess = UART_DebugPort.Rx_FIFO_Count;
    UART_DebugPort.Rx_FIFO_Count = 0;
    UART_DebugPort.Parser(CharsToProcessBuffer, CharsToProcess);

    // STEP 3: Delay based on how fast a person can type
    osDelay(DELAY_TIME_FAST_TYPER);

} // END OF debugConsoleTaskActions



/********************************************************************************************************
* @brief Debug console parser - parse the incoming user command once \r has been entered.  Make special
* accommodations for user '?' and backspace (user can backspace as they type).
*
* @author original: Hab Collector \n
*
* @param CharacterBuffer: The command buffer to be processed for user entry
* @param NumberOfChars: Number of characters to process from CharacterBuffer
*
* STEP 1: Support all user entered characters except return, backspace, repeat and ?
* STEP 2: Handle return entered - process command and reset for next entry
* STEP 3: Process for user entered back space (user made a typing mistake and is backing up
* STEP 4: Process ? - Show help commands
* STEP 5: Process ` - Repeat last command
********************************************************************************************************/
void debugPortParser(char *CharacterBuffer, uint8_t NumberOfChars)
{
    for (uint8_t Index = 0; Index < NumberOfChars; Index++)
    {
        // STEP 1: Support all user entered characters except return, backspace and ?
        if ((CharacterBuffer[Index] != '\r') && (CharacterBuffer[Index] != '\b') && (CharacterBuffer[Index] != '?') && (CharacterBuffer[Index] != '`'))
        {
            printf("%c", CharacterBuffer[Index]);
            fflush(stdout);
            DebugCommand[DebugCommandIndex] = CharacterBuffer[Index];
            DebugCommandIndex++;
            if (DebugCommandIndex >= sizeof(DebugCommand))
            {
                resetDebugCommandSearch();
                return;
            }
        }

        // STEP 2: Handle return entered - process command and reset for next entry
        if (CharacterBuffer[Index] == '\r')
        {
            DebugCommand[DebugCommandIndex] = 0x00; // Ensures end of string for parsing
            strcpy(LastDebugCommand, DebugCommand);
            printf("\r\n");
            fflush(stdout);
            callDebugFunction(DebugCommand);
        }

        // STEP 3: Process for user entered back space (user made a typing mistake and is backing up
        if (CharacterBuffer[Index] == '\b')
        {
            DebugCommand[DebugCommandIndex] = 0x00;
            DebugCommandIndex = (DebugCommandIndex == 0)? 0 : (DebugCommandIndex - 1);
            terminal_CursorBackSpace();
            printf(" ");
            terminal_CursorBackSpace();
            fflush(stdout);
        }

        // STEP 4: Process ? - Show help commands
        if (CharacterBuffer[Index] == '?')
        {
            printDebugConsoleHelp();
            resetDebugCommandSearch();
            return;
        }

        // STEP 5: Process ` - Repeat last command
        if (CharacterBuffer[Index] == '`')
        {
            printf("%s\r\n", LastDebugCommand);
            fflush(stdout);
            callDebugFunction(LastDebugCommand);
        }
    }

} // END OF debugPortParser



/********************************************************************************************************
* @brief print to screen the command prompt
*
* @author original: Hab Collector \n
********************************************************************************************************/
void commandPrompt(void)
{
    printf("Command> ");

} // END OF commandPrompt



/********************************************************************************************************
* @brief Search the user entry for a valid debug command string.  If the command string is found process
* the command
*
* @author original: Hab Collector \n
*
* @note: Allow the individual commands to do their own error checking, and command parsing for parameters
*
* STEP 1: Check for commands - call command function if found
* STEP 2: Update if no valid command found
********************************************************************************************************/
static void callDebugFunction(char *CommandToProcess)
{
    // STEP 1: Check for commands - call command function if found
    bool CommandFound = true;
    if (strstr(CommandToProcess, DEBUG_CONSOLE_HELP) != NULL)
    {
        printDebugConsoleHelp();
    }
    else if (strstr(CommandToProcess, WRITE_DIGITAL_POT_ATTEN) != NULL)
    {
        writeDigitalPotAttenuation(CommandToProcess);
    }
    else if (strstr(CommandToProcess, READ_DIGITAL_POT_ATTEN) != NULL)
    {
        readDigitalPotAttenuation(CommandToProcess);
    }
    else
    {
        CommandFound = false;
    }

    // STEP 2: Update if no valid command found
    if ((!CommandFound) && (DebugCommandIndex != 0))
        printYellow("No such command\r\n");
    resetDebugCommandSearch();

} // END OF callDebugFunction


/********************************************************************************************************
* @brief Reset Command buffer and index for a new search.
*
* @author original: Hab Collector \n
*
* @note: Call this function when you want to enter a new command
*
* STEP 1: Reset for new command search
********************************************************************************************************/
static void resetDebugCommandSearch(void)
{
    // STEP 1: Reset for new command search
    memset(DebugCommand, 0x00, sizeof(DebugCommand));
    DebugCommandIndex = 0;
    commandPrompt();
    fflush(stdout);

} // END OF resetDebugCommandSearch



/********************************************************************************************************
*                                     START OF ACTIONABLE COMMANDS
********************************************************************************************************/

/********************************************************************************************************
* @brief Provide a printed list to the user of all debug commands
*
* @author original: Hab Collector \n
*
* @note: I did not automate this as much as I have in the past - be sure to enter all commands here
*
* STEP 1: Print debug command help
********************************************************************************************************/
static void printDebugConsoleHelp(void)
{
    // STEP 1: Print debug command help
    printf("\r\nDEBUG COMMANDS: \r\n");
    printf("  Write Pot X: Where X is value 0 to 255 of wiper\r\n");
    printf("  Read Pot X:  Returns value of pot wiper 0 - 255\r\n");

} // END OF printDebugConsoleHelp



static void writeDigitalPotAttenuation(char *CommandLine)
{
    // STEP 1: Make sure there is a space after command
    char *SpaceChar = CommandLine + strlen(WRITE_DIGITAL_POT_ATTEN);
    if (*SpaceChar != ' ')
    {
        printRed("Error: Command Syntax see help\r\n");
        return;
    }

    // TODO: Hab extract the channel value and make channel specific writes

    // STEP 2: Get the value and convert it
    uint16_t PotWiperValue;
    uint8_t CommandLenght = strlen(CommandLine);
    char *EndofCommandLinePointer = CommandLine + CommandLenght;
    char *PotValuePointer = SpaceChar + 1;
    char SetPotValue[4] = {0}; // 3 Digits terminated with null
    uint8_t NumberOfDigits = EndofCommandLinePointer - PotValuePointer;
    if (NumberOfDigits > (sizeof(SetPotValue) - 1))
    {
        printRed("Error: Command Syntax see help\r\n");
        return;
    }
    else
    {
        strncpy(SetPotValue, PotValuePointer, NumberOfDigits);
        PotWiperValue = atoi(SetPotValue);
    }

    if (MCP45HVX1_WriteWiperVerify(&hi2c1, A1A0_EXTERNAL_ADDR_CH1, PotWiperValue))
        printf("Pot Wiper Value: %d\r\n", (int)PotWiperValue);
    else
        printf("Error: Setting pot wiper value\r\n");

} // END OF writeDigitalPotAttenuation


static void readDigitalPotAttenuation(char *CommandLine)
{
    // TODO: Hab extract the channel value and make channel specific reads
    uint8_t PotWiperValue = 0;
    if (MCP45HVX1_ReadWiperValue(&hi2c1, A1A0_EXTERNAL_ADDR_CH1, &PotWiperValue))
        printf("Read Pot Wiper Value: %d\r\n", PotWiperValue);
    else
        printf("Error: Reading pot wiper value\r\n");

} // END OF readDigitalPotAttenuation


