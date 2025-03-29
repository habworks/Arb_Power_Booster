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
#include "cmsis_os2.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


static void callDebugFunction(void);
static void resetDebugCommandSearch(void);
// Actionable Commands
static void printDebugConsoleHelp(void);
static void setDigitalPotAttenuation(char *CommandLine);

static char CharsToProcessBuffer[10] = {0};
static char DebugCommand[100] = {0};
static uint8_t DebugCommandIndex = 0;





void debugConsoleTaskActions(void)
{
    if (UART_DebugPort.Rx_FIFO_Count == 0)
        return;

    memset(CharsToProcessBuffer, 0x00, sizeof(CharsToProcessBuffer));
    memcpy(CharsToProcessBuffer, UART_DebugPort.Rx_FIFO, UART_DebugPort.Rx_FIFO_Count);
    uint8_t CharsToProcess = UART_DebugPort.Rx_FIFO_Count;
    UART_DebugPort.Rx_FIFO_Count = 0;
    UART_DebugPort.Parser(CharsToProcessBuffer, CharsToProcess);

    // STEP X: Delay based on how fast a person can type
    osDelay(150);
}

void debugPortParser(char *CharacterBuffer, uint8_t NumberOfChars)
{
    for (uint8_t Index = 0; Index < NumberOfChars; Index++)
    {
        if ((CharacterBuffer[Index] != '\r') && (CharacterBuffer[Index] != '\b') && (CharacterBuffer[Index] != '?'))
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


        if (CharacterBuffer[Index] == '\r')
        {
            printf("\r\n");
            fflush(stdout);
            callDebugFunction();
        }

        if (CharacterBuffer[Index] == '\b')
        {
            DebugCommand[DebugCommandIndex] = 0x00;
            DebugCommandIndex = (DebugCommandIndex == 0)? 0 : (DebugCommandIndex - 1);
            terminal_CursorBackSpace();
            printf(" ");
            terminal_CursorBackSpace();
            fflush(stdout);
        }

        if (CharacterBuffer[Index] == '?')
        {
            printDebugConsoleHelp();
            resetDebugCommandSearch();
            return;
        }

    }
} // END OF debugPortParser

void commandPrompt(void)
{
    printf("Command> ");
}


static void callDebugFunction(void)
{
    bool CommandFound = true;
    if (strstr(DebugCommand, SET_DIGITAL_POT_ATTEN) != NULL)
    {
        setDigitalPotAttenuation(DebugCommand);
    }
    else if (strstr(DebugCommand, DEBUG_CONSOLE_HELP != NULL))
    {
        printDebugConsoleHelp();
    }
    else
    {
        CommandFound = false;
    }

    if ((!CommandFound) && (DebugCommandIndex != 0))
        printYellow("No such command\r\n");
    resetDebugCommandSearch();

}


static void resetDebugCommandSearch(void)
{
    memset(DebugCommand, 0x00, sizeof(DebugCommand));
    DebugCommandIndex = 0;
    commandPrompt();
    fflush(stdout);
}


static void setDigitalPotAttenuation(char *CommandLine)
{
//    char StringValue[4] = {0};
//    uint8_t Index = 0;
//    char *StringValuePointer = DebugCommand + strlen(SET_DIGITAL_POT_ATTEN) + 1;
//    while (*StringValuePointer != '\r')
//    {
//        StringValue[Index] = *StringValuePointer;
//        StringValuePointer++;
//        Index++;
//    }
//    int Value = atoi(StringValue);
//    if ((Value > 100) || (Index > 3))
//    {
//        printf("Error: Bad command format..\r\n");
//        printf("Format: \"%s X\" Where X is 1 to 100\r\n", SET_DIGITAL_POT_ATTEN);
//        return;
//    }

//    printf("Pot value set to: %d\r\n", Value);
    printf("Test of setting dpot\r\n");

}

static void printDebugConsoleHelp(void)
{
    printf("\r\nDEBUG COMMANDS: \r\n");
    printf("  Set Pot X: Where X is value 0 to 255 of wiper\r\n");
}



