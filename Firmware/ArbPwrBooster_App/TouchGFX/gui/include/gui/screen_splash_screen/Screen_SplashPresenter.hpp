#ifndef SCREEN_SPLASHPRESENTER_HPP
#define SCREEN_SPLASHPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_SplashView;

class Screen_SplashPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_SplashPresenter(Screen_SplashView& v);

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

    virtual ~Screen_SplashPresenter() {}

private:
    Screen_SplashPresenter();

    Screen_SplashView& view;
};

#endif // SCREEN_SPLASHPRESENTER_HPP
