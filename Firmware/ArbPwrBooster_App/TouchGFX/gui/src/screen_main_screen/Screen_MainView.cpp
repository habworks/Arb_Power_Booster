#include <gui/screen_main_screen/Screen_MainView.hpp>

// ADDED INCLUDES
#include "touchgfx/Color.hpp"
#include "../../../../../../touchgfx/generated/texts/include/texts/TextKeysAndLanguages.hpp"
#include "Main_Support.h"
#include "IO_Support.h"
#include "ADC_Support.h"


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
    ArbPwrBooster.Screen = MAIN_SCREEN;

    // STEP 2: Update Channel 1
    inputImpedanceSet_CH1();
    outputSwitch_CH1();
    // Update Current Set Values for channel1
    Unicode::snprintfFloat(textArea_CH1_CurrentSetBuffer, TEXTAREA_CH1_CURRENTSET_SIZE, "%05.3f", ArbPwrBooster.CH1.Limit.Current);
    textArea_CH1_CurrentSet.setWildcard(textArea_CH1_CurrentSetBuffer);
    textArea_CH1_CurrentSet.invalidate();

    // STEP 3: Update Channel 2
    inputImpedanceSet_CH2();
    outputSwitch_CH2();
    // Update Current Set Values for channel2
    Unicode::snprintfFloat(textArea_CH2_CurrentSetBuffer, TEXTAREA_CH2_CURRENTSET_SIZE, "%06.3f", ArbPwrBooster.CH2.Limit.Current);
    textArea_CH2_CurrentSet.setWildcard(textArea_CH2_CurrentSetBuffer);
    textArea_CH2_CurrentSet.invalidate();

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
void Screen_MainView::outputToggle_CH1(void)
{
    // STEP 1: Toggle the output relay ON / OFF
    if (ArbPwrBooster.CH1.OutputSwitch == OFF)
    {
        CH1_OUTPUT_ENABLE();
        box_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_ON_BOX_COLOR));
        box_CH1_Enable.setAlpha(ALPAH_FULL_VISIBLE);
        textArea_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_ON_TXT_COLOR));
        ArbPwrBooster.CH1.OutputSwitch = ON;
    }
    else
    {
        CH1_OUTPUT_DISABLE();
        box_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_BOX_COLOR));
        box_CH1_Enable.setAlpha(ALPHA_50_VISIBLE);
        textArea_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_TXT_COLOR));
        ArbPwrBooster.CH1.OutputSwitch = OFF;
    }
    box_CH1_Enable.invalidate();
    textArea_CH1_Enable.invalidate();

} // END OF outputToggle_CH1



/********************************************************************************************************
* @brief Switch the output of channel 1.  Update the background to a bright green if ON, a grey if off.  In
* addition adjust colors of the ON value to make obvious to user.  The output is set according to the ArbPwrBooster
* status.
*
* @author original: Hab Collector \n
*
* @note: Make updates for Input Z, Output Enable and Current Limit for both channels
*
* STEP 1: Switch the output relay ON / OFF according to ArbPwrBooster status
********************************************************************************************************/
void Screen_MainView::outputSwitch_CH1(void)
{
    // STEP 1: Switch the output relay ON / OFF according to ArbPwrBooster status
    if (ArbPwrBooster.CH1.OutputSwitch == OFF)
    {
        CH1_OUTPUT_DISABLE();
        box_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_BOX_COLOR));
        box_CH1_Enable.setAlpha(ALPHA_50_VISIBLE);
        textArea_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_TXT_COLOR));
        ArbPwrBooster.CH1.OutputSwitch = OFF;
    }
    else
    {
        CH1_OUTPUT_ENABLE();
        box_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_ON_BOX_COLOR));
        box_CH1_Enable.setAlpha(ALPAH_FULL_VISIBLE);
        textArea_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_ON_TXT_COLOR));
        ArbPwrBooster.CH1.OutputSwitch = ON;
    }
    box_CH1_Enable.invalidate();
    textArea_CH1_Enable.invalidate();

} // END OF outputSwitch_CH1



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
void Screen_MainView::outputToggle_CH2(void)
{
    // STEP 1: Toggle the output relay ON / OFF
    if (ArbPwrBooster.CH2.OutputSwitch == OFF)
    {
        CH2_OUTPUT_ENABLE();
        box_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_ON_BOX_COLOR));
        box_CH2_Enable.setAlpha(ALPAH_FULL_VISIBLE);
        textArea_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_ON_TXT_COLOR));
        ArbPwrBooster.CH2.OutputSwitch = ON;
    }
    else
    {
        CH2_OUTPUT_DISABLE();
        box_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_BOX_COLOR));
        box_CH2_Enable.setAlpha(ALPHA_50_VISIBLE);
        textArea_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_TXT_COLOR));
        ArbPwrBooster.CH2.OutputSwitch = OFF;
    }
    box_CH2_Enable.invalidate();
    textArea_CH2_Enable.invalidate();

} // END OF outputToggle_CH2



/********************************************************************************************************
* @brief Switch the output of channel 2.  Update the background to a bright green if ON, a grey if off.  In
* addition adjust colors of the ON value to make obvious to user.  The output is set according to the ArbPwrBooster
* status.
*
* @author original: Hab Collector \n
*
* @note: The POR default is OFF.
*
* STEP 1: Switch the output relay ON / OFF according to ArbPwrBooster status
********************************************************************************************************/
void Screen_MainView::outputSwitch_CH2(void)
{
    // STEP 1: Switch the output relay ON / OFF according to ArbPwrBooster status
    if (ArbPwrBooster.CH2.OutputSwitch == OFF)
    {
        CH2_OUTPUT_DISABLE();
        box_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_BOX_COLOR));
        box_CH2_Enable.setAlpha(ALPHA_50_VISIBLE);
        textArea_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_TXT_COLOR));
        ArbPwrBooster.CH2.OutputSwitch = OFF;
    }
    else
    {
        CH2_OUTPUT_ENABLE();
        box_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_ON_BOX_COLOR));
        box_CH2_Enable.setAlpha(ALPAH_FULL_VISIBLE);
        textArea_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_ON_TXT_COLOR));
        ArbPwrBooster.CH2.OutputSwitch = ON;
    }
    box_CH2_Enable.invalidate();
    textArea_CH2_Enable.invalidate();

} // END OF outputSwitch_CH2



/********************************************************************************************************
* @brief Toggle the input Z of channel 1.  Update the background text to reflect the present value of that
* impedance.
*
* @author original: Hab Collector \n
*
* @note: Input impedance switches between 1M (POR default) and 50 ohm
* @note: Input impedance is a configurable parameter so the config file must be updated
*
* STEP 1: Toggle input impedance
* STEP 2: Update config file for future POR
********************************************************************************************************/
void Screen_MainView::inputImpedanceToggle_CH1(void)
{
    // STEP 1: Toggle input impedance
    if (ArbPwrBooster.CH1.InputImpedance == ONE_MEG_OHM)
    {
        CH1_INPUT_50_ENABLE();
        textArea_CH1_InputZ.setTypedText(TypedText(T_FIFTY_OHM));
        ArbPwrBooster.CH1.InputImpedance = FIFTY_OHM;
    }
    else
    {
        CH1_INPUT_50_DISABLE();
        textArea_CH1_InputZ.setTypedText(TypedText(T_ONE_MEG_OHM));
        ArbPwrBooster.CH1.InputImpedance = ONE_MEG_OHM;
    }
    textArea_CH1_InputZ.invalidate();

    // STEP 2: Update config file for future POR
    saveConfigParameters();

} // END OF inputImpedanceToggle_CH1



/********************************************************************************************************
* @brief Set the input Z of channel 1.  Update the background text to reflect the present value of that
* impedance.  The input impedance is set according to the ArbPwrBooster status.
*
* @author original: Hab Collector \n
*
* @note: Input impedance switches between 1M (POR default) and 50 ohm
*
* STEP 1: Set input impedance according to the ArbPwrBooster status
********************************************************************************************************/
void Screen_MainView::inputImpedanceSet_CH1(void)
{
    // STEP 1: Set input impedance according to the ArbPwrBooster status
    if (ArbPwrBooster.CH1.InputImpedance == ONE_MEG_OHM)
    {
        CH1_INPUT_50_DISABLE();
        textArea_CH1_InputZ.setTypedText(TypedText(T_ONE_MEG_OHM));
        ArbPwrBooster.CH1.InputImpedance = ONE_MEG_OHM;
    }
    else
    {
        CH1_INPUT_50_ENABLE();
        textArea_CH1_InputZ.setTypedText(TypedText(T_FIFTY_OHM));
        ArbPwrBooster.CH1.InputImpedance = FIFTY_OHM;
    }
    textArea_CH1_InputZ.invalidate();

} // END OF inputImpedanceSet_CH1



/********************************************************************************************************
* @brief Toggle the input Z of channel 2.  Update the background text to reflect the present value of that
* impedance.
*
* @author original: Hab Collector \n
*
* @note: Input impedance switches between 1M (POR default) and 50 ohm
* @note: Input impedance is a configurable parameter so the config file must be updated
*
* STEP 1: Toggle input impedance
* STEP 2: Update config file for future POR
********************************************************************************************************/
void Screen_MainView::inputImpedanceToggle_CH2(void)
{
    // STEP 1: Toggle input impedance
    if (ArbPwrBooster.CH2.InputImpedance == ONE_MEG_OHM)
    {
        CH2_INPUT_50_ENABLE();
        textArea_CH2_InputZ.setTypedText(TypedText(T_FIFTY_OHM));
        ArbPwrBooster.CH2.InputImpedance = FIFTY_OHM;
    }
    else
    {
        CH2_INPUT_50_DISABLE();
        textArea_CH2_InputZ.setTypedText(TypedText(T_ONE_MEG_OHM));
        ArbPwrBooster.CH2.InputImpedance = ONE_MEG_OHM;
    }
    textArea_CH2_InputZ.invalidate();

    // STEP 2: Update config file for future POR
    saveConfigParameters();

} // END OF inputImpedanceToggle_CH2



/********************************************************************************************************
* @brief Set the input Z of channel 2.  Update the background text to reflect the present value of that
* impedance.  The input impedance is set according to the ArbPwrBooster status.
*
* @author original: Hab Collector \n
*
* @note: Input impedance switches between 1M (POR default) and 50 ohm
*
* STEP 1: Set input impedance according to ArbPwrBooster status
********************************************************************************************************/
void Screen_MainView::inputImpedanceSet_CH2(void)
{
    // STEP 1: Set input impedance according to ArbPwrBooster status
    if (ArbPwrBooster.CH2.InputImpedance == ONE_MEG_OHM)
    {
        CH2_INPUT_50_DISABLE();
        textArea_CH2_InputZ.setTypedText(TypedText(T_ONE_MEG_OHM));
        ArbPwrBooster.CH2.InputImpedance = ONE_MEG_OHM;
    }
    else
    {
        CH2_INPUT_50_ENABLE();
        textArea_CH2_InputZ.setTypedText(TypedText(T_FIFTY_OHM));
        ArbPwrBooster.CH2.InputImpedance = FIFTY_OHM;
    }
    textArea_CH2_InputZ.invalidate();

} // END OF inputImpedanceSet_CH2



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
    ArbPwrBooster.ActiveChannel = CHANNEL_1;

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
    ArbPwrBooster.ActiveChannel = CHANNEL_2;

} // END OF setActiveLimit_CH2



/********************************************************************************************************
* @brief Live update of the main screen.  All parameters for CH1 and CH are updated.  The Config Icon background
* a flexbutton is also updated if there is a config related error condition
*
* @author original: Hab Collector \n
*
* STEP 1: Update Channel 1
* STEP 2: Update Channel 2
* STEP 3: Config Icon Color
********************************************************************************************************/
void Screen_MainView::updateMainScreen_View(void)
{
    // STEP 1: Update Channel 1
    // RMS current
    Unicode::snprintfFloat(textArea_CH1_ArmsBuffer, TEXTAREA_CH1_ARMS_SIZE, "%05.3f", ArbPwrBooster.CH1.Measure.RMS_Current);
    textArea_CH1_Arms.setWildcard(textArea_CH1_ArmsBuffer);
    // Max Current
    Unicode::snprintfFloat(textArea_CH1_AmaxBuffer, TEXTAREA_CH1_AMAX_SIZE, "%05.3f", ArbPwrBooster.CH1.Measure.MaxCurrent);
    textArea_CH1_Amax.setWildcard(textArea_CH1_AmaxBuffer);
    // Min Current
    Unicode::snprintfFloat(textArea_CH1_AminBuffer, TEXTAREA_CH1_AMIN_SIZE, "%05.3f", ArbPwrBooster.CH1.Measure.MinCurrent);
    textArea_CH1_Amin.setWildcard(textArea_CH1_AminBuffer);

    // STEP 2: Update Channel 2
    // RMS current
    Unicode::snprintfFloat(textArea_CH2_ArmsBuffer, TEXTAREA_CH2_ARMS_SIZE, "%05.3f", ArbPwrBooster.CH2.Measure.RMS_Current);
    textArea_CH2_Arms.setWildcard(textArea_CH2_ArmsBuffer);
    // Max Current
    Unicode::snprintfFloat(textArea_CH2_AmaxBuffer, TEXTAREA_CH2_AMAX_SIZE, "%05.3f", ArbPwrBooster.CH2.Measure.MaxCurrent);
    textArea_CH2_Amax.setWildcard(textArea_CH2_AmaxBuffer);
    // Min Current
    Unicode::snprintfFloat(textArea_CH2_AminBuffer, TEXTAREA_CH2_AMIN_SIZE, "%05.3f", ArbPwrBooster.CH2.Measure.MinCurrent);
    textArea_CH2_Amin.setWildcard(textArea_CH2_AminBuffer);

    // STEP 3: Config Icon Color
    char NotUsedStatusText[50];
    uint8_t NotUsedErrorNumber;
    if (systemMeasureWithinLimits(NotUsedStatusText, &NotUsedErrorNumber))
    {
        flexButton_Config.setAlpha(ALPHA_ZERO_VISIBLE);
    }
    else
    {
        flexButton_Config.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(CONFIG_ICON_ERROR_COLOR), touchgfx::Color::getColorFromRGB(0, 153, 204), touchgfx::Color::getColorFromRGB(CONFIG_ICON_ERROR_COLOR), touchgfx::Color::getColorFromRGB(51, 102, 153));
        flexButton_Config.setAlpha(ALPAH_FULL_VISIBLE);
    }

    this->invalidate();

} // END OF updateMainScreen_View

// HAB TODO: Not sure if and how I plan to use this TBD
void Screen_MainView::POR_setControlDefaults(void)
{
    // STEP 1: Set controls for POR defaults
    // Impedance
//    CH1_INPUT_50_DISABLE();
//    textArea_CH1_InputZ.setTypedText(TypedText(T_ONE_MEG_OHM));
//    ArbPwrBooster.CH1.InputImpedance = ONE_MEG_OHM;
    // Output Switch
    CH1_OUTPUT_DISABLE();
    box_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_BOX_COLOR));
    box_CH1_Enable.setAlpha(ALPHA_50_VISIBLE);
    textArea_CH1_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_TXT_COLOR));
    ArbPwrBooster.CH1.OutputSwitch = OFF;

    // STEP 1: Set controls for POR defaults
    // Impedance
//    CH2_INPUT_50_DISABLE();
//    textArea_CH2_InputZ.setTypedText(TypedText(T_ONE_MEG_OHM));
//    ArbPwrBooster.CH2.InputImpedance = ONE_MEG_OHM;
    // Output Switch
    CH1_OUTPUT_DISABLE();
    box_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_BOX_COLOR));
    box_CH2_Enable.setAlpha(ALPHA_50_VISIBLE);
    textArea_CH2_Enable.setColor(touchgfx::Color::getColorFromRGB(OUTPUT_OFF_TXT_COLOR));
    ArbPwrBooster.CH2.OutputSwitch = OFF;

    this->invalidate();
}
