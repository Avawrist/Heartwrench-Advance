#include "level.h"

Level::Level(LevelName level_name)
{
    load(level_name);
}

Level::~Level()
{
    clear();
}

void Level::clear()
{
    
    // Free level pointers
    camera.reset();
    bg_ptr.reset();
    backdrop_ptr.reset();
    bg_item.reset();

    BN_LOG("=== Level cleared ===");
    BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());

}

void Level::load(LevelName level_name)
{
    if(level_name == NO_LEVEL) {return;}

    camera       = bn::camera_ptr::create(0, 0);
    player_spawn = bn::point(0, 0);

    cam_is_scrolling = false;
    cam_x_offset     = 0;
    cam_y_offset     = 0;

    // Record current level & pos
    current_level_name = level_name;

    // Initialize Variables
    switch(level_name)
    {
        case LEVEL_TEST:

            // Set Room //
            current_room = Room(ROOM_TEST_1, camera.value());
            
            // Load BG //
            backdrop_ptr = bn::regular_bg_items::test_bg.create_bg(0, 0);
            bg_ptr       = bn::regular_bg_items::test_level.create_bg(0, 0);
            bg_item      = bn::regular_bg_items::test_level;

        break;

        default:

            BN_LOG("Level creation failed - Level Name not found.");
            return;

        break;
    }

    cells = bg_ptr->map().cells_ref().value();
    
    // Set Camera
    backdrop_ptr->set_camera(camera.value());
    bg_ptr->set_camera(camera.value());

    BN_LOG("=== Level loaded ===");
    BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());
    
}

void Level::reload()
{
    clear(); 
    load(current_level_name);
}

void Level::updateAndDraw()
{

    for(int32 i = current_room.game_objects.size() - 1; i >= 0; i--)
    {
        if(current_room.game_objects.data()[i] != NULL)
        {
            current_room.game_objects.data()[i]->update(current_room.room_bounds,
                                                        current_room.game_objects,
                                                        bg_ptr.value(),
                                                        cells,
                                                        bg_item.value(),
                                                        camera.value());
            current_room.game_objects.data()[i]->draw();
        }   
    } 

}

void Level::updateCamera()
{
    #define HALF_SCREEN_WIDTH  120
    #define HALF_SCREEN_HEIGHT 80

    if(cam_is_scrolling)
    {
        // This method does NOT support diagonal scrolling. 
        // Don't even try it babyyyy.

        // No clamp - apply offsets gradually
        if(abs(cam_x_offset) > 0)
        {
            int32 cam_x_offset_unit = (abs(cam_x_offset) / cam_x_offset) * SCROLL_SPEED;
            camera.value().set_x(camera.value().x() + cam_x_offset_unit);

            cam_x_offset -= cam_x_offset_unit;
        }
        else if(abs(cam_y_offset) > 0)
        {
            int32 cam_y_offset_unit = (abs(cam_y_offset) / cam_y_offset) * SCROLL_SPEED;
            camera.value().set_y(camera.value().y() + cam_y_offset_unit);

            cam_y_offset -= cam_y_offset_unit;
        }
        else
        {
            // Both offsets are 0. Scroll is finished.
            cam_is_scrolling = false;
        }

    }
    else
    {
        // Typical camera behavior - Follow the player & clamp to the room bounds. 
        int32 new_cam_x = current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos().x().integer();
        int32 new_cam_y = current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos().y().integer();
        new_cam_x = clamp(current_room.room_bounds.left_bound  + HALF_SCREEN_WIDTH,  
                          current_room.room_bounds.right_bound - HALF_SCREEN_WIDTH, 
                          new_cam_x);
        new_cam_y = clamp(current_room.room_bounds.top_bound    + HALF_SCREEN_HEIGHT, 
                          current_room.room_bounds.bottom_bound - HALF_SCREEN_HEIGHT,
                          new_cam_y);
        camera.value().set_position(new_cam_x, new_cam_y);

    }

}

void Level::reloadOnDeath()
{
    // If player died, reload the level
    if(((Player*)(current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX)))->is_dead)
    {
        reload();
    }
}

void Level::freeInactiveObjects()
{
    // Get an iterator to the gameobjects vector starting after the first two entries
    // if object at current iterator is NULL
    // erase the object :D 

    bn::ivector<GameObject*>::iterator current = current_room.game_objects.begin();
    bn::ivector<GameObject*>::iterator last    = current_room.game_objects.end();
    current++; // Skip player index
    while(current != last)
    {
        if((*current)->inactive)
        {
            // Erase game object
            current_room.game_objects.erase(current);
            delete *current;
        }
        current++;
    }

    updateIndexes();
}

void Level::updateIndexes()
{    
    for(int32 i = current_room.game_objects.size() - 1; i >= 0; i--)
    {
        current_room.game_objects.data()[i]->object_id = i;
    }
}

void Level::transitionRoom()
{
    #define SCREEN_WIDTH  240
    #define SCREEN_HEIGHT 160

    if(current_room.game_objects.size() == 0) {return;}

    Player temp_player = Player(*((Player*)(current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX))));
    
    if(temp_player.pos().x() > current_room.room_bounds.right_bound)
    {
        if(current_room.right_neighbor != NO_ROOM)
        {
            // Create the neighbor room
            current_room = Room(current_room.right_neighbor, camera.value());

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(temp_player);

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = SCREEN_WIDTH;
            cam_y_offset     = 0;

            return;
        }
    }

    else if(temp_player.pos().x() < current_room.room_bounds.left_bound)
    {
        if(current_room.left_neighbor != NO_ROOM)
        {
            // Create the neighbor room
            current_room = Room(current_room.right_neighbor, camera.value());

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(temp_player);

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = -SCREEN_WIDTH;
            cam_y_offset     = 0;

            return;
        }
    }

    else if(temp_player.pos().y() < current_room.room_bounds.top_bound)
    {
        if(current_room.top_neighbor != NO_ROOM)
        {
            // Create the neighbor room
            current_room = Room(current_room.right_neighbor, camera.value());

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(temp_player);

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = 0;
            cam_y_offset     = -SCREEN_HEIGHT;

            return;
        }
    }

    else if(temp_player.pos().y() > current_room.room_bounds.bottom_bound)
    {
        if(current_room.bottom_neighbor != NO_ROOM)
        {
            // Create the neighbor room
            current_room = Room(current_room.right_neighbor, camera.value());

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(temp_player);

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = 0;
            cam_y_offset     = SCREEN_HEIGHT;

            return;
        }
    }
}