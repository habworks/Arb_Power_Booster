#ifndef SCREEN_MAINVIEW_HPP
#define SCREEN_MAINVIEW_HPP

#include <gui_generated/screen_main_screen/Screen_MainViewBase.hpp>
#include <gui/screen_main_screen/Screen_MainPresenter.hpp>

class Screen_MainView : public Screen_MainViewBase
{
public:
    Screen_MainView();
    virtual ~Screen_MainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    // ADDED FUNCTIONS
    virtual void update_Screen_Main(void);
    virtual void switchOutput_CH1(void);
    virtual void switchOutput_CH2(void);
protected:
};

#endif // SCREEN_MAINVIEW_HPP
