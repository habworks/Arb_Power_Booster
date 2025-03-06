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
* Update the display for all fixed items and set the init conditions for the screen
*
* @author original: Hab Collector \n
*
* @note: Make updates for current limit, enable, and the correct channel color
*
* STEP 1: Set the active screen and init conditions
* STEP 2: Render screen based on active channel
********************************************************************************************************/
void Screen_SetView::update_Screen_Set(void)
{
    // STEP 1: Set the active screen and init conditions
    ArbPwrBoosterStatus.Screen = SET_SCREEN;
    ArbPwrBoosterStatus.SetLimitDigit = INTEGER;

    // STEP 2: Render screen based on active channel
    render_Screen_Set(ArbPwrBoosterStatus.ActiveChannel);

} // END OF update_Screen_Set



/********************************************************************************************************
* @brief Render the Set Screen in accordance with the active channel
*
* @author original: Hab Collector \n
*
* @note: Make updates for current limit, enable, and the correct channel color
*
* STEP 1: Set view parameters based on active channel
* STEP 2: Update the display text objects
********************************************************************************************************/
void Screen_SetView::render_Screen_Set(Type_Channel Channel)
{
    bool LimitEnable = false;
    uint8_t Red = 0, Green = 0, Blue = 0;
    uint8_t DigitRed = 0, DigitGreen = 0, DigitBlue = 0;
    uint8_t EnumTextValue = 0;
    uint8_t Integer, Tenths, Hundredths, Thousandths;
    double CurrentLimit = 0;

    // STEP 1: Set view parameters based on active channel
    // Channel 1 Options
    if (Channel == CHANNEL_1)
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
            LimitEnable = true;
        }
        else
        {
            DigitRed = DISABLE_BASE_COLOR_RED;
            DigitGreen = DISABLE_BASE_COLOR_GRN;
            DigitBlue = DISABLE_BASE_COLOR_BLU;
            LimitEnable = false;
        }
        CurrentLimit = ArbPwrBoosterStatus.CH1.Limit.Current;
    }
    // Channel 2 Options
    if (Channel == CHANNEL_2)
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
            LimitEnable = true;
        }
        else
        {
            DigitRed = DISABLE_BASE_COLOR_RED;
            DigitGreen = DISABLE_BASE_COLOR_GRN;
            DigitBlue = DISABLE_BASE_COLOR_BLU;
            LimitEnable = false;
        }
        CurrentLimit = ArbPwrBoosterStatus.CH2.Limit.Current;
    }

    // STEP 2: Update the display text objects
    // Channel Title
    textArea_CH_ToSet.setTypedText(TypedText(EnumTextValue));
    textArea_CH_ToSet.setColor(touchgfx::Color::getColorFromRGB(Red, Green, Blue));
    // Digit Integer
    digitsFromDouble(CurrentLimit, &Integer, &Tenths, &Hundredths, &Thousandths);
    Unicode::snprintf(textArea_AmpsDigit_0Buffer, TEXTAREA_AMPSDIGIT_0_SIZE, "%d", Integer);
    textArea_AmpsDigit_0.setWildcard(textArea_AmpsDigit_0Buffer);
    textArea_AmpsDigit_0.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    // Digit Tenths
    Unicode::snprintf(textArea_AmpsDigit_1Buffer, TEXTAREA_AMPSDIGIT_1_SIZE, "%d", Tenths);
    textArea_AmpsDigit_1.setWildcard(textArea_AmpsDigit_1Buffer);
    textArea_AmpsDigit_1.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    // Digit Hundredths
    Unicode::snprintf(textArea_AmpsDigit_2Buffer, TEXTAREA_AMPSDIGIT_2_SIZE, "%d", Hundredths);
    textArea_AmpsDigit_2.setWildcard(textArea_AmpsDigit_2Buffer);
    textArea_AmpsDigit_2.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    // Digit Thousandths
    Unicode::snprintf(textArea_AmpsDigit_3Buffer, TEXTAREA_AMPSDIGIT_3_SIZE, "%d", Thousandths);
    textArea_AmpsDigit_3.setWildcard(textArea_AmpsDigit_3Buffer);
    textArea_AmpsDigit_3.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    // Decimal Point
    textArea_DecimalPoint.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    // Unit Symbol
    textArea_Units.setColor(touchgfx::Color::getColorFromRGB(DigitRed, DigitGreen, DigitBlue));
    // Control text
    textArea_EnableLimit.setColor(touchgfx::Color::getColorFromRGB(Red, Green, Blue));
    textArea_ResetMinMax.setColor(touchgfx::Color::getColorFromRGB(Red, Green, Blue));
    // Select Button
    radioButton_EnableLimit.setSelected(LimitEnable);

    // STEP 3: Update the digit display
    if (LimitEnable)
    {
        // Disable and make invisible all then enable and make visible the desired
        flexButton_AmpsDigit_0.setAlpha(ALPHA_ZERO_VISIBLE);
        flexButton_AmpsDigit_1.setAlpha(ALPHA_ZERO_VISIBLE);
        flexButton_AmpsDigit_2.setAlpha(ALPHA_ZERO_VISIBLE);
        flexButton_AmpsDigit_3.setAlpha(ALPHA_ZERO_VISIBLE);
        switch(ArbPwrBoosterStatus.SetLimitDigit)
        {
        case INTEGER:
        {
            flexButton_AmpsDigit_0.setAlpha(ALPAH_FULL_VISIBLE);
        }
        break;
        case TENTHS:
        {
            flexButton_AmpsDigit_1.setAlpha(ALPAH_FULL_VISIBLE);
        }
        break;
        case HUNDREDTHS:
        {
            flexButton_AmpsDigit_2.setAlpha(ALPAH_FULL_VISIBLE);
        }
        break;
        case THOUSANDTHS:
        {
            flexButton_AmpsDigit_3.setAlpha(ALPAH_FULL_VISIBLE);
        }
        break;
        default:
            break;
        } // END OF SWITCH
    }

    // Refresh everything
    this->invalidate();

} // END OF render_Screen_Set


void Screen_SetView::enableCurrentLimit(void)
{
    if (ArbPwrBoosterStatus.ActiveChannel == CHANNEL_1)
        ArbPwrBoosterStatus.CH1.Limit.Enable = true;
    else
        ArbPwrBoosterStatus.CH2.Limit.Enable = true;

    render_Screen_Set(ArbPwrBoosterStatus.ActiveChannel);
}


void Screen_SetView::disableCurrentLimit(void)
{
    if (ArbPwrBoosterStatus.ActiveChannel == CHANNEL_1)
        ArbPwrBoosterStatus.CH1.Limit.Enable = false;
    else
        ArbPwrBoosterStatus.CH2.Limit.Enable = false;

    render_Screen_Set(ArbPwrBoosterStatus.ActiveChannel);
}


void Screen_SetView::setIntegerToUpdate(void)
{
    ArbPwrBoosterStatus.SetLimitDigit = INTEGER;
    render_Screen_Set(ArbPwrBoosterStatus.ActiveChannel);
}


void Screen_SetView::setTenthsToUpdate(void)
{
    ArbPwrBoosterStatus.SetLimitDigit = TENTHS;
    render_Screen_Set(ArbPwrBoosterStatus.ActiveChannel);
}


void Screen_SetView::setHundredthsToUpdate(void)
{
    ArbPwrBoosterStatus.SetLimitDigit = HUNDREDTHS;
    render_Screen_Set(ArbPwrBoosterStatus.ActiveChannel);
}


void Screen_SetView::setThousandthsToUpdate(void)
{
    ArbPwrBoosterStatus.SetLimitDigit = THOUSANDTHS;
    render_Screen_Set(ArbPwrBoosterStatus.ActiveChannel);
}


void Screen_SetView::incrementDigitUp(void)
{

}

void Screen_SetView::incrementDigitDown(void)
{

}
