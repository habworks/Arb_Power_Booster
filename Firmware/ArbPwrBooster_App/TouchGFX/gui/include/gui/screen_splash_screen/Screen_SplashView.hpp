#ifndef SCREEN_SPLASHVIEW_HPP
#define SCREEN_SPLASHVIEW_HPP

#include <gui_generated/screen_splash_screen/Screen_SplashViewBase.hpp>
#include <gui/screen_splash_screen/Screen_SplashPresenter.hpp>

class Screen_SplashView : public Screen_SplashViewBase
{
public:
    Screen_SplashView();
    virtual ~Screen_SplashView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    // ADDED FUNCTIONS
    virtual void update_Screen_Splash(void);
    virtual void GUI_IntroComplete(void);
protected:
};

#endif // SCREEN_SPLASHVIEW_HPP
