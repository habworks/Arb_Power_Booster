#ifndef SCREEN_SETPRESENTER_HPP
#define SCREEN_SETPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_SetView;

class Screen_SetPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_SetPresenter(Screen_SetView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Screen_SetPresenter() {}

private:
    Screen_SetPresenter();

    Screen_SetView& view;
};

#endif // SCREEN_SETPRESENTER_HPP
