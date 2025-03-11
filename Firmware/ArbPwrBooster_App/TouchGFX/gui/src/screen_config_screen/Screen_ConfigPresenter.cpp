#include <gui/screen_config_screen/Screen_ConfigView.hpp>
#include <gui/screen_config_screen/Screen_ConfigPresenter.hpp>

Screen_ConfigPresenter::Screen_ConfigPresenter(Screen_ConfigView& v)
    : view(v)
{

}

void Screen_ConfigPresenter::activate()
{

}

void Screen_ConfigPresenter::deactivate()
{

}


// HAB ADDED
void Screen_ConfigPresenter::updateConfigScreen_Presenter(void)
{
    view.updateConfigScreen_View();
}
