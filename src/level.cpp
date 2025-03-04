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
    
    // Free Room
    delete current_room_ptr;
    current_room_ptr = NULL;

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
    cam_x_offset    = 0;
    cam_y_offset    = 0;

    // Record current level & pos
    current_level = level_name;

    // Initialize Variables
    switch(level_name)
    {
        case LEVEL_TEST:

            // Set Room Ptr //
            current_room_ptr = new Room(ROOM_TEST_1, camera.value());

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

    // Initialize Room Colliders
    current_room_ptr->populateTileColliders(bg_ptr.value(), cells, bg_item.value());

    BN_LOG("=== Level loaded ===");
    BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());
    
}

void Level::reload()
{
    clear(); 
    load(current_level);
}

void Level::updateAndDraw()
{
    if(current_room_ptr == NULL) {return;}

    for(int32 i = current_room_ptr->game_objects.size() - 1; i >= 0; i--)
    {
        if(current_room_ptr->game_objects.data()[i] != NULL)
        {
            current_room_ptr->game_objects.data()[i]->update(current_room_ptr->room_bounds,
                                                             current_room_ptr->game_objects,
                                                             bg_ptr.value(),
                                                             cells,
                                                             bg_item.value(),
                                                             camera.value());
            current_room_ptr->game_objects.data()[i]->draw();
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
        int32 new_cam_x = current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos().x().integer();
        int32 new_cam_y = current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos().y().integer();
        new_cam_x = clamp(current_room_ptr->room_bounds.left_bound  + HALF_SCREEN_WIDTH,  
                          current_room_ptr->room_bounds.right_bound - HALF_SCREEN_WIDTH, 
                          new_cam_x);
        new_cam_y = clamp(current_room_ptr->room_bounds.top_bound    + HALF_SCREEN_HEIGHT, 
                          current_room_ptr->room_bounds.bottom_bound - HALF_SCREEN_HEIGHT,
                          new_cam_y);
        camera.value().set_position(new_cam_x, new_cam_y);

    }

}

void Level::reloadOnDeath()
{
    if(current_room_ptr == NULL) {return;}

    // If player died, reload the level
    if(((Player*)(current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX)))->is_dead)
    {
        reload();
    }
}

void Level::freeInactiveObjects()
{
    // Get an iterator to the gameobjects vector starting after the first two entries
    // if object at current iterator is NULL
    // erase the object :D 

    if(current_room_ptr == NULL) {return;}

    bn::ivector<GameObject*>::iterator current = current_room_ptr->game_objects.begin();
    bn::ivector<GameObject*>::iterator last    = current_room_ptr->game_objects.end();
    current++; // Skip player index
    while(current != last)
    {
        if((*current)->inactive)
        {
            // Erase game object
            current_room_ptr->game_objects.erase(current);
            delete *current;
        }
        current++;
    }

    updateIndexes();
}

void Level::updateIndexes()
{    
    if(current_room_ptr == NULL) {return;}

    for(int32 i = current_room_ptr->game_objects.size() - 1; i >= 0; i--)
    {
        current_room_ptr->game_objects.data()[i]->object_id = i;
    }
}

void Level::transitionRoom()
{
    #define SCREEN_WIDTH  240
    #define SCREEN_HEIGHT 160

    if(current_room_ptr == NULL) {return;}

    Player* player_ptr = (Player*)current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX);
    bn::fixed_point player_pos = player_ptr->pos();
    
    if(player_pos.x() > current_room_ptr->room_bounds.right_bound)
    {
        if(current_room_ptr->right_neighbor != NO_ROOM)
        {
            // Store the current room in a temp ptr
            Room* temp_room_ptr = current_room_ptr;

            // Create the neighbor room
            current_room_ptr = new Room(temp_room_ptr->right_neighbor, camera.value());

            BN_LOG("=== New room loaded ===");
            BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(*player_ptr);

            // Delete the old room
            delete temp_room_ptr;

            BN_LOG("=== Past room cleared ===");
            BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());

            // Populate tile colliders in new room
            current_room_ptr->populateTileColliders(bg_ptr.value(), cells, bg_item.value());

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = SCREEN_WIDTH;
            cam_y_offset     = 0;

            return;
        }
    }

    if(player_pos.x() < current_room_ptr->room_bounds.left_bound)
    {
        if(current_room_ptr->left_neighbor != NO_ROOM)
        {
            // Store the current room in a temp ptr
            Room* temp_room_ptr = current_room_ptr;

            // Create the neighbor room
            current_room_ptr = new Room(temp_room_ptr->left_neighbor, camera.value());

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(*player_ptr);

            // Delete the old room
            delete temp_room_ptr;

            // Populate tile colliders in new room
            current_room_ptr->populateTileColliders(bg_ptr.value(), cells, bg_item.value());

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = -SCREEN_WIDTH;
            cam_y_offset     = 0;

            return;
        }
    }

    if(player_pos.y() < current_room_ptr->room_bounds.top_bound)
    {
        if(current_room_ptr->top_neighbor != NO_ROOM)
        {
            // Store the current room in a temp ptr
            Room* temp_room_ptr = current_room_ptr;

            // Create the neighbor room
            current_room_ptr = new Room(temp_room_ptr->top_neighbor, camera.value());

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(*player_ptr);

            // Delete the old room
            delete temp_room_ptr;

            // Populate tile colliders in new room
            current_room_ptr->populateTileColliders(bg_ptr.value(), cells, bg_item.value());

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = 0;
            cam_y_offset     = -SCREEN_HEIGHT;

            return;
        }
    }

    if(player_pos.y() > current_room_ptr->room_bounds.bottom_bound)
    {
        if(current_room_ptr->bottom_neighbor != NO_ROOM)
        {
            // Store the current room in a temp ptr
            Room* temp_room_ptr = current_room_ptr;

            // Create the neighbor room
            current_room_ptr = new Room(temp_room_ptr->bottom_neighbor, camera.value());

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room_ptr->game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(*player_ptr);

            // Delete the old room
            delete temp_room_ptr;

            // Populate tile colliders in new room
            current_room_ptr->populateTileColliders(bg_ptr.value(), cells, bg_item.value());

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = 0;
            cam_y_offset     = SCREEN_HEIGHT;

            return;
        }
    }
}