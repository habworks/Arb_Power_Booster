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
void Screen_ConfigView::update_Screen_Config(void)
{
    ArbPwrBoosterStatus.Screen = CONFIG_SCREEN;

    // Update FW Revision
    Unicode::snprintf(textArea_FW_RevBuffer, TEXTAREA_FW_REV_SIZE, "%d.%d.%d", FW_MAJOR_REV, FW_MINOR_REV, FW_TEST_REV);
    textArea_FW_Rev.setWildcard(textArea_FW_RevBuffer);
    textArea_FW_Rev.invalidate();
    // Update HW Revision
    Unicode::snprintf(textArea_HW_RevBuffer, TEXTAREA_HW_REV_SIZE, "%d", HW_REVISION);
    textArea_HW_Rev.setWildcard(textArea_HW_RevBuffer);
    textArea_HW_Rev.invalidate();
}
