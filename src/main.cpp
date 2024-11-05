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

#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_items_test_room.h"

#include "bn_span.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_cell_info.h"

// Common
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

// My Libs
#include "utility.h"
#include "game_object.h"
#include "player.h"
#include "block.h"

int main()
{   
    bn::core::init();
    
    // Camera
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);

    // Game Objects test
    #define ROOM_W 64
    #define ROOM_H 64
    #define MAX_OBJECTS 1200
    bn::vector<GameObject*, MAX_OBJECTS> game_objects;
 
    // Create Player: MUST BE UPDATED FIRST IN GAMEOBJECT ARRAY
    game_objects.push_back(new Player());
    game_objects.back()->setCamera(camera);

    // Create Test Room
    bn::optional<bn::regular_bg_ptr> bg_ptr = bn::regular_bg_items::test_room.create_bg(0, 0);
    bn::span<const bn::regular_bg_map_cell> cells = bg_ptr->map().cells_ref().value();
    bg_ptr->set_camera(camera);

    #define BLOCK_INDEX 1
    #define BLOCK_WIDTH 8

    for(uint32 x = 0; x < ROOM_W; x++)
    {
        for(uint32 y = 0; y < ROOM_H; y++)
        {
            bn::regular_bg_map_cell       cell_index = cells[bn::regular_bg_items::test_room.map_item().cell_index(x, y)];
            bn::regular_bg_map_cell_info  cell_info(cell_index);
            if(cell_info.tile_index() == BLOCK_INDEX)
            {
                int32 converted_x = ((x - (ROOM_W / 2)) * BLOCK_WIDTH) + (BLOCK_WIDTH / 2);
                int32 converted_y = ((y - (ROOM_H / 2)) * BLOCK_WIDTH) + (BLOCK_WIDTH / 2);
                game_objects.push_back(new Block());
                game_objects.back()->setCamera(camera);
                game_objects.back()->setPos(converted_x, converted_y); 
            }
        }
    }
  
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
            (game_objects.data())[i]->update(game_objects.data(), game_objects.size());
            (game_objects.data())[i]->draw();
        }
        BN_PROFILER_STOP();
        //bn::profiler::show();

        // Update Camera
        camera.set_position(game_objects[0]->pos());
        
        // Update Core
        bn::core::update();
        BN_LOG(bn::core::last_missed_frames());
    }
}
