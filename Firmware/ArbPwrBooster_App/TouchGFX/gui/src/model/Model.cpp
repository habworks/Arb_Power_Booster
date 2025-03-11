#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

// ADDED INCLUDES
#include "touchgfx/Color.hpp"
#include "../../../../../../touchgfx/generated/texts/include/texts/TextKeysAndLanguages.hpp"
#include "Main_Support.h"
#include "cmsis_os2.h"

// ADDED EXTERNS
extern osSemaphoreId_t DisplayUpdateSemaphoreHandle;

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
    if (DisplayUpdateSemaphoreHandle != NULL)
    {
        if (osSemaphoreAcquire(DisplayUpdateSemaphoreHandle, 10U) == osOK)
        {
            activeDisplayUpdate();
        }
    }
}


void Model::activeDisplayUpdate(void)
{
    if (ArbPwrBooster.Screen == MAIN_SCREEN)
    {
        modelListener->updateMainScreen_Presenter();
    }

    if (ArbPwrBooster.Screen == CONFIG_SCREEN)
    {
        modelListener->updateConfigScreen_Presenter();
    }
}
