#ifndef SCREEN_MAINPRESENTER_HPP
#define SCREEN_MAINPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_MainView;

class Screen_MainPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_MainPresenter(Screen_MainView& v);

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

    virtual ~Screen_MainPresenter() {}

    // HAB ADDED
    virtual void updateMainScreen_Presenter(void);

private:
    Screen_MainPresenter();

    Screen_MainView& view;
};

#endif // SCREEN_MAINPRESENTER_HPP
