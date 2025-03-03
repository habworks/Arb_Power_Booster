#include <gui/screen_main_screen/Screen_MainView.hpp>

// ADDED INCLUDES
#include <touchgfx/Color.hpp>
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


void Screen_MainView::switchOutput_CH1(void)
{
    // HAB TODO: May want to add some pre-qualifers to turning on output
    // Toggle the output relay ON / OFF
    if (ArbPwrBoosterStatus.CH1.OutputSwitch == OFF)
    {
        CH1_OUTPUT_ENABLE();
        box_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(0x5E, 0xEB, 0x13));
        box_CH1_Enable.setAlpha(255);
        textArea_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(0xFF, 0xFF, 0xFF));
        ArbPwrBoosterStatus.CH1.OutputSwitch = ON;
    }
    else
    {
        CH1_OUTPUT_DISABLE();
        box_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(0xA7, 0xC4, 0xB2));
        box_CH1_Enable.setAlpha(50);
        textArea_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(0x70, 0x70, 0x70));
        ArbPwrBoosterStatus.CH1.OutputSwitch = OFF;
    }
    box_CH1_Enable.invalidate();
    textArea_CH1_Enable.invalidate();
}


void Screen_MainView::switchOutput_CH2(void)
{
    // HAB TODO: May want to add some pre-qualifers to turning on output
    // Toggle the output relay ON / OFF
    if (ArbPwrBoosterStatus.CH2.OutputSwitch == OFF)
    {
        CH2_OUTPUT_ENABLE();
        box_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(0x5E, 0xEB, 0x13));
        box_CH2_Enable.setAlpha(255);
        textArea_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(0xFF, 0xFF, 0xFF));
        ArbPwrBoosterStatus.CH2.OutputSwitch = ON;
    }
    else
    {
        CH2_OUTPUT_DISABLE();
        box_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(0xA7, 0xC4, 0xB2));
        box_CH2_Enable.setAlpha(50);
        textArea_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(0x70, 0x70, 0x70));
        ArbPwrBoosterStatus.CH2.OutputSwitch = OFF;
    }
    box_CH2_Enable.invalidate();
    textArea_CH2_Enable.invalidate();
}
