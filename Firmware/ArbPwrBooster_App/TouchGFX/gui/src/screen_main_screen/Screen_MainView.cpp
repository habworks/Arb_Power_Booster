#include <gui/screen_main_screen/Screen_MainView.hpp>

// ADDED INCLUDES
#include "touchgfx/Color.hpp"
#include "../../../../../../touchgfx/generated/texts/include/texts/TextKeysAndLanguages.hpp"
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
/********************************************************************************************************
* @brief Update the screen on entry.  It is called by the end of the Main Screen transition interaction.
* Update the display for all fixed items
*
* @author original: Hab Collector \n
*
* @note: Make updates for Input Z, Output Enable and Current Limit for both channels
* @note: Arms, Amax, Amin  are updated by the listener on a periodic interval
*
* STEP 1: Set the active screen
* STEP 2: Update Channel 1
* STEP 3: Update Channel 2
********************************************************************************************************/
void Screen_MainView::update_Screen_Main(void)
{
    // STEP 1: Set the active screen
    ArbPwrBoosterStatus.Screen = MAIN_SCREEN;

    // STEP 2: Update Channel 1
    // Update Current Set Values for channel1
    Unicode::snprintfFloat(textArea_CH1_CurrentSetBuffer, TEXTAREA_CH1_CURRENTSET_SIZE, "%05.3f", ArbPwrBoosterStatus.CH1.Limit.Current);
    textArea_CH1_CurrentSet.setWildcard(textArea_CH1_CurrentSetBuffer);
    textArea_CH1_CurrentSet.invalidate();

    // STEP 3: Update Channel 2
    // Update Current Set Values for channel2
    Unicode::snprintfFloat(textArea_CH2_CurrentSetBuffer, TEXTAREA_CH2_CURRENTSET_SIZE, "%06.3f", ArbPwrBoosterStatus.CH2.Limit.Current);
    textArea_CH2_CurrentSet.setWildcard(textArea_CH2_CurrentSetBuffer);
    textArea_CH2_CurrentSet.invalidate();

    // HAB TODO: Add for others this function is incomplete

} // END OF update_Screen_Main



/********************************************************************************************************
* @brief Toggle the output of channel 1.  Update the background to a bright green if ON, a grey if off.  In
* addition adjust colors of the ON value to make obvious to user
*
* @author original: Hab Collector \n
*
* @note: Make updates for Input Z, Output Enable and Current Limit for both channels
*
* STEP 1: Toggle the output relay ON / OFF
********************************************************************************************************/
void Screen_MainView::switchOutput_CH1(void)
{
    // HAB TODO: May want to add some pre-qualifers to turning on output
    // HAB TODO: Add defines for these magic numbers
    // STEP 1: Toggle the output relay ON / OFF
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

} // END OF switchOutput_CH1



/********************************************************************************************************
* @brief Toggle the output of channel 2.  Update the background to a bright green if ON, a grey if off.  In
* addition adjust colors of the ON value to make obvious to user.
*
* @author original: Hab Collector \n
*
* @note: The POR default is OFF.
*
* STEP 1: Toggle the output relay ON / OFF
********************************************************************************************************/
void Screen_MainView::switchOutput_CH2(void)
{
    // HAB TODO: May want to add some pre-qualifers to turning on output
    // STEP 1: Toggle the output relay ON / OFF
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

} // END OF switchOutput_CH2



/********************************************************************************************************
* @brief Toggle the input Z of channel 1.  Update the background text to reflect the present value of that
* impedance.
*
* @author original: Hab Collector \n
*
* @note: Input impedance switches between 1M (POR default) and 50 ohm
*
* STEP 1: Toggle input impedance
********************************************************************************************************/
void Screen_MainView::InputImpedanceSet_CH1(void)
{
    // STEP 1: Toggle input impedance
    if (ArbPwrBoosterStatus.CH1.InputImpedance == ONE_MEG_OHM)
    {
        CH1_INPUT_50_ENABLE();
        textArea_CH1_InputZ.setTypedText(TypedText(T_FIFTY_OHM));
        ArbPwrBoosterStatus.CH1.InputImpedance = FIFTY_OHM;
    }
    else
    {
        CH1_INPUT_50_DISABLE();
        textArea_CH1_InputZ.setTypedText(TypedText(T_ONE_MEG_OHM));
        ArbPwrBoosterStatus.CH1.InputImpedance = ONE_MEG_OHM;
    }
    textArea_CH1_InputZ.invalidate();

} // END OF InputImpedanceSet_CH1



/********************************************************************************************************
* @brief Toggle the input Z of channel 2.  Update the background text to reflect the present value of that
* impedance.
*
* @author original: Hab Collector \n
*
* @note: Input impedance switches between 1M (POR default) and 50 ohm
*
* STEP 1: Toggle input impedance
********************************************************************************************************/
void Screen_MainView::InputImpedanceSet_CH2(void)
{
    // STEP 1: Toggle input impedance
    if (ArbPwrBoosterStatus.CH2.InputImpedance == ONE_MEG_OHM)
    {
        CH2_INPUT_50_ENABLE();
        textArea_CH2_InputZ.setTypedText(TypedText(T_FIFTY_OHM));
        ArbPwrBoosterStatus.CH2.InputImpedance = FIFTY_OHM;
    }
    else
    {
        CH2_INPUT_50_DISABLE();
        textArea_CH2_InputZ.setTypedText(TypedText(T_ONE_MEG_OHM));
        ArbPwrBoosterStatus.CH2.InputImpedance = ONE_MEG_OHM;
    }
    textArea_CH2_InputZ.invalidate();

} // END OF InputImpedanceSet_CH2



/********************************************************************************************************
* @brief Sets the active channel as 1 for the purpose of setting limits
*
* @author original: Hab Collector \n
*
* STEP 1: Set the active channel
********************************************************************************************************/
void Screen_MainView::setActiveLimit_CH1(void)
{
    // STEP 1: Set the active channel
    ArbPwrBoosterStatus.ActiveChannel = CHANNEL_1;

} // END OF setActiveLimit_CH1



/********************************************************************************************************
* @brief Sets the active channel as 2 for the purpose of setting limits
*
* @author original: Hab Collector \n
*
* STEP 1: Set the active channel
********************************************************************************************************/
void Screen_MainView::setActiveLimit_CH2(void)
{
    ArbPwrBoosterStatus.ActiveChannel = CHANNEL_2;

} // END OF setActiveLimit_CH2
