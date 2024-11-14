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
#include "moving_platform.h"
#include "room.h"

int main()
{   
    bn::core::init();
    
    // Camera
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);

    // Game Objects test
    bn::vector<GameObject*, MAX_GAME_OBJECTS> game_objects;

    // Create test Moving Platforms MUST BE UPDATED FIRST
    game_objects.push_back(new MovingPlatform(bn::point(32, -128), bn::point(32, -192)));
    game_objects.back()->setCamera(camera);

    game_objects.push_back(new MovingPlatform(bn::point(-32, -128), bn::point(-96, -192)));
    game_objects.back()->setCamera(camera);

    game_objects.push_back(new MovingPlatform(bn::point(96, 48), bn::point(228, 48)));
    game_objects.back()->setCamera(camera);

    // Create Player
    game_objects.push_back(new Player());
    game_objects.back()->setCamera(camera);
    game_objects.back()->setPos(0, -64);

    // Create Test Room
    Room room(ROOM_TEST_2, camera);

    BN_LOG("Game Objects count: ", game_objects.size());
    BN_LOG("Bytes allocated in IWRAM: ", bn::memory::used_stack_iwram());
    BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());
    
    // Game Loop
    while(true)
    {
        // Update Game Objects
        BN_PROFILER_START("");
        for(int32 i = 0; i < game_objects.size(); i++)
        {
            (game_objects.data())[i]->update(game_objects, room);
            (game_objects.data())[i]->draw();
        }
        BN_PROFILER_STOP();
        //bn::profiler::show();

        // Update Camera
        camera.set_position(game_objects[3]->pos());
        
        // Update Core
        bn::core::update();
        //BN_LOG(bn::core::last_missed_frames());
    }
}
