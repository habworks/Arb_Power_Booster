#ifndef SCREEN_CONFIGPRESENTER_HPP
#define SCREEN_CONFIGPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_ConfigView;

class Screen_ConfigPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_ConfigPresenter(Screen_ConfigView& v);

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

    virtual ~Screen_ConfigPresenter() {}

private:
    Screen_ConfigPresenter();

    Screen_ConfigView& view;
};

#endif // SCREEN_CONFIGPRESENTER_HPP
