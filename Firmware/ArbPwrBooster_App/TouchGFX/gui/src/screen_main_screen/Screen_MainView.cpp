#include <gui/screen_main_screen/Screen_MainView.hpp>

// ADDED INCLUDES
#include "Main_Support.h"
#include "IO_Support.h"


Screen_MainView::Screen_MainView()
{

}

void Screen_MainView::setupScreen()
{
    Screen_MainViewBase::setupScreen();
}

void Screen_MainView::tearDownScreen()
{
    Screen_MainViewBase::tearDownScreen();
}


// ADDED FUNCTIONS
void Screen_MainView::update_Screen_Main(void)
{
    ArbPwrBoosterStatus.Screen = MAIN_SCREEN;

    // Update Current Set Values for channel1
    Unicode::snprintfFloat(textArea_CH1_CurrentSetBuffer, TEXTAREA_CH1_CURRENTSET_SIZE, "%05.3f", ArbPwrBoosterStatus.CH1.CurrentLimit);
    textArea_CH1_CurrentSet.setWildcard(textArea_CH1_CurrentSetBuffer);
    textArea_CH1_CurrentSet.invalidate();

    // Update Current Set Values for channel2
    Unicode::snprintfFloat(textArea_CH2_CurrentSetBuffer, TEXTAREA_CH2_CURRENTSET_SIZE, "%06.3f", ArbPwrBoosterStatus.CH2.CurrentLimit);
    textArea_CH2_CurrentSet.setWildcard(textArea_CH2_CurrentSetBuffer);
    textArea_CH2_CurrentSet.invalidate();

}
