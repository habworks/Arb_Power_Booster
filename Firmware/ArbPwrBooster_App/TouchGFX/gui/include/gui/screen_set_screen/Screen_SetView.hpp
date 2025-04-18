#ifndef SCREEN_SETVIEW_HPP
#define SCREEN_SETVIEW_HPP

#include <gui_generated/screen_set_screen/Screen_SetViewBase.hpp>
#include <gui/screen_set_screen/Screen_SetPresenter.hpp>
// ADDED INCLUDES
#include "Main_Support.h"

class Screen_SetView : public Screen_SetViewBase
{
public:
    Screen_SetView();
    virtual ~Screen_SetView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    // ADDED FUNCTIONS
    virtual void update_Screen_Set(void);
    virtual void enableCurrentLimit(void);
    virtual void disableCurrentLimit(void);
    virtual void setIntegerToUpdate(void);
    virtual void setTenthsToUpdate(void);
    virtual void setHundredthsToUpdate(void);
    virtual void setThousandthsToUpdate(void);
    virtual void incrementDigitUp(void);
    virtual void incrementDigitDown(void);
    virtual void resetMinMaxCurrentLimits(void);
    virtual void updateConfigFile(void);
    // ADDED FUNCTIONS NOT CALLED BY TOUCHGFX
    virtual void render_Screen_Set(Type_Channel Channel);
protected:
};

#endif // SCREEN_SETVIEW_HPP
