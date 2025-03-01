/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SCREEN_SETVIEWBASE_HPP
#define SCREEN_SETVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/screen_set_screen/Screen_SetPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/containers/buttons/Buttons.hpp>
#include <touchgfx/widgets/RadioButton.hpp>
#include <touchgfx/widgets/RadioButtonGroup.hpp>

class Screen_SetViewBase : public touchgfx::View<Screen_SetPresenter>
{
public:
    Screen_SetViewBase();
    virtual ~Screen_SetViewBase();
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Image image1;
    touchgfx::TextArea textArea_ResetMinMax;
    touchgfx::IconButtonStyle< touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  >  flexButton_ResetMinMax;
    touchgfx::IconButtonStyle< touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  >  flexButton_Home;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  flexButton_AmpsDigit_3;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  flexButton_AmpsDigit_2;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  flexButton_AmpsDigit_1;
    touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  flexButton_AmpsDigit_0;
    touchgfx::TextArea textArea_DisableLimit;
    touchgfx::TextArea textArea_Units;
    touchgfx::RadioButtonGroup<1> radioButtonGroup1;
    touchgfx::RadioButton radioButton_EnableLimit;
    touchgfx::TextArea textArea_DecimalPoint;
    touchgfx::TextArea textArea__AmpsDigit_3;
    touchgfx::TextArea textArea__AmpsDigit_2;
    touchgfx::TextArea textArea__AmpsDigit_1;
    touchgfx::TextArea textArea_AmpsDigit_0;
    touchgfx::TextArea textArea_CH_ToSet;
    touchgfx::IconButtonStyle< touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  >  flexButton_DigitDown;
    touchgfx::IconButtonStyle< touchgfx::BoxWithBorderButtonStyle< touchgfx::ClickButtonTrigger >  >  flexButton_DigitUp;
    touchgfx::TextArea textArea_TitleSet;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<Screen_SetViewBase, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

    /*
     * Callback Handler Declarations
     */
    void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);

};

#endif // SCREEN_SETVIEWBASE_HPP
