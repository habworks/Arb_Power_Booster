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
    virtual void outputToggle_CH1(void);
    virtual void outputSwitch_CH1(void);
    virtual void inputImpedanceToggle_CH1(void);
    virtual void inputImpedanceSet_CH1(void);
    virtual void setActiveLimit_CH1(void);
    virtual void outputToggle_CH2(void);
    virtual void outputSwitch_CH2(void);
    virtual void inputImpedanceToggle_CH2(void);
    virtual void inputImpedanceSet_CH2(void);
    virtual void setActiveLimit_CH2(void);
//    virtual void POR_setControlDefaults(void);
    // ADDED FUNCTIONS CALLED BY PRESENTER
    virtual void updateMainScreen_View(void);
protected:
};

#endif // SCREEN_MAINVIEW_HPP
