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
#include "Main_Support.h"
#include "UART_Support.h"
#include "Terminal_Emulator_Support.h"
#include "MCP45HVX1_Driver.h"
#include "IO_Support.h"
#include "cmsis_os2.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



// Static Function Declarations
static void callDebugFunction(char *CommandToProcess);
static void resetDebugCommandSearch(void);
// Actionable Commands
static void printDebugConsoleHelp(void);
static void writeDigitalPotAttenuation(char *CommandLine);
static void readDigitalPotAttenuation(char *CommandLine);
static void channelOutputSwitch(char *CommandLine);
static void deviceStatus(char *CommandLine);
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
    else if (strstr(CommandToProcess, CHANNEL_OUTPUT_SWITCH) != NULL)
    {
        channelOutputSwitch(CommandToProcess);
    }
    else if (strstr(CommandToProcess, DEVICE_STATUS) != NULL)
    {
        deviceStatus(CommandToProcess);
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
* @note: This command is called by typing "?" or "Help"
*
* STEP 1: Print debug command help
********************************************************************************************************/
static void printDebugConsoleHelp(void)
{
    // STEP 1: Print debug command help
    printf("\r\nDEBUG COMMANDS: \r\n");
    printf("  Write Pot # X :  Where X is value 0 to 255 of wiper (255 = full resistance)\r\n");
    printf("  Read Pot #:      Returns value of pot wiper 0 - 255\r\n");
    printf("  Output # On/Off: Turns the Output On/Off\r\n");
    printf("  Status:          Show device status\r\n");
    printf("\r\nWhere # is Channel Number (1 or 2)\r\n");

} // END OF printDebugConsoleHelp



/********************************************************************************************************
* @brief Write to the digital pot of channel 1 or 2 the pot value step.  The pot value has 256 steps.  Valid
* step values are 0 to 255, with 127 consider mid way.  A pot value of 255 sets the max resistance, while 0
* sets the min resistance (0).
*
* @author original: Hab Collector \n
*
* @note: Example Format for this command "Write Pot 1 127" - sets the value of the channel 1 pot to 50% resistance
* @note: The pot acts a voltage divider within the circuit and is part of the PID control control loop
* @note: Valid channels are 1 and 2
* @note: Command is case sensitive
*
* STEP 1: Make sure there is a space after command
* STEP 2: Get the channel number
* STEP 3: Get the value and convert it
* STEP 4: Writ the POT Value
********************************************************************************************************/
static void writeDigitalPotAttenuation(char *CommandLine)
{
    uint8_t DigitalPotAddress = 0;

    // STEP 1: Make sure there is a space after command
    char *SpaceChar = CommandLine + strlen(WRITE_DIGITAL_POT_ATTEN);
    if (*SpaceChar != ' ')
    {
        printRed("Error: Command Syntax see help\r\n");
        return;
    }

    // STEP 2: Get the channel number
    char *ChannelNumberChar = SpaceChar + 1;
    if (*ChannelNumberChar == '1')
        DigitalPotAddress = A1A0_EXTERNAL_ADDR_CH1;
    else if (*ChannelNumberChar == '2')
        DigitalPotAddress = A1A0_EXTERNAL_ADDR_CH2;
    else
    {
        printRed("Error: Channel number must be 1 or 2\r\n");
        return;
    }

    // STEP 3: Get the value and convert it
    uint16_t PotWiperValue;
    uint8_t CommandLenght = strlen(CommandLine);
    char *EndofCommandLinePointer = CommandLine + CommandLenght;
    char *PotValuePointer = ChannelNumberChar + 1 + 1; // ChannelNumber_#
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

    // STEP 4: Writ the POT Value
    if (MCP45HVX1_WriteWiperVerify(&hi2c1, DigitalPotAddress, PotWiperValue))
        printf("Pot Wiper Value: %d\r\n", (int)PotWiperValue);
    else
        printf("Error: Setting pot wiper value\r\n");

} // END OF writeDigitalPotAttenuation



/********************************************************************************************************
* @brief Read of the digital pot of channel 1 or 2 the pot value step.  The pot value has 256 steps.  Valid
* step values are 0 to 255, with 127 consider mid way.  A pot value of 255 sets the max resistance, while 0
* sets the min resistance (0).
*
* @author original: Hab Collector \n
*
* @note: Example Format for this command "Read Pot 1" - Reads the step value of channel 1 pot
* @note: The pot acts a voltage divider within the circuit and is part of the PID control control loop
* @note: Valid channels are 1 and 2
* @note: Command is case sensitive
*
* STEP 1: Get the channel number
* STEP 2: Read the pot value
********************************************************************************************************/
static void readDigitalPotAttenuation(char *CommandLine)
{
    uint8_t DigitalPotAddress = 0;

    // STEP 1: Get the channel number
    char *ChannelNumberChar = CommandLine + strlen(READ_DIGITAL_POT_ATTEN) + 1;
    if (*ChannelNumberChar == '1')
        DigitalPotAddress = A1A0_EXTERNAL_ADDR_CH1;
    else if (*ChannelNumberChar == '2')
        DigitalPotAddress = A1A0_EXTERNAL_ADDR_CH2;
    else
    {
        printRed("Error: Channel number must be 1 or 2\r\n");
        return;
    }

    // STEP 2: Read the pot value
    uint8_t PotWiperValue = 0;
    if (MCP45HVX1_ReadWiperValue(&hi2c1, DigitalPotAddress, &PotWiperValue))
        printf("Read Pot Wiper Value: %d\r\n", PotWiperValue);
    else
        printf("Error: Reading pot wiper value\r\n");

} // END OF readDigitalPotAttenuation



/********************************************************************************************************
* @brief Turn On or Off Channel 1 or 2 output relay.
*
* @author original: Hab Collector \n
*
* @note: Example Format for this command "Output 1 On" - Turns On Channel 1 output relay
* @note: Valid channels are 1 and 2, valid switch is On or Off
* @note: Command is case sensitive
*
* STEP 1: Get the channel number
* STEP 2: Read the pot value
********************************************************************************************************/
static void channelOutputSwitch(char *CommandLine)
{
    uint8_t ChannelNumber = 0;
    Type_RelaySwitch ChannelSwitch;

    // STEP 1: Get the channel number
    char *ChannelNumberChar = CommandLine + strlen(CHANNEL_OUTPUT_SWITCH) + 1;
    if (*ChannelNumberChar == '1')
        ChannelNumber = 1;
    else if (*ChannelNumberChar == '2')
        ChannelNumber = 2;
    else
    {
        printRed("Error: Channel number must be 1 or 2\r\n");
        return;
    }

    // STEP 2: Get the switch status
    char *SwitchStatusPointer = ChannelNumberChar + 1 + 1;
    if (strstr(SwitchStatusPointer, "On") != NULL)
        ChannelSwitch = ON;
    else if (strstr(SwitchStatusPointer, "Off") != NULL)
        ChannelSwitch = OFF;
    else
    {
        printRed("Error: Channel must be On or Off\r\n");
        return;
    }

    // STEP 3: Switch channel accordingly
    if (ChannelNumber == 1)
        (ChannelSwitch == ON)? CH1_OUTPUT_ENABLE() : CH1_OUTPUT_DISABLE();
    if (ChannelNumber == 2)
        (ChannelSwitch == ON)? CH2_OUTPUT_ENABLE() : CH2_OUTPUT_DISABLE();

    if (ChannelSwitch)
        printf("Channel On\r\n");
    else
        printf("Channel Off\r\n");

} // END OF channelOutputSwitch



/********************************************************************************************************
* @brief Display device status by channel
*
* @author original: Hab Collector \n
*
* @note: Example Format for this command "Status"
* @note: Command is case sensitive
*
* STEP 1: Display Channel 1 status
* STEP 2: Display Channel 2 status
********************************************************************************************************/
static void deviceStatus(char *CommandLine)
{
    NOT_USED(CommandLine);

    // STEP 1: Display Channel 1 status
    printBrightYellow("CH1:\r\n");
    printf("  Output Switch:   %d\r\n", ArbPwrBooster.CH1.OutputSwitch);
    printf("  Input Loading:   %d\r\n", ArbPwrBooster.CH1.InputImpedance);
    printf("  CC Limit Enable: %d\r\n", ArbPwrBooster.CH1.Limit.Enable);
    printf("  Current Limit:   %2.3fArms\r\n", ArbPwrBooster.CH1.Limit.Current);
    printf("  Input Volt RMS:  %2.3fVrms\r\n", ArbPwrBooster.CH1.Measure.RMS_Voltage);
    printf("  Input Divider:   %2.1f%%\r\n", (float)(ArbPwrBooster.CH1.PID->PotStep / ArbPwrBooster.CH1.PID->MaxStepValue) * 100.0);
    printf("  Input Attenuate: %2.3fVrms\r\n", ArbPwrBooster.CH1.Measure.RMS_Voltage * (float)(ArbPwrBooster.CH1.PID->PotStep / ArbPwrBooster.CH1.PID->MaxStepValue));
    printf("  Output Amp RMS:  %2.3fArms\r\n", ArbPwrBooster.CH1.Measure.RMS_Current);
    printf("  PID Kp:          %d\r\n", (int)ArbPwrBooster.CH1.PID->ProportionalGain);
    printf("  PID Ki:          %d\r\n", (int)ArbPwrBooster.CH1.PID->IntegralGain);
    printf("  PID Kd:          %d\r\n\n", (int)ArbPwrBooster.CH1.PID->DerivativeGain);

    // STEP 2: Display Channel 2 status
    printBrightMagenta("CH2:\r\n");
    printf("  Output Switch:   %d\r\n", ArbPwrBooster.CH2.OutputSwitch);
    printf("  Input Loading:   %d\r\n", ArbPwrBooster.CH2.InputImpedance);
    printf("  CC Limit Enable: %d\r\n", ArbPwrBooster.CH2.Limit.Enable);
    printf("  Current Limit:   %2.3fArms\r\n", ArbPwrBooster.CH2.Limit.Current);
    printf("  Input Volt RMS:  %2.3fVrms\r\n", ArbPwrBooster.CH2.Measure.RMS_Voltage);
    printf("  Input Divider:   %2.1f%%\r\n", (float)(ArbPwrBooster.CH2.PID->PotStep / ArbPwrBooster.CH2.PID->MaxStepValue) * 100.0);
    printf("  Input Attenuate: %2.3fVrms\r\n", ArbPwrBooster.CH2.Measure.RMS_Voltage * (float)(ArbPwrBooster.CH2.PID->PotStep / ArbPwrBooster.CH2.PID->MaxStepValue));
    printf("  Output Amp RMS:  %2.3fArms\r\n", ArbPwrBooster.CH2.Measure.RMS_Current);
    printf("  PID Kp:          %d\r\n", (int)ArbPwrBooster.CH2.PID->ProportionalGain);
    printf("  PID Ki:          %d\r\n", (int)ArbPwrBooster.CH2.PID->IntegralGain);
    printf("  PID Kd:          %d\r\n\n", (int)ArbPwrBooster.CH2.PID->DerivativeGain);

} // END OF deviceStatus



