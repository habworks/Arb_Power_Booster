#include <gui/screen_set_screen/Screen_SetView.hpp>

// ADDED INCLUDES
#include "Main_Support.h"
#include "touchgfx/Color.hpp"
#include "../../../../../../touchgfx/generated/texts/include/texts/TextKeysAndLanguages.hpp"

Screen_SetView::Screen_SetView()
{

}

void Screen_SetView::setupScreen()
{
    Screen_SetViewBase::setupScreen();
}

void Screen_SetView::tearDownScreen()
{
    Screen_SetViewBase::tearDownScreen();
}

// ADDED FUNCTIONS
/********************************************************************************************************
* @brief Update the screen on entry.  It is called by the end of the Set Screen transition interaction.
* Update the display for all fixed items
*
* @author original: Hab Collector \n
*
* @note: Make updates for Input Z, Output Enable and Current Limit for both channels
* @note: Arms, Amax, Amin  are updated by the listener on a periodic interval
*
* STEP 1: Set the active screen
* STEP 2: Update display based on active channel
********************************************************************************************************/
void Screen_SetView::update_Screen_Set(void)
{
    // STEP 1: Set the active screen
    ArbPwrBoosterStatus.Screen = SET_SCREEN;

    // STEP 2: Update display based on active channel
    uint8_t Red, Green, Blue;
    uint8_t DigitRed, DigitGreen, DigitBlue;
    uint8_t EnumTextValue;
    uint8_t Integer, Tenths, Hundredths, Thousandths;
    double CurrentLimit;

    // Channel 1 Options
    if (ArbPwrBoosterStatus.ActiveChannel == CHANNEL_1)
    {
        EnumTextValue = T_CH1_HEADING;
        Red = CH1_BASE_COLOR_RED;
        Green = CH1_BASE_COLOR_GRN;
        Blue = CH1_BASE_COLOR_BLU;
        if (ArbPwrBoosterStatus.CH1.Limit.Enable)
        {
            DigitRed = CH1_BASE_COLOR_RED;
            DigitGreen = CH1_BASE_COLOR_GRN;
            DigitBlue = CH1_BASE_COLOR_BLU;
        }
        else
        {
            DigitRed = DISABLE_BASE_COLOR_RED;
            DigitGreen = DISABLE_BASE_COLOR_GRN;
            DigitBlue = DISABLE_BASE_COLOR_BLU;
        }
        CurrentLimit = ArbPwrBoosterStatus.CH1.Limit.Current;
    }
    // Channel 2 Options
    if (ArbPwrBoosterStatus.ActiveChannel == CHANNEL_2)
    {
        EnumTextValue = T_CH2_HEADING;
        Red = CH2_BASE_COLOR_RED;
        Green = CH2_BASE_COLOR_GRN;
        Blue = CH2_BASE_COLOR_BLU;
        if (ArbPwrBoosterStatus.CH2.Limit.Enable)
        {
            DigitRed = CH2_BASE_COLOR_RED;
            DigitGreen = CH2_BASE_COLOR_GRN;
            DigitBlue = CH2_BASE_COLOR_BLU;
        }
        else
        {
            DigitRed = DISABLE_BASE_COLOR_RED;
            DigitGreen = DISABLE_BASE_COLOR_GRN;
            DigitBlue = DISABLE_BASE_COLOR_BLU;
        }
        CurrentLimit = ArbPwrBoosterStatus.CH2.Limit.Current;
    }

    // STEP 3: Update the display objects
    // Channel Title
    textArea_CH_ToSet.setTypedText(TypedText(EnumTextValue));
    textArea_CH_ToSet.setColor(touchgfx::Color::getColorFromRGB(Red, Green, Blue));
    // Digits Integer
    digitsFromDouble(CurrentLimit, &Integer, &Tenths, &Hundredths, &Thousandths);
    Unicode::snprintf(textArea_AmpsDigit_0Buffer, TEXTAREA_AMPSDIGIT_0_SIZE, "%d", Integer);
    textArea_AmpsDigit_0.setWildcard(textArea_AmpsDigit_0Buffer);
    // Digits Tenths
    textArea_AmpsDigit_0.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    textArea_AmpsDigit_1.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    textArea_AmpsDigit_2.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    textArea_AmpsDigit_3.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    textArea_AmpsDigit_3.invalidate();
    textArea_DecimalPoint.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    // Unit
    textArea_Units.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    // Control text
    textArea_EnableLimit.setColor(touchgfx::Color::getColorFromRGB(Red, Green, Blue));
    textArea_ResetMinMax.setColor(touchgfx::Color::getColorFromRGB(Red, Green, Blue));
    // Refresh everything
    this->invalidate();

} // END OF update_Screen_Set
