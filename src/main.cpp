// Butano
#include "bn_log.h"
#include "bn_profiler.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_camera_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_memory.h"
#include "bn_vector.h"

// Common
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

// My Libs
#include "utility.h"
#include "level.h"

int main()
{   
    bn::core::init();
    
    // Create Test Level
    Level* current_level_ptr = new Level(LEVEL_TEST);

    BN_LOG("Bytes allocated in IWRAM: ", bn::memory::used_stack_iwram());
    BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());

    // Game Loop
    while(true)
    {
        // Update Game

        //BN_PROFILER_START("");

        // Update Levels
        if(current_level_ptr->cam_is_scrolling)
        {
            current_level_ptr->updateCamera();
        }
        else
        {
            current_level_ptr->updateAndDraw();
            current_level_ptr->updateCamera();
            current_level_ptr->freeInactiveObjects();
            current_level_ptr->reloadOnDeath();
            current_level_ptr->transitionRoom();
        }

        //BN_PROFILER_STOP();
        //bn::profiler::show();

        // Update Core
        bn::core::update();
        BN_LOG(bn::core::last_missed_frames());
    }

    delete current_level_ptr;
}
