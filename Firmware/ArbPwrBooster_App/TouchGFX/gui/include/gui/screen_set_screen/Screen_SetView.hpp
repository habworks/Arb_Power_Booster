#ifndef SCREEN_SETVIEW_HPP
#define SCREEN_SETVIEW_HPP

#include <gui_generated/screen_set_screen/Screen_SetViewBase.hpp>
#include <gui/screen_set_screen/Screen_SetPresenter.hpp>

class Screen_SetView : public Screen_SetViewBase
{
public:
    Screen_SetView();
    virtual ~Screen_SetView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    // ADDED FUNCTIONS
    virtual void update_Screen_Set(void);
protected:
};

#endif // SCREEN_SETVIEW_HPP
