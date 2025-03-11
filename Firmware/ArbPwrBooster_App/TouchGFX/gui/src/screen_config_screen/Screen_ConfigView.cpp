#include <gui/screen_config_screen/Screen_ConfigView.hpp>

// ADDED INCLUDES
#include "touchgfx/Color.hpp"
#include "../../../../../../touchgfx/generated/texts/include/texts/TextKeysAndLanguages.hpp"
#include "Main_Support.h"
#include "IO_Support.h"


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
* @note: ±20V Supply rails are updated by the listener on a periodic interval
*
* STEP 1: Set the active screen
* STEP 2: Update FW Revision
* STEP 3: Update HW Revision
* STEP 4: Update Device Status
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

    // STEP 4: Update Device Status
    // HAB TODO

} // END OF update_Screen_Config



void Screen_ConfigView::updateConfigScreen_View(void)
{
    // Power +20V
    Unicode::snprintfFloat(textArea_PosSupplyMonitorBuffer, TEXTAREA_POSSUPPLYMONITOR_SIZE, "%3.1f", ArbPwrBooster.SystemMeasure.Positive_20V);
    textArea_PosSupplyMonitor.setWildcard(textArea_PosSupplyMonitorBuffer);
    // Power -20V
    Unicode::snprintfFloat(textArea_NegSupplyMonitorBuffer, TEXTAREA_NEGSUPPLYMONITOR_SIZE, "%3.1f", ArbPwrBooster.SystemMeasure.Negative_20V);
    textArea_NegSupplyMonitor.setWildcard(textArea_NegSupplyMonitorBuffer);
    // System 3V3
    Unicode::snprintfFloat(textArea_System3V3MonitorBuffer, TEXTAREA_SYSTEM3V3MONITOR_SIZE, "%4.3f", ArbPwrBooster.SystemMeasure.VDD_VDREF);
    textArea_System3V3Monitor.setWildcard(textArea_System3V3MonitorBuffer);
    // System 3V3
    Unicode::snprintfFloat(textArea_SystemTempMonitorBuffer, TEXTAREA_SYSTEMTEMPMONITOR_SIZE, "%3.1f", ArbPwrBooster.SystemMeasure.TempDegreeC);
    textArea_SystemTempMonitor.setWildcard(textArea_SystemTempMonitorBuffer);
    // System Status
    Unicode::strncpy(textArea_SystemStatusMsgBuffer, "Im good", TEXTAREA_SYSTEMSTATUSMSG_SIZE);
//    Unicode::snprintf(textArea_SystemStatusMsgBuffer, TEXTAREA_SYSTEMSTATUSMSG_SIZE, "%s", "I'm good");
    textArea_SystemStatusMsg.setWildcard(textArea_SystemStatusMsgBuffer);

    this->invalidate();

}
