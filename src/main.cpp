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
#include "game_object.h"
#include "player.h"
#include "devil_platform.h"
#include "angel_platform.h"
#include "scythe_platform.h"
#include "room.h"

int main()
{   
    bn::core::init();
    
    // Camera
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);

    // Game Objects test
    bn::vector<GameObject*, MAX_GAME_OBJECTS> game_objects;

    // Create Player FIRST. They will always be updated last.
    Player* player_ptr = new Player();
    game_objects.push_back(player_ptr);
    game_objects.back()->setCamera(camera);
    game_objects.back()->setPos(0, -64);

    // Create space for Scythe next.
    ScythePlatform* scythe_ptr = NULL;
    game_objects.push_back(scythe_ptr);

    // Create test Moving Platforms next. They must always be updated first.
    game_objects.push_back(new DevilPlatform(bn::point(-32, -32), bn::point(-192, -192)));
    game_objects.back()->setCamera(camera);
    
    game_objects.push_back(new DevilPlatform(bn::point(96, 44), bn::point(228, 44)));
    game_objects.back()->setCamera(camera);

    // Create Test Room
    Room room(ROOM_TEST, camera);

    BN_LOG("Game Objects count: ", game_objects.size());
    BN_LOG("Bytes allocated in IWRAM: ", bn::memory::used_stack_iwram());
    BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());
    
    // Game Loop
    while(true)
    {
        // Update Game Objects from Back of the list to the front
        BN_PROFILER_START("");
        for(int32 i = game_objects.size() - 1; i >= 0; i--)
        {
            if((game_objects.data())[i] != NULL)
            {
                (game_objects.data())[i]->update(game_objects, room, camera);
                (game_objects.data())[i]->draw();
            }
        }
        BN_PROFILER_STOP();
        //bn::profiler::show();

        // Update Camera
        camera.set_position(player_ptr->pos());
        
        // Update Core
        bn::core::update();
        //BN_LOG(bn::core::last_missed_frames());
    }
}
