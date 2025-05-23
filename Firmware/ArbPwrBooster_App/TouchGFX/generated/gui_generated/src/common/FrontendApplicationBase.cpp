/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <new>
#include <gui_generated/common/FrontendApplicationBase.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Texts.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <gui/screen_splash_screen/Screen_SplashView.hpp>
#include <gui/screen_splash_screen/Screen_SplashPresenter.hpp>
#include <gui/screen_main_screen/Screen_MainView.hpp>
#include <gui/screen_main_screen/Screen_MainPresenter.hpp>
#include <gui/screen_config_screen/Screen_ConfigView.hpp>
#include <gui/screen_config_screen/Screen_ConfigPresenter.hpp>
#include <gui/screen_set_screen/Screen_SetView.hpp>
#include <gui/screen_set_screen/Screen_SetPresenter.hpp>

using namespace touchgfx;

FrontendApplicationBase::FrontendApplicationBase(Model& m, FrontendHeap& heap)
    : touchgfx::MVPApplication(),
      transitionCallback(),
      frontendHeap(heap),
      model(m)
{
    touchgfx::HAL::getInstance()->setDisplayOrientation(touchgfx::ORIENTATION_LANDSCAPE);
    touchgfx::Texts::setLanguage(GB);
    reinterpret_cast<touchgfx::LCD16bpp&>(touchgfx::HAL::lcd()).enableTextureMapperAll();
    reinterpret_cast<touchgfx::LCD16bpp&>(touchgfx::HAL::lcd()).enableDecompressorL8_All();
    reinterpret_cast<touchgfx::LCD16bpp&>(touchgfx::HAL::lcd()).enableDecompressorRGB();
}

/*
 * Screen Transition Declarations
 */

// Screen_Splash

void FrontendApplicationBase::gotoScreen_SplashScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplicationBase::gotoScreen_SplashScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_SplashScreenNoTransitionImpl()
{
    touchgfx::makeTransition<Screen_SplashView, Screen_SplashPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// Screen_Main

void FrontendApplicationBase::gotoScreen_MainScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplicationBase::gotoScreen_MainScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_MainScreenNoTransitionImpl()
{
    touchgfx::makeTransition<Screen_MainView, Screen_MainPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplicationBase::gotoScreen_MainScreenSlideTransitionWest()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplicationBase::gotoScreen_MainScreenSlideTransitionWestImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_MainScreenSlideTransitionWestImpl()
{
    touchgfx::makeTransition<Screen_MainView, Screen_MainPresenter, touchgfx::SlideTransition<WEST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// Screen_Config

void FrontendApplicationBase::gotoScreen_ConfigScreenSlideTransitionEast()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplicationBase::gotoScreen_ConfigScreenSlideTransitionEastImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_ConfigScreenSlideTransitionEastImpl()
{
    touchgfx::makeTransition<Screen_ConfigView, Screen_ConfigPresenter, touchgfx::SlideTransition<EAST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// Screen_Set

void FrontendApplicationBase::gotoScreen_SetScreenSlideTransitionEast()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplicationBase::gotoScreen_SetScreenSlideTransitionEastImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_SetScreenSlideTransitionEastImpl()
{
    touchgfx::makeTransition<Screen_SetView, Screen_SetPresenter, touchgfx::SlideTransition<EAST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
