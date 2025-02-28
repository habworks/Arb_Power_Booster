#ifndef SCREEN_CONFIGVIEW_HPP
#define SCREEN_CONFIGVIEW_HPP

#include <gui_generated/screen_config_screen/Screen_ConfigViewBase.hpp>
#include <gui/screen_config_screen/Screen_ConfigPresenter.hpp>

class Screen_ConfigView : public Screen_ConfigViewBase
{
public:
    Screen_ConfigView();
    virtual ~Screen_ConfigView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN_CONFIGVIEW_HPP
