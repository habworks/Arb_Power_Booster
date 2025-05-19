/** ****************************************************************************************************
 * @file 			Main_Support.c
 * @brief			Functions that support the Arbitrary Power Booster Hardware
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
#include "Main_Support.h"
#include "ADC_Support.h"
#include "IO_Support.h"
#include "UART_Support.h"
#include "Debug_Port.h"
#include "Terminal_Emulator_Support.h"
#include "MCP45HVX1_Driver.h"
#include "cmsis_os2.h"
#include "ff.h"
#include <String.h>
#include <stdlib.h>
#include <stdio.h>

extern osSemaphoreId_t DisplayUpdateSemaphoreHandle;
extern FATFS SDFatFS;
extern char SDPath[4];

// GLOBALS
Type_ArbPwrBoosterStatus ArbPwrBooster;



/********************************************************************************************************
* @brief Init of the Arbitrary Power Booster hardware peripherals and drivers for use in POR state
*
* @author original: Hab Collector \n
*
* STEP 1: Init hardware peripherals
* STEP 2: Init drivers
********************************************************************************************************/
void Init_ArbPwrBoosterHardware(void)
{
    // STEP 1: Init hardware peripherals
    Init_GPIO_Hardware();
    Init_ADC_Hardware();
    Init_UART_Hardware();

    // STEP 2: Init drivers
    Init_MCP45HVX1();

} // END OF Init_ArbPwrBoosterHardware



/********************************************************************************************************
* @brief Init of the Arbitrary Power Booster class - set member values to defaults for POR operation
*
* @author original: Hab Collector \n
*
* STEP 1: Set members to default POR value
********************************************************************************************************/
void Init_ArbPwrBoosterClass(void)
{
    // STEP 1: Set members to default POR value
    ArbPwrBooster.Ready = false;
    ArbPwrBooster.ActiveChannel = CHANNEL_1;
    ArbPwrBooster.Screen = SPLASH_SCREEN;
    // Channel 1 Init
    ArbPwrBooster.CH1.InputImpedance = ONE_MEG_OHM;
    ArbPwrBooster.CH1.OutputSwitch = OFF;
    ArbPwrBooster.CH1.Limit.Enable = true;
    ArbPwrBooster.CH1.Limit.Current = 0.300;
    ArbPwrBooster.CH1.Measure.ResetCurrentMinMax = true;
    ArbPwrBooster.CH1.Measure.MaxCurrent = 0;
    ArbPwrBooster.CH1.Measure.MinCurrent = 0;
    ArbPwrBooster.CH1.Measure.RMS_CurrentFunction = update_RMS_Value;
    ArbPwrBooster.CH1.Measure.ResetVoltageMinMax = true;
    ArbPwrBooster.CH1.Measure.MinVoltage = 0;
    ArbPwrBooster.CH1.Measure.MaxVoltage = 0;
    ArbPwrBooster.CH1.Measure.RMS_VoltageFunction = update_RMS_Value;
    // Channel 2 Init
    ArbPwrBooster.CH2.InputImpedance = ONE_MEG_OHM;
    ArbPwrBooster.CH2.OutputSwitch = OFF;
    ArbPwrBooster.CH2.Limit.Enable = true;
    ArbPwrBooster.CH2.Limit.Current = 1.250;
    ArbPwrBooster.CH2.Measure.ResetCurrentMinMax = true;
    ArbPwrBooster.CH2.Measure.MaxCurrent = 0;
    ArbPwrBooster.CH2.Measure.MinCurrent = 0;
    ArbPwrBooster.CH2.Measure.RMS_CurrentFunction = update_RMS_Value;
    ArbPwrBooster.CH2.Measure.ResetVoltageMinMax = true;
    ArbPwrBooster.CH2.Measure.MinVoltage = 0;
    ArbPwrBooster.CH2.Measure.MaxVoltage = 0;
    ArbPwrBooster.CH2.Measure.RMS_VoltageFunction = update_RMS_Value;

} // END OF Init_ArbPwrBoosterClass



/********************************************************************************************************
* @brief Convert the a rational number in the form of A.xyz into its unique components of A, x, y, z
*   A = Integer
*   x = Tenths
*   y = Hundredths
*   z = Thousandths
* Example Rational number 3.1429 becomes A = 3, x = 1, y = 4, z = 3
*
* @author original: Hab Collector \n
*
* @note: Values are rounded up to the nearest thousandths
* @note: Integer, Tenths, Hundredths, Thousandths are returned by reference
*
* @param RationalNumber: Value to be converted
* @param Integer: Said place holder within a rational number
* @param Tenths: Said place holder within a rational number
* @param Hundredths: Said place holder within a rational number
* @param Thousandths: Said place holder within a rational number
*
* STEP 1: Locate the decimal by pointer
* STEP 2: Extract Integer Part
* STEP 3: Extract Tenths
* STEP 4: Extract Hundredths
* STEP 5: Extract Thousandths
********************************************************************************************************/
void digitsFromDouble(double RationalNumber, int8_t *Integer, uint8_t *Tenths, uint8_t *Hundredths, uint8_t *Thousandths)
{
    char ValueString[10] = {0};
    char ExtractedString[3] = {0};
    char *DecimalLocation;

    // STEP 1: Locate the decimal by pointer
    sprintf(ValueString, "%2.3f", RationalNumber);
    DecimalLocation = strstr(ValueString, ".");

    // STEP 2: Extract Integer Part
    uint8_t StringLength = strlen(ValueString);
    for (uint8_t Index = 0; Index < StringLength; Index++)
    {
        if (ValueString[Index] == '.')
            break;
        ExtractedString[Index] = ValueString[Index];
    }
    *Integer = atoi(ExtractedString);

    // STEP 3: Extract Tenths
    memset(ExtractedString, 0x00, sizeof(ExtractedString));
    char *ValueLocationTenths = DecimalLocation + 1;
    ExtractedString[0] = *ValueLocationTenths;
    *Tenths = atoi(ExtractedString);

    // STEP 4: Extract Hundredths
    char *ValueLocationHundredths = DecimalLocation + 2;
    ExtractedString[0] = *ValueLocationHundredths;
    *Hundredths = atoi(ExtractedString);

    // STEP 5: Extract Thousandths
    char *ValueLocationThousandths = DecimalLocation + 3;
    ExtractedString[0] = *ValueLocationThousandths;
    *Thousandths = atoi(ExtractedString);

} // END OF digitsFromDouble



/********************************************************************************************************
* @brief Convert the integer values of A, x, y, z to a rational number of value A.xyz where
*   A = Integer
*   x = Tenths
*   y = Hundredths
*   z = Thousandths
* Example digits A = 3, x = 1, y = 4, z = 3 becomes rational number 3.1429
* @author original: Hab Collector \n
*
* @param Integer: Said place holder within a rational number
* @param Tenths: Said place holder within a rational number
* @param Hundredths: Said place holder within a rational number
* @param Thousandths: Said place holder within a rational number
*
* @return Rational Number
*
* STEP 1: Concatenate values into a rational number
********************************************************************************************************/
double digitsToDouble(int8_t *Integer, uint8_t *Tenths, uint8_t *Hundredths, uint8_t *Thousandths)
{
    // STEP 1: Concatenate values into a rational number
    double Value = (double)abs(*Integer);
    Value += *Tenths * 0.100;
    Value += *Hundredths * 0.010;
    Value += *Thousandths * 0.001;
    if (*Integer < 0)
        Value *= -1.0;

    return(Value);

} // END OF digitsToDouble



/********************************************************************************************************
* @brief Error handler for critical errors ONLY.  A critical error has no recovery.  The error handler is
* used to trap the system so the call stack and offending condition can be evaluated.
*
* @author original: Hab Collector \n
*
* @param FileName: Filename where error occur - calling function must use __FILE__
* @param FileLineNumber: Line number within in the file were the error occurred - call function must use __LINE__
* @param ErrorNumber: An number value representing some condition about the error
* @param Description: A brief description of the error - do not include \r\n
*
* STEP 1: Print out error information
* STEP 2: Trap here forever
********************************************************************************************************/
void systemErrorHandler(char *FileName, int FileLineNumber, uint32_t ErrorNumber, char *Description)
{
    bool WaitHere = true;

    // STEP 1: Print out error information
    printf("CRITICAL ERROR:");
    printf("\tError Number:     %d\r\n", (int)ErrorNumber);
    printf("\tDescription:      %s\r\n", Description);
    printf("\tFile Name:        %s\r\n", FileName);
    printf("\tFile Line Number: %d", (int)FileLineNumber);

    // STEP 2: Trap here forever
    while(WaitHere);

} // END OF systemErrorHandler



/********************************************************************************************************
* @brief Pre-actions to be performed before mainUpdateTaskActions - These actions are performed only once.
*
* @author original: Hab Collector \n
*
* STEP 1: Load POR Configuration from file
* STEP 2: Clear the debug port terminal and display entry information
********************************************************************************************************/
void mainUpdateTaskInit(void)
{
    // STEP 1: Load POR Configuration from file
    Type_ConfigParameterStatus ConfigLoadStatus = loadConfigParameters();

    // STEP 2: Clear the debug port terminal and display entry information
    terminal_ClearScreen();
    printGreen("IMR Engineering, LLC\r\n");
    printf("  Hab Collector, Principal Engineer\r\n");
    printf("  www.imrengineering.com\r\n");
    printBrightRed("Arbitrary Power Booster\r\n");
    printf("  HW REV: %d\r\n", HW_REVISION);
    printf("  FW REV: %d.%d.%d\r\n\n", FW_MAJOR_REV, FW_MINOR_REV, FW_TEST_REV);
    if (ConfigLoadStatus == CONFIG_FILE_ERROR)
        printYellow("WARNING: No access to uSD card\r\n");
    printf("For assistance type Help or just ?\r\n");
    commandPrompt();
    fflush(stdout);

} // END OF mainUpdateTaskInit



/********************************************************************************************************
* @brief These are the actions taken by thread mainUpdateTask().  Actions are primarily related to live
* updates of the Main and Config screens.  The main power is turned on here but monitored elsewhere. A secondary
* function of re-staring the non-contineous DMA ADC1 is also performed.  These actions should only be performed
* when the ArbPwrStatus is Ready.
*
* @author original: Hab Collector \n
*
* STEP 1: Do nothing until introduction splash screen completed
* STEP 2: Turn on Main Power
* STEP 3: Restart ADC1 DMA conversion
* STEP 4: Set semaphore to update the active display
* STEP 5: Make task inactive for a period of time - based on desired GUI update speed
********************************************************************************************************/
void mainUpdateTaskActions(void)
{
    static bool FirstTimeReady = true;

    // STEP 1: Do nothing until introduction splash screen completed
    if (ArbPwrBooster.Ready)
    {
        // STEP 2: Turn on Main Power
        if (FirstTimeReady)
        {
            MAIN_PWR_ON();
            FirstTimeReady = false;
        }

        // STEP 3: Restart ADC1 DMA conversion
        ADC1_StartConversion();

        // STEP 4: Set semaphore to update the active display
        if ((ArbPwrBooster.Screen == MAIN_SCREEN) || (ArbPwrBooster.Screen == CONFIG_SCREEN))
            osSemaphoreRelease(DisplayUpdateSemaphoreHandle);
    }

    // STEP 5: Make task inactive for a period of time - based on desired GUI update speed
    osDelay(GUI_UPDATE_RATE);

} // END OF mainUpdateTaskActions



/********************************************************************************************************
* @brief Intended for POR load the configuration parameters from the configuration file.  The configuration
* file is a binary file that holds the configurable parameters for both channels.  Parameters include:
*   Input Impedance
*   Current Limit Enable
*   Current Limit Value
* If the file is not present it will be created with the default.  If the file is present, the parameter
* values will be loaded as such.  Should an error occur during the loading or creation of a file the default
* values are used.
*
* @author original: Hab Collector \n
*
* @note: Drive use will be minimal - for safety the drive will be unmounted when not active
* @note: The configuration file is stored on the root path of the uSD
* @note: The order of save must match the order of recall
*
* @return: Status of the operation - should be either CONFIG_CREATE_NEW or CONFIG_LOAD_OK if things went well
*
* STEP 1: Mount the drive
* STEP 2: Open Config File
* STEP 3: Take action to create if the file does not exist
* STEP 4: Take action to read file contents when it does exist
* STEP 5: An error occurred
* STEP 6: Sync and close file, unmount and return
********************************************************************************************************/
Type_ConfigParameterStatus loadConfigParameters(void)
{
    FRESULT FileStatus;
    FIL ConfigFileObject;
    uint8_t ConfigParameterTotalBytes = 20;
    Type_ConfigParameterStatus ConfigLoadStatus = CONFIG_FILE_ERROR;

    // STEP 1: Mount the drive
    FileStatus = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
    if (FileStatus != FR_OK)
    {
        printYellow("WARNING: Could not mount uSD Drive\r\n");
        return(CONFIG_FILE_ERROR);
    }

    // STEP 2: Open Config File
    FileStatus = f_open(&ConfigFileObject, CONFIG_FILENAME, FA_READ);

    // STEP 3: Take action to create if the file does not exist
    if (FileStatus == FR_NO_FILE)
    {
        printf("Config file does not exist - create and load defaults\r\n");
        FileStatus = f_open(&ConfigFileObject, CONFIG_FILENAME, FA_CREATE_ALWAYS | FA_WRITE);
        if (FileStatus == FR_OK)
        {
            UINT BytesWritten = 0;
            uint16_t TotalBytesWritten = 0;
            Type_Double CurrentLimit;
            // Set Channel 1 POR Defaults
            Init_ArbPwrBoosterClass();
            // Write Channel 1 config parameters: Order of write must correspond with subsequent order of read
            f_write(&ConfigFileObject, &ArbPwrBooster.CH1.InputImpedance, 1, &BytesWritten);
            TotalBytesWritten += BytesWritten;
            f_write(&ConfigFileObject, &ArbPwrBooster.CH1.Limit.Enable, 1, &BytesWritten);
            TotalBytesWritten += BytesWritten;
            CurrentLimit.Value = ArbPwrBooster.CH1.Limit.Current;
            f_write(&ConfigFileObject, &CurrentLimit.Byte0, sizeof(double), &BytesWritten);
            TotalBytesWritten += BytesWritten;
            // Write Channel 2 config parameters: Order of write must correspond with subsequent order of read
            f_write(&ConfigFileObject, &ArbPwrBooster.CH2.InputImpedance, 1, &BytesWritten);
            TotalBytesWritten += BytesWritten;
            f_write(&ConfigFileObject, &ArbPwrBooster.CH2.Limit.Enable, 1, &BytesWritten);
            TotalBytesWritten += BytesWritten;
            CurrentLimit.Value = ArbPwrBooster.CH2.Limit.Current;
            f_write(&ConfigFileObject, &CurrentLimit.Byte0, sizeof(double), &BytesWritten);
            TotalBytesWritten += BytesWritten;
            printf("Bytes Written to create config file = %d\r\n", (int)TotalBytesWritten);
            ConfigLoadStatus = (TotalBytesWritten == ConfigParameterTotalBytes)? CONFIG_CREATE_NEW : CONFIG_FILE_ERROR;
        }
    }

    // STEP 4: Take action to read file contents when it does exist
    else if (FileStatus == FR_OK)
    {
        UINT BytesRead = 0;
        uint16_t TotalBytesRead = 0;
        Type_Double CurrentLimit;
        f_lseek(&ConfigFileObject, 0);
        // Read Channel 1 config parameters: Order of read must correspond to previous order of write
        f_read(&ConfigFileObject, &ArbPwrBooster.CH1.InputImpedance, 1, &BytesRead);
        TotalBytesRead += BytesRead;
        f_read(&ConfigFileObject, &ArbPwrBooster.CH1.Limit.Enable, 1, &BytesRead);
        TotalBytesRead += BytesRead;
        f_read(&ConfigFileObject, &CurrentLimit.Byte0, sizeof(double), &BytesRead);
        ArbPwrBooster.CH1.Limit.Current = CurrentLimit.Value;
        TotalBytesRead += BytesRead;
        // Read Channel 2 config parameters: Order of read must correspond to previous order of write
        f_read(&ConfigFileObject, &ArbPwrBooster.CH2.InputImpedance, 1, &BytesRead);
        TotalBytesRead += BytesRead;
        f_read(&ConfigFileObject, &ArbPwrBooster.CH2.Limit.Enable, 1, &BytesRead);
        TotalBytesRead += BytesRead;
        f_read(&ConfigFileObject, &CurrentLimit.Byte0, sizeof(double), &BytesRead);
        ArbPwrBooster.CH2.Limit.Current = CurrentLimit.Value;
        TotalBytesRead += BytesRead;
        printf("Total Bytes Read = %d\r\n", (int)TotalBytesRead);
        ConfigLoadStatus = (TotalBytesRead == ConfigParameterTotalBytes)? CONFIG_LOAD_OK : CONFIG_FILE_ERROR;
    }

    // STEP 5: An error occurred
    else
    {
        printYellow("WARNING: Could not create or read config file - using defaults\r\n");
        Init_ArbPwrBoosterClass();
        ConfigLoadStatus = CONFIG_FILE_ERROR;
    }

    // STEP 6: Sync and close file, unmount and return
    f_sync(&ConfigFileObject);
    f_close(&ConfigFileObject);
    f_mount(0, (TCHAR const*)SDPath, 0);
    return(ConfigLoadStatus);

} // END OF loadConfigStatus



/********************************************************************************************************
* @brief Save the configuration file.  Intended for when there is a parameter change.  The configuration
* file is a binary file that holds the configurable parameters for both channels.  The order of save must
* match that of function loadConfigStatus().  See function loadConfigStatus header notes for more details
*
* @author original: Hab Collector \n
*
* @note: Drive use will be minimal - for safety the drive will be unmounted when not active
* @note: The configuration file is stored on the root path of the uSD
*
* @return: Status of the operation, CONFIG_SAVE_OK if all is well
*
* STEP 1: Mount the drive
* STEP 2: Open Config File for updating with config parameters and save
* STEP 3: Sync and close file, unmount and return
********************************************************************************************************/
Type_ConfigParameterStatus saveConfigParameters(void)
{
    FRESULT FileStatus;
    FIL ConfigFileObject;
    uint8_t ConfigParameterTotalBytes = 20;
    Type_ConfigParameterStatus ConfigSaveStatus = CONFIG_FILE_ERROR;

    // STEP 1: Mount the drive
    FileStatus = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
    if (FileStatus != FR_OK)
    {
        printYellow("WARNING: Could not mount uSD Drive\r\n");
        return(CONFIG_FILE_ERROR);
    }

    // STEP 2: Open Config File for updating with config parameters and save
    FileStatus = f_open(&ConfigFileObject, CONFIG_FILENAME, FA_CREATE_ALWAYS | FA_WRITE);
    if (FileStatus == FR_OK)
    {
        UINT BytesWritten = 0;
        uint16_t TotalBytesWritten = 0;
        Type_Double CurrentLimit;
        // Write Channel 1 config parameters: Order of write must correspond with subsequent order of read
        f_write(&ConfigFileObject, &ArbPwrBooster.CH1.InputImpedance, 1, &BytesWritten);
        TotalBytesWritten += BytesWritten;
        f_write(&ConfigFileObject, &ArbPwrBooster.CH1.Limit.Enable, 1, &BytesWritten);
        TotalBytesWritten += BytesWritten;
        CurrentLimit.Value = ArbPwrBooster.CH1.Limit.Current;
        f_write(&ConfigFileObject, &CurrentLimit.Byte0, sizeof(double), &BytesWritten);
        TotalBytesWritten += BytesWritten;
        // Write Channel 2 config parameters: Order of write must correspond with subsequent order of read
        f_write(&ConfigFileObject, &ArbPwrBooster.CH2.InputImpedance, 1, &BytesWritten);
        TotalBytesWritten += BytesWritten;
        f_write(&ConfigFileObject, &ArbPwrBooster.CH2.Limit.Enable, 1, &BytesWritten);
        TotalBytesWritten += BytesWritten;
        CurrentLimit.Value = ArbPwrBooster.CH2.Limit.Current;
        f_write(&ConfigFileObject, &CurrentLimit.Byte0, sizeof(double), &BytesWritten);
        TotalBytesWritten += BytesWritten;
        printf("Bytes Written to config file = %d\r\n", (int)TotalBytesWritten);
        ConfigSaveStatus = (TotalBytesWritten == ConfigParameterTotalBytes)? CONFIG_SAVE_OK : CONFIG_FILE_ERROR;
    }

    f_sync(&ConfigFileObject);
    f_close(&ConfigFileObject);
    f_mount(0, (TCHAR const*)SDPath, 0);
    return(ConfigSaveStatus);

} // END OF saveConfigParameters


void switchOnAction_CH1(void)
{
    fflush(stdout);
    CH1_OUTPUT_ENABLE();
    ArbPwrBooster.CH1.Measure.ResetCurrentMinMax = true;
    ArbPwrBooster.CH1.Measure.ResetVoltageMinMax = true;
    PID_Reset(ArbPwrBooster.CH1.PID);
    MCP45HVX1_WriteWiperVerify(&hi2c1, A1A0_EXTERNAL_ADDR_CH1, MCP45HVX1_POT_FULL_RESOLUTION);
    ArbPwrBooster.CH1.OutputSwitch = ON;
}

void switchOffAction_CH1(void)
{
    CH1_OUTPUT_DISABLE();
    ArbPwrBooster.CH1.OutputSwitch = OFF;
    ArbPwrBooster.CH1.PID->Enable = false;
    MCP45HVX1_WriteWiperVerify(&hi2c1, A1A0_EXTERNAL_ADDR_CH1, MCP45HVX1_POT_FULL_RESOLUTION);
}


