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
#include "room.h"

int main()
{   
    bn::core::init();
    
    // Create Test Room
    Room* current_room_ptr = new Room(ROOM_TEST, bn::point(0, 0));

    //BN_LOG("Bytes allocated in IWRAM: ", bn::memory::used_stack_iwram());
    //BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());

    // Game Loop
    while(true)
    {
        // Update Game

        //BN_PROFILER_START("");

        // Update Rooms
        current_room_ptr->updateAndDraw();
        current_room_ptr->updateCamera();
        current_room_ptr->freeInactiveObjects();
        current_room_ptr->checkConditions();

        //BN_PROFILER_STOP();
        //bn::profiler::show();    

        // Update Core
        bn::core::update();
        //BN_LOG(bn::core::last_missed_frames());
    }

    delete current_room_ptr;
}
