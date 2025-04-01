#include <gui/screen_splash_screen/Screen_SplashView.hpp>

// ADDED INCLUDES
#include "Main_Support.h"
#include "ADC_Support.h"


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
* @author original: Hab Collector \n
*
* @note: Splash screen occurs after a POR - Can use this to init various parameters or do it elsewhere
*
* STEP 1: Set the active screen
********************************************************************************************************/
void Screen_SplashView::update_Screen_Splash(void)
{
    // STEP 1: Set the active screen
    ArbPwrBooster.Screen = SPLASH_SCREEN;

} // END OF update_Screen_Splash



/********************************************************************************************************
* @brief Signal that the GUI introduction is complete - this signals the start of processing
*
* @author original: Hab Collector \n
*
* @note: Splash screen occurs after a POR - Can use this to init various parameters or do it elsewhere
*
* STEP 1: Set the active screen
********************************************************************************************************/
void Screen_SplashView::GUI_IntroComplete(void)
{
    // STEP 1: Alert the system the GUI intro is completed
    ArbPwrBooster.Ready = true;
    ADC13_StartConversion();

} // END OF GUI_IntroComplete
