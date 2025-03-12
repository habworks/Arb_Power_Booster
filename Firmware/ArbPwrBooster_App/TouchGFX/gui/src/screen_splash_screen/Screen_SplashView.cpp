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
/********************************************************************************************************
* @brief Update the screen on entry.  It is called by the end of the Splash Screen transition interaction.
*
*
* @author original: Hab Collector \n
*
* @note: Splash screen occurs after a POR - Can use this to init various parameters or do it elsewhere
*
* STEP 1: Set the active screen
* STEP 2: Update Channel 1
* STEP 3: Update Channel 2
********************************************************************************************************/
void Screen_SplashView::update_Screen_Splash(void)
{
    ArbPwrBooster.Screen = SPLASH_SCREEN;
}
