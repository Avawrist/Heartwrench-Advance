// Butano
#include "bn_sprites.h"
#include "bn_log.h"
#include "bn_profiler.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_camera_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_memory.h"
#include "bn_vector.h"

// My Libs
#include "utility.h"
#include "game_state.h"
#include "level.h"

int main()
{   
    bn::core::init();
    
    // Initialize GameState
    GameState game_state;

    // Create Test Level
    Level current_level(LEVEL_TEST);

    // Game Loop
    while(true)
    {
        // Update Level
        if(current_level.cam_is_scrolling) 
        {
            current_level.updateCamera();
            current_level.updateFade();
        }
        else                             
        {
            current_level.updateAll();
        }

        // Update Global Timer
        current_level.updateGlobalTimer();

        // Update GameState
        game_state.update();
       
        bn::core::update();
    }
}

/* 
//bool profile = false;

if(bn::keypad::l_held()) {profile = true;}
if(profile) {BN_PROFILER_START("game update");}

BN_LOG("Bytes allocated in IWRAM: ", bn::memory::used_stack_iwram());
BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());

BN_LOG("Game Object IDs: ");
for(int32 i = 0; i < current_level.current_room.game_objects.size(); i++)
{BN_LOG(current_level.current_room.game_objects.at(i)->object_id);}

BN_LOG("Unloaded Object IDs: ");
for(int32 j = 0; j < current_level.current_room.unloaded_objects.size(); j++)
{BN_LOG(current_level.current_room.unloaded_objects.at(j).loaded_instance_id);}

if(profile)
{
    BN_PROFILER_STOP();
    bn::profiler::show();
}

//BN_LOG(bn::core::last_missed_frames());
*/
