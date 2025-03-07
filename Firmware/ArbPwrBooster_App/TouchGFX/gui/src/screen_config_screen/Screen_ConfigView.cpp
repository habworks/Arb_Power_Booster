#include <gui/screen_config_screen/Screen_ConfigView.hpp>

// ADDED INCLUDES
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
