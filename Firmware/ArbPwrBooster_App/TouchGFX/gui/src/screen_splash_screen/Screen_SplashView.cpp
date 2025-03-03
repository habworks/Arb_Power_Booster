#include <gui/screen_splash_screen/Screen_SplashView.hpp>

// ADDED INCLUDES
#include "Main_Support.h"


Screen_SplashView::Screen_SplashView()
{

}

void Screen_SplashView::setupScreen()
{
    Screen_SplashViewBase::setupScreen();
}

void Screen_SplashView::tearDownScreen()
{
    Screen_SplashViewBase::tearDownScreen();
}


// ADDED FUNCTIONS
void Screen_SplashView::update_Screen_Splash(void)
{
    ArbPwrBoosterStatus.Screen = SPLASH_SCREEN;
    Init_ArbPwrBooster();
}
