#include <gui/screen_config_screen/Screen_ConfigView.hpp>

// ADDED INCLUDES
#include "touchgfx/Color.hpp"
#include "../../../../../../touchgfx/generated/texts/include/texts/TextKeysAndLanguages.hpp"
#include "Main_Support.h"
#include "IO_Support.h"
#include "ADC_Support.h"
#include <string.h>


Screen_ConfigView::Screen_ConfigView()
{

}

void Screen_ConfigView::setupScreen()
{
    Screen_ConfigViewBase::setupScreen();
}

void Screen_ConfigView::tearDownScreen()
{
    Screen_ConfigViewBase::tearDownScreen();
}

// ADDED FUNCTIONS
/********************************************************************************************************
* @brief Update the screen on entry.  It is called by the end of the Config Screen transition interaction.
*
* @author original: Hab Collector \n
*
* @note: Called at the end of the config screen transition
* @note: ±VS, 3V3, Temp and device status are updated by the listener on a periodic interval
*
* STEP 1: Set the active screen
* STEP 2: Update FW Revision
* STEP 3: Update HW Revision
********************************************************************************************************/
void Screen_ConfigView::update_Screen_Config(void)
{
    // STEP 1: Set the active screen
    ArbPwrBooster.Screen = CONFIG_SCREEN;

    // STEP 2: Update FW Revision
    Unicode::snprintf(textArea_FW_RevBuffer, TEXTAREA_FW_REV_SIZE, "%d.%d.%d", FW_MAJOR_REV, FW_MINOR_REV, FW_TEST_REV);
    textArea_FW_Rev.setWildcard(textArea_FW_RevBuffer);
    textArea_FW_Rev.invalidate();

    // STEP 3: Update HW Revision
    Unicode::snprintf(textArea_HW_RevBuffer, TEXTAREA_HW_REV_SIZE, "%d", HW_REVISION);
    textArea_HW_Rev.setWildcard(textArea_HW_RevBuffer);
    textArea_HW_Rev.invalidate();

} // END OF update_Screen_Config



/********************************************************************************************************
* @brief Live update of the config screen.  All system variables: +VS, -VS, Temperature and reference
* voltage are updated.
*
* @author original: Hab Collector \n
*
* @note: Screen is updated by the semaphore in the model
*
* STEP 1: Check and update for config error conditions
* STEP 2: Update displayed values: +VS, -VS, Vref, and Temp
* STEP 3: Display System Status message
********************************************************************************************************/
void Screen_ConfigView::updateConfigScreen_View(void)
{
    char StatusMsg[TEXTAREA_SYSTEMSTATUSMSG_SIZE] = {0x00};
    uint8_t ConfigError = 0;

    // STEP 1: Check and update for config error conditions
    systemMeasureWithinLimits(StatusMsg, &ConfigError);

    // STEP 2: Update displayed values: +VS, -VS, Vref, and Temp
    // Power +VS
    Unicode::snprintfFloat(textArea_PosSupplyMonitorBuffer, TEXTAREA_POSSUPPLYMONITOR_SIZE, "%3.1f", ArbPwrBooster.SystemMeasure.Positive_VS);
    textArea_PosSupplyMonitor.setWildcard(textArea_PosSupplyMonitorBuffer);
    if (ConfigError & CONFIG_POS_20V_ERROR)
        textArea_PosSupplyMonitor.setColor(touchgfx::Color::getColorFromRGB(STATUS_MSG_ERROR_COLOR));
    else
        textArea_PosSupplyMonitor.setColor(touchgfx::Color::getColorFromRGB(STATUS_MSG_OK_COLOR));
    // Power -VS
    Unicode::snprintfFloat(textArea_NegSupplyMonitorBuffer, TEXTAREA_NEGSUPPLYMONITOR_SIZE, "%3.1f", ArbPwrBooster.SystemMeasure.Negative_VS);
    textArea_NegSupplyMonitor.setWildcard(textArea_NegSupplyMonitorBuffer);
    if (ConfigError & CONFIG_NEG_20V_ERROR)
        textArea_NegSupplyMonitor.setColor(touchgfx::Color::getColorFromRGB(STATUS_MSG_ERROR_COLOR));
   else
       textArea_NegSupplyMonitor.setColor(touchgfx::Color::getColorFromRGB(STATUS_MSG_OK_COLOR));
    // System 3V3
    Unicode::snprintfFloat(textArea_System3V3MonitorBuffer, TEXTAREA_SYSTEM3V3MONITOR_SIZE, "%4.3f", ArbPwrBooster.SystemMeasure.VDD_VDREF);
    textArea_System3V3Monitor.setWildcard(textArea_System3V3MonitorBuffer);
    if (ConfigError & CONFIG_VREF_ERROR)
        textArea_System3V3Monitor.setColor(touchgfx::Color::getColorFromRGB(STATUS_MSG_ERROR_COLOR));
   else
       textArea_System3V3Monitor.setColor(touchgfx::Color::getColorFromRGB(STATUS_MSG_OK_COLOR));
    // System Temp
    Unicode::snprintfFloat(textArea_SystemTempMonitorBuffer, TEXTAREA_SYSTEMTEMPMONITOR_SIZE, "%3.1f", ArbPwrBooster.SystemMeasure.TempDegreeC);
    textArea_SystemTempMonitor.setWildcard(textArea_SystemTempMonitorBuffer);
    if (ConfigError & CONFIG_TEMP_ERROR)
        textArea_SystemTempMonitor.setColor(touchgfx::Color::getColorFromRGB(STATUS_MSG_ERROR_COLOR));
   else
       textArea_SystemTempMonitor.setColor(touchgfx::Color::getColorFromRGB(STATUS_MSG_OK_COLOR));

    // STEP 3: Display System Status message
    Unicode::UnicodeChar UnicodeBuffer[TEXTAREA_SYSTEMSTATUSMSG_SIZE];
    Unicode::fromUTF8((const uint8_t*)StatusMsg, UnicodeBuffer, TEXTAREA_SYSTEMSTATUSMSG_SIZE);
    Unicode::strncpy(textArea_SystemStatusMsgBuffer, UnicodeBuffer, TEXTAREA_SYSTEMSTATUSMSG_SIZE);
    textArea_SystemStatusMsg.setWildcard(textArea_SystemStatusMsgBuffer);
    if (ConfigError == CONFIG_NO_ERROR)
        textArea_SystemStatusMsg.setColor(touchgfx::Color::getColorFromRGB(STATUS_MSG_OK_COLOR));
    else
        textArea_SystemStatusMsg.setColor(touchgfx::Color::getColorFromRGB(STATUS_MSG_ERROR_COLOR));

    this->invalidate();

} // END OF updateConfigScreen_View
