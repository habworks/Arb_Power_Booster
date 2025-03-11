#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    // START HAB ADDED
    virtual void updateMainScreen_Presenter(void){}
    virtual void updateConfigScreen_Presenter(void){}
    // END HAD ADDED

    void bind(Model* m)
    {
        model = m;
    }
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
