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
    // HAD ADDED THIS CODE TO MODEL TICK
    // Verify semaphore as valid and available - if so take it and update the active display
    if (DisplayUpdateSemaphoreHandle != NULL)
    {
        if (osSemaphoreAcquire(DisplayUpdateSemaphoreHandle, 10U) == osOK)
        {
            activeDisplayUpdate();
        }
    }
}


/********************************************************************************************************
* @brief Called by the model tick in turn this function calls an action to update either the config screen
* or the main screen which both rely on live udpates.
*
* @author original: Hab Collector \n
*
* STEP 1: If main screen active perform live update
* STEP 2: UIf config screen active perform live update
********************************************************************************************************/
void Model::activeDisplayUpdate(void)
{
    // STEP 1: If main screen active perform live update
    if (ArbPwrBooster.Screen == MAIN_SCREEN)
    {
        modelListener->updateMainScreen_Presenter();
    }

    // STEP 2: If config screen active perform live update
    if (ArbPwrBooster.Screen == CONFIG_SCREEN)
    {
        modelListener->updateConfigScreen_Presenter();
    }

} // END OF activeDisplayUpdate
