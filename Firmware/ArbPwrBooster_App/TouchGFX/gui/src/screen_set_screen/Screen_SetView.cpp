#include <gui/screen_set_screen/Screen_SetView.hpp>

// ADDED INCLUDES
#include "Main_Support.h"
#include "touchgfx/Color.hpp"
#include "../../../../../../touchgfx/generated/texts/include/texts/TextKeysAndLanguages.hpp"

double StartValueCurrentLimit;
bool StartValueLimitEnable;

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
* @note: Must capture the current limit and its enable status on screen entry to know if it has changed on screen exit
*
* STEP 1: Set the active screen and init conditions
* STEP 2: Render screen based on active channel
* STEP 3: Capture the start conditions for current limit
********************************************************************************************************/
void Screen_SetView::update_Screen_Set(void)
{
    // STEP 1: Set the active screen and init conditions
    ArbPwrBooster.Screen = SET_SCREEN;
    ArbPwrBooster.SetLimitDigit = INTEGER;

    // STEP 2: Render screen based on active channel
    render_Screen_Set(ArbPwrBooster.ActiveChannel);

    // STEP 3: Capture the start conditions for current limit
    if (ArbPwrBooster.ActiveChannel == CHANNEL_1)
    {
        StartValueCurrentLimit = ArbPwrBooster.CH1.Limit.Current;
        StartValueLimitEnable = ArbPwrBooster.CH1.Limit.Enable;
    }
    else
    {
        StartValueCurrentLimit = ArbPwrBooster.CH2.Limit.Current;
        StartValueLimitEnable = ArbPwrBooster.CH2.Limit.Enable;
    }

} // END OF update_Screen_Set



/********************************************************************************************************
* @brief Render the Set Screen in accordance with the active channel.  Screen shows limit value in base
* channel color if limit is enable.  It also shows active edit digit color: Integer, Tenths, Hundredths, or
* Thousandths.  If limit not enable, disable color is used and no active digit is transparent.  The enable
* box is also set or cleared accordingly.
*
* @author original: Hab Collector \n
*
* @note: Make updates for current limit, enable, and the correct channel color
* @note: Can support integer values from 99 to -99
*
* @param Channel: The active channel
*
* STEP 1: Set view parameters based on active channel
* STEP 2: Update the display text objects
* STEP 3: Update the digit display selection
********************************************************************************************************/
void Screen_SetView::render_Screen_Set(Type_Channel Channel)
{
    bool LimitEnable = false;
    uint8_t Red = 0, Green = 0, Blue = 0;
    uint8_t DigitRed = 0, DigitGreen = 0, DigitBlue = 0;
    uint8_t EnumTextValue = 0;
    int8_t Integer = 0;
    uint8_t Tenths, Hundredths, Thousandths;
    double CurrentLimit = 0;

    // STEP 1: Set view parameters based on active channel
    // Channel 1 Options
    if (Channel == CHANNEL_1)
    {
        EnumTextValue = T_CH1_HEADING;
        Red = CH1_BASE_COLOR_RED;
        Green = CH1_BASE_COLOR_GRN;
        Blue = CH1_BASE_COLOR_BLU;
        if (ArbPwrBooster.CH1.Limit.Enable)
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
        CurrentLimit = ArbPwrBooster.CH1.Limit.Current;
    }
    // Channel 2 Options
    if (Channel == CHANNEL_2)
    {
        EnumTextValue = T_CH2_HEADING;
        Red = CH2_BASE_COLOR_RED;
        Green = CH2_BASE_COLOR_GRN;
        Blue = CH2_BASE_COLOR_BLU;
        if (ArbPwrBooster.CH2.Limit.Enable)
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
        CurrentLimit = ArbPwrBooster.CH2.Limit.Current;
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

    // STEP 3: Update the digit display selection
    if (LimitEnable)
    {
        // Disable and make invisible all then enable and make visible the desired
        flexButton_AmpsDigit_0.setAlpha(ALPHA_ZERO_VISIBLE);
        flexButton_AmpsDigit_1.setAlpha(ALPHA_ZERO_VISIBLE);
        flexButton_AmpsDigit_2.setAlpha(ALPHA_ZERO_VISIBLE);
        flexButton_AmpsDigit_3.setAlpha(ALPHA_ZERO_VISIBLE);
        switch(ArbPwrBooster.SetLimitDigit)
        {
        case INTEGER:
        {
            flexButton_AmpsDigit_0.setAlpha(ALPAH_FULL_VISIBLE);
            // Adjust button size for 1 place integer: Will be positive value 0-9
            if ((Integer < 10) && (Integer >= 0))
            {
                flexButton_AmpsDigit_0.setPosition(INT_0_POSITION_1_DIGIT);
                flexButton_AmpsDigit_0.setBoxWithBorderPosition(INT_0_SETBOX_1_DIGIT);
            }
            // Adjust button size for 2 place integer: Will be positive value > 9 and negative value > -10
            else if ( (Integer >= 10) || ((Integer < 0) && (Integer > -10)))
            {
                flexButton_AmpsDigit_0.setPosition(INT_0_POSITION_2_DIGIT);
                flexButton_AmpsDigit_0.setBoxWithBorderPosition(INT_0_SETBOX_2_DIGIT);
            }
            // Adjust button size for 3 place: Will be value <= -10
            else if (Integer <= -10)
            {
                flexButton_AmpsDigit_0.setPosition(INT_0_POSITION_3_DIGIT);
                flexButton_AmpsDigit_0.setBoxWithBorderPosition(INT_0_SETBOX_3_DIGIT);
            }
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



/********************************************************************************************************
* @brief Enable current member for the active channel and updates the screen accordingly.
*
* @author original: Hab Collector \n
*
* @note: Make updates for current limit, enable, and the correct channel color
*
* STEP 1: Set the active channel enable
* STEP 2: Call screen render the screen to reflect the change
********************************************************************************************************/
void Screen_SetView::enableCurrentLimit(void)
{
    // STEP 1: Set the active channel enable
    if (ArbPwrBooster.ActiveChannel == CHANNEL_1)
        ArbPwrBooster.CH1.Limit.Enable = true;
    else
        ArbPwrBooster.CH2.Limit.Enable = true;

    // STEP 2: Call screen render the screen to reflect the change
    render_Screen_Set(ArbPwrBooster.ActiveChannel);

} // END OF enableCurrentLimit



/********************************************************************************************************
* @brief Disable current member for the active channel and updates the screen accordingly.
*
* @author original: Hab Collector \n
*
* @note: Make updates for current limit, enable, and the correct channel color
*
* STEP 1: Clear the active channel enable
* STEP 2: Call screen render the screen to reflect the change
********************************************************************************************************/
void Screen_SetView::disableCurrentLimit(void)
{
    // STEP 1: Clear the active channel enable
    if (ArbPwrBooster.ActiveChannel == CHANNEL_1)
        ArbPwrBooster.CH1.Limit.Enable = false;
    else
        ArbPwrBooster.CH2.Limit.Enable = false;

    // STEP 2: Call screen render the screen to reflect the change
    render_Screen_Set(ArbPwrBooster.ActiveChannel);

} // END OF disableCurrentLimit


/********************************************************************************************************
* @brief Make Integer the set limit digit to be modified for the active channel
*
* @author original: Hab Collector \n
*
* @note: Integer range is intended to be -10 to 10 for this revision of code
* @note: Display wise integer can be -99 to 99
*
* STEP 1: Set active limit digit to integer
* STEP 2: Call screen render to reflect the change
********************************************************************************************************/
void Screen_SetView::setIntegerToUpdate(void)
{
    // STEP 1: Set active limit digit to integer
    if ((ArbPwrBooster.ActiveChannel == CHANNEL_1) && (!ArbPwrBooster.CH1.Limit.Enable))
        return;
    if ((ArbPwrBooster.ActiveChannel == CHANNEL_2) && (!ArbPwrBooster.CH2.Limit.Enable))
        return;
    ArbPwrBooster.SetLimitDigit = INTEGER;

    // STEP 2: Call screen render to reflect the change
    render_Screen_Set(ArbPwrBooster.ActiveChannel);

} // END OF setIntegerToUpdate



/********************************************************************************************************
* @brief Make Tenths the set limit digit to be modified for the active channel
*
* @author original: Hab Collector \n
*
* STEP 1: Set active limit digit to tenths
* STEP 2: Call screen render to reflect the change
********************************************************************************************************/
void Screen_SetView::setTenthsToUpdate(void)
{
    // STEP 1: Set active limit digit to tenths
    if ((ArbPwrBooster.ActiveChannel == CHANNEL_1) && (!ArbPwrBooster.CH1.Limit.Enable))
        return;
    if ((ArbPwrBooster.ActiveChannel == CHANNEL_2) && (!ArbPwrBooster.CH2.Limit.Enable))
        return;
    ArbPwrBooster.SetLimitDigit = TENTHS;

    // STEP 2: Call screen render to reflect the change
    render_Screen_Set(ArbPwrBooster.ActiveChannel);

} // END OF setTenthsToUpdate



/********************************************************************************************************
* @brief Make Hundredths the set limit digit to be modified for the active channel
*
* @author original: Hab Collector \n
*
* STEP 1: Set active limit digit to hundredths
* STEP 2: Call screen render to reflect the change
********************************************************************************************************/
void Screen_SetView::setHundredthsToUpdate(void)
{
    // STEP 1: Set active limit digit to hundredths
    if ((ArbPwrBooster.ActiveChannel == CHANNEL_1) && (!ArbPwrBooster.CH1.Limit.Enable))
        return;
    if ((ArbPwrBooster.ActiveChannel == CHANNEL_2) && (!ArbPwrBooster.CH2.Limit.Enable))
        return;
    ArbPwrBooster.SetLimitDigit = HUNDREDTHS;

    // STEP 2: Call screen render to reflect the change
    render_Screen_Set(ArbPwrBooster.ActiveChannel);

} // END OF setHundredthsToUpdate



/********************************************************************************************************
* @brief Make Thousandths the set limit digit to be modified for the active channel
*
* @author original: Hab Collector \n
*
* STEP 1: Set active limit digit to thousandths
* STEP 2: Call screen render to reflect the change
********************************************************************************************************/
void Screen_SetView::setThousandthsToUpdate(void)
{
    // STEP 1: Set active limit digit to thousandths
    if ((ArbPwrBooster.ActiveChannel == CHANNEL_1) && (!ArbPwrBooster.CH1.Limit.Enable))
        return;
    if ((ArbPwrBooster.ActiveChannel == CHANNEL_2) && (!ArbPwrBooster.CH2.Limit.Enable))
        return;
    ArbPwrBooster.SetLimitDigit = THOUSANDTHS;

    // STEP 2: Call screen render to reflect the change
    render_Screen_Set(ArbPwrBooster.ActiveChannel);

} // END OF setThousandthsToUpdate



/********************************************************************************************************
* @brief Increment the active limit digit by one.  For integers the max it can be incremented to is +10 with
* no role over.  For all other digits the max is 9 and the role over is to 0.  Do not increment if the
* active channel limit is not enabled.
*
* @author original: Hab Collector \n
*
* STEP 1: Get limit current from the active channel and separate to its digit parts
* STEP 2: Increment the correspond digit: Integer stops at +10, all others go 9 and roles over to 0
* STEP 3: Update to new limit
* STEP 4: Call screen render to reflect the change
********************************************************************************************************/
void Screen_SetView::incrementDigitUp(void)
{
    int8_t Integer;
    uint8_t Tenths, Hundredths, Thousandths;
    double *LimitCurrent;

    // STEP 1: Get limit current from the active channel and separate to its digit parts
    if (ArbPwrBooster.ActiveChannel == CHANNEL_1)
    {
        if (!ArbPwrBooster.CH1.Limit.Enable)
            return;
        LimitCurrent = &ArbPwrBooster.CH1.Limit.Current;
    }
    else
    {
        if (!ArbPwrBooster.CH2.Limit.Enable)
            return;
        LimitCurrent = &ArbPwrBooster.CH2.Limit.Current;
    }
    digitsFromDouble(*LimitCurrent, &Integer, &Tenths, &Hundredths, &Thousandths);

    // STEP 2: Increment the correspond digit: Integer stops at +10, all others go 9 and roles over to 0
    switch(ArbPwrBooster.SetLimitDigit)
    {
    case INTEGER:
    {
        Integer++;
        if (Integer > 10)
            Integer = 10;
    }
    break;
    case TENTHS:
    {
        Tenths++;
        if (Tenths > 9)
            Tenths = 0;
    }
    break;
    case HUNDREDTHS:
    {
        Hundredths++;
        if (Hundredths > 9)
            Hundredths = 0;
    }
    break;
    case THOUSANDTHS:
    {
        Thousandths++;
        if (Thousandths > 9)
            Thousandths = 0;
    }
    break;
    default:
        break;
    } // END OF SWITCH

    // STEP 3: Update to new limit
    *LimitCurrent = digitsToDouble(&Integer, &Tenths, &Hundredths, &Thousandths);

    // STEP 4: Call screen render to reflect the change
    render_Screen_Set(ArbPwrBooster.ActiveChannel);

} // END OF incrementDigitUp



/********************************************************************************************************
* @brief Decrement the active limit digit by one.  For integers the max it can be decremented to is -10 with
* no role over.  For all other digits the min is 0 and the role over is to 9.  Do not decrement if limit for
* active channel is not enabled.
*
* @author original: Hab Collector \n
*
* STEP 1: Get limit current from the active channel and separate to its digit parts
* STEP 2: Decrement the correspond digit: Integer stops at -10, all others go 0 and roles over to 9
* STEP 3: Update to new limit
* STEP 4: Call screen render to reflect the change
********************************************************************************************************/
void Screen_SetView::incrementDigitDown(void)
{
    int8_t Integer;
    uint8_t Tenths, Hundredths, Thousandths;
    double *LimitCurrent;

    // STEP 1: Get limit current from the active channel and separate to its digit parts
    if (ArbPwrBooster.ActiveChannel == CHANNEL_1)
    {
        if (!ArbPwrBooster.CH1.Limit.Enable)
            return;
        LimitCurrent = &ArbPwrBooster.CH1.Limit.Current;
    }
    else
    {
        if (!ArbPwrBooster.CH2.Limit.Enable)
            return;
        LimitCurrent = &ArbPwrBooster.CH2.Limit.Current;
    }
    digitsFromDouble(*LimitCurrent, &Integer, &Tenths, &Hundredths, &Thousandths);

    // STEP 2: Decrement the correspond digit: Integer stops at -10, all others go 0 and roles over to 9
    switch(ArbPwrBooster.SetLimitDigit)
    {
    case INTEGER:
    {
        Integer--;
        if (Integer < -10)
            Integer = -10;
    }
    break;
    case TENTHS:
    {
        int8_t TempTenths = Tenths;
        TempTenths--;
        if (TempTenths < 0)
            Tenths = 9;
        else
            Tenths = TempTenths;
    }
    break;
    case HUNDREDTHS:
    {
        int8_t TempHundreths = Hundredths;
        TempHundreths--;
        if (TempHundreths < 0)
            Hundredths = 9;
        else
            Hundredths = TempHundreths;
    }
    break;
    case THOUSANDTHS:
    {
        int8_t TempThousandths = Thousandths;
        TempThousandths--;
        if (TempThousandths < 0)
            Thousandths = 9;
        else
            Thousandths = TempThousandths;
    }
    break;
    default:
        break;
    } // END OF SWITCH

    // STEP 3: Create new limit
    *LimitCurrent = digitsToDouble(&Integer, &Tenths, &Hundredths, &Thousandths);

    // STEP 4: Call screen render to reflect the change
    render_Screen_Set(ArbPwrBooster.ActiveChannel);

} // END OF incrementDigitDown



/********************************************************************************************************
* @brief Reset min and max for the active channel.  This is called within  the TouchGFX with a return to
* the main screen.
*
* @author original: Hab Collector \n
*
* @note: Values are reset to 0 - the updating function knows 0 is neither the min or max but
* @note: It is possible user changed current limit so save config
*
* STEP 1: Reset the active channel min max limit
* STEP 2: Save config file
********************************************************************************************************/
void Screen_SetView::resetMinMaxCurrentLimits(void)
{
    // STEP 1: Reset the active channel min max limit
    if (ArbPwrBooster.ActiveChannel == CHANNEL_1)
        ArbPwrBooster.CH1.Measure.ResetCurrentMinMax = true;
    else
        ArbPwrBooster.CH2.Measure.ResetCurrentMinMax = true;

    // STEP 2: Save config file
    saveConfigParameters();

} // END OF resetMinMaxCurrentLimits



/********************************************************************************************************
* @brief update the config file parameters as necessary
*
* @author original: Hab Collector \n
*
* @note: If there was a change from when the screen opening in vars StartValueCurrentLimit or StartValueLimitEnable
*
* STEP 1: Update as necessary based on active channel
********************************************************************************************************/
void Screen_SetView::updateConfigFile(void)
{
    if (ArbPwrBooster.ActiveChannel == CHANNEL_1)
    {
        if ((ArbPwrBooster.CH1.Limit.Current != StartValueCurrentLimit) || (ArbPwrBooster.CH1.Limit.Enable != StartValueLimitEnable))
            saveConfigParameters();
    }
    else
    {
        if ((ArbPwrBooster.CH2.Limit.Current != StartValueCurrentLimit) || (ArbPwrBooster.CH2.Limit.Enable != StartValueLimitEnable))
            saveConfigParameters();
    }

} // END OF updateConfigFile
