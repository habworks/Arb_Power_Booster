#include <gui/screen_main_screen/Screen_MainView.hpp>
#include <gui/screen_main_screen/Screen_MainPresenter.hpp>

Screen_MainPresenter::Screen_MainPresenter(Screen_MainView& v)
    : view(v)
{

}

void Screen_MainPresenter::activate()
{

}

void Screen_MainPresenter::deactivate()
{

}


// HAB ADDED
void Screen_MainPresenter::updateMainScreen_Presenter(void)
{
    view.updateMainScreen_View();
}
