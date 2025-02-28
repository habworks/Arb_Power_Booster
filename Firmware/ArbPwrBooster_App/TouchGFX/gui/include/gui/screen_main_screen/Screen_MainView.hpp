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
protected:
};

#endif // SCREEN_MAINVIEW_HPP
