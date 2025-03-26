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
    Level current_level(LEVEL_TEST);

    // Game Loop
    while(true)
    {
        // Update Game
        //bool profile = false;

        //if(bn::keypad::l_held()) {profile = true;}
        //if(profile) {BN_PROFILER_START("game update");}

        // Update Levels
        if(current_level.cam_is_scrolling)
        {
            current_level.updateCamera();
        }
        else
        {
            current_level.updateAndDraw();
            current_level.updateCamera();
            current_level.freeObjects();
            current_level.reloadOnDeath();
            current_level.transitionRoom();
        }

        BN_LOG("Bytes allocated in IWRAM: ", bn::memory::used_stack_iwram());
        BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());

        /*
        BN_LOG("Game Object IDs: ");
        for(int32 i = 0; i < current_level.current_room.game_objects.size(); i++)
        {BN_LOG(current_level.current_room.game_objects.at(i)->object_id);}

        BN_LOG("Unloaded Object IDs: ");
        for(int32 j = 0; j < current_level.current_room.unloaded_objects.size(); j++)
        {BN_LOG(current_level.current_room.unloaded_objects.at(j).loaded_instance_id);}
        */

        //if(profile)
        //{
        //    BN_PROFILER_STOP();
        //    bn::profiler::show();
        //}

        // Update Core
        bn::core::update();
        //BN_LOG(bn::core::last_missed_frames());
    }
}
