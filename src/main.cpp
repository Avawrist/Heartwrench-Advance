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
#include "exit.h"
#include "player.h"

int main()
{   
    bn::core::init();
    
    // Camera
    //bn::camera_ptr camera = bn::camera_ptr::create(0, 0);

    // Create Test Room
    Room* current_room_ptr = new Room(ROOM_TEST);

    BN_LOG("Bytes allocated in IWRAM: ", bn::memory::used_stack_iwram());
    BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());

    // Game Loop
    while(true)
    {
        // Update Game Objects from Back of the list to the front
        BN_PROFILER_START("");
        for(int32 i = current_room_ptr->game_objects.size() - 1; i >= 0; i--)
        {
            (current_room_ptr->game_objects.data())[i]->update(current_room_ptr->game_objects, 
                                                                current_room_ptr->bg_ptr.value(),
                                                                current_room_ptr->cells,
                                                                current_room_ptr->bg_item.value(),
                                                                current_room_ptr->camera.value());
            (current_room_ptr->game_objects.data())[i]->draw();   
        }
        BN_PROFILER_STOP();
        //bn::profiler::show();

        // If player died, reload the room
        if(((Player*)(current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX)))->state == STATE_DEAD)
        {current_room_ptr->clear(); 
         current_room_ptr->load(current_room_ptr->current_room);}

        // Check Exit 1
        Exit* exit_1_ptr = (Exit*)(current_room_ptr->game_objects.at(EXIT_1_OBJECT_LIST_INDEX));
        if(exit_1_ptr->is_triggered) 
        {current_room_ptr->clear(); 
         current_room_ptr->load((RoomName)(exit_1_ptr->go_to_room_enum));}

        // Check Exit 2
        Exit* exit_2_ptr = (Exit*)(current_room_ptr->game_objects.at(EXIT_2_OBJECT_LIST_INDEX));
        if(exit_2_ptr->is_triggered) 
        {current_room_ptr->clear(); 
         current_room_ptr->load((RoomName)(exit_2_ptr->go_to_room_enum));}

        // Check Exit 3
        Exit* exit_3_ptr = (Exit*)(current_room_ptr->game_objects.at(EXIT_3_OBJECT_LIST_INDEX));
        if(exit_3_ptr->is_triggered) 
        {current_room_ptr->clear(); 
         current_room_ptr->load((RoomName)(exit_3_ptr->go_to_room_enum));}

        // Check Exit 4
        Exit* exit_4_ptr = (Exit*)(current_room_ptr->game_objects.at(EXIT_4_OBJECT_LIST_INDEX));
        if(exit_4_ptr->is_triggered) 
        {current_room_ptr->clear(); 
         current_room_ptr->load((RoomName)(exit_4_ptr->go_to_room_enum));}

        // Do NOT delete the Exit pointers, they are handled by the Room object.

        // Update Camera //
        #define HALF_SCREEN_WIDTH  120
        #define HALF_SCREEN_HEIGHT 80
        #define TILESET_HEIGHT     8
        bn::regular_bg_ptr temp_bg_ptr = current_room_ptr->bg_ptr.value();
        int32 half_room_width_pixels   = temp_bg_ptr.dimensions().width()  / 2;
	    int32 half_room_height_pixels  = temp_bg_ptr.dimensions().height() / 2;
        int32 new_cam_x = current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos().x().integer();
        int32 new_cam_y = current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos().y().integer();
        new_cam_x = clamp(-half_room_width_pixels + HALF_SCREEN_WIDTH,  
                           half_room_width_pixels - HALF_SCREEN_WIDTH, 
                           new_cam_x);
        new_cam_y = clamp(-half_room_height_pixels + HALF_SCREEN_HEIGHT + TILESET_HEIGHT, 
                           half_room_height_pixels - HALF_SCREEN_HEIGHT, 
                           new_cam_y);
        current_room_ptr->camera.value().set_position(new_cam_x, new_cam_y);
        
        // Update Core
        bn::core::update();
        //BN_LOG(bn::core::last_missed_frames());
    }
}
