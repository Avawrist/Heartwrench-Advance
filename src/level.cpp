#include "level.h"

Level::Level()
{
    
}

Level::Level(LevelName level_name)
{
    load(level_name);
}

Level::Level(const Level& other)
{
    current_room = other.current_room;
    
    camera         = other.camera;
    main_bg_ptr    = other.main_bg_ptr;
    backdrop_ptr   = other.backdrop_ptr;
    painted_bg_ptr = other.painted_bg_ptr;
    object_bg_ptr  = other.object_bg_ptr;
    bg_item        = other.bg_item;
    cells          = other.cells;
    object_bg_item = other.object_bg_item;
    object_cells   = other.object_cells;

    tile_width  = other.tile_width;
    tile_height = other.tile_height;

    current_level_name = other.current_level_name;
    player_spawn       = other.player_spawn;
    cam_is_scrolling   = other.cam_is_scrolling;
    cam_x_offset       = other.cam_x_offset;
    cam_y_offset       = other.cam_y_offset;

    random_engine = other.random_engine;
}

Level::~Level()
{
    clear();
}

void Level::operator =(const Level& other)
{
    current_room = other.current_room;

    camera         = other.camera;
    main_bg_ptr    = other.main_bg_ptr;
    backdrop_ptr   = other.backdrop_ptr;
    painted_bg_ptr = other.painted_bg_ptr;
    object_bg_ptr  = other.object_bg_ptr;
    bg_item        = other.bg_item;
    cells          = other.cells;
    object_bg_item = other.object_bg_item;
    object_cells   = other.object_cells;

    tile_width  = other.tile_width;
    tile_height = other.tile_height;

    current_level_name = other.current_level_name;
    player_spawn       = other.player_spawn;
    cam_is_scrolling   = other.cam_is_scrolling;
    cam_x_offset       = other.cam_x_offset;
    cam_y_offset       = other.cam_y_offset;

    random_engine = other.random_engine;
}

void Level::clear()
{
    
    // Free level pointers
    camera.reset();
    main_bg_ptr.reset();
    backdrop_ptr.reset();
    painted_bg_ptr.reset();
    object_bg_ptr.reset();

    bg_item.reset();
    object_bg_item.reset();

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

    // Store room name before constructing
    RoomName temp_room_name = NO_ROOM;

    // Initialize Variables
    switch(level_name)
    {
        case LEVEL_TEST:
            
            // Load BGs //
            backdrop_ptr   = bn::regular_bg_items::test_bg.create_bg(0, 0);
            painted_bg_ptr = bn::regular_bg_items::test_painted_bg.create_bg(0, 0);

            main_bg_ptr    = bn::regular_bg_items::test_level.create_bg(0, 0);
            bg_item        = bn::regular_bg_items::test_level;

            object_bg_ptr  = bn::regular_bg_items::test_object_bg.create_bg(0, 0);
            object_bg_item = bn::regular_bg_items::test_object_bg;

            // Update cells
            cells        = main_bg_ptr->map().cells_ref().value();
            object_cells = object_bg_ptr->map().cells_ref().value();

            // Set Room //
            temp_room_name = ROOM_TEST_1;

        break;

        default:

            BN_LOG("Level creation failed - Level Name not found.");
            return;

        break;
    }

    current_room = Room(temp_room_name, 
                        camera.value(), 
                        object_bg_ptr.value(), 
                        object_bg_item.value(), 
                        object_cells);
    
    // Set draw priority for BGs
    painted_bg_ptr->set_z_order(PAINTED_BG_ORDER);
    backdrop_ptr->set_z_order(BACKDROP_ORDER);
    main_bg_ptr->set_z_order(MAIN_BG_ORDER);
    object_bg_ptr->set_z_order(OBJECT_BG_ORDER);

    // Set Camera
    backdrop_ptr->set_camera(camera.value());
    main_bg_ptr->set_camera(camera.value());
    painted_bg_ptr->set_camera(camera.value());
    object_bg_ptr->set_camera(camera.value());
    object_bg_ptr->set_visible(false);

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

    // Update & draw all objects
    for(int32 i = current_room.game_objects.size() - 1; i >= 0; i--)
    {
        if(current_room.game_objects.data()[i] != NULL)
        {
            current_room.game_objects.data()[i]->update(current_room.room_bounds,
                                                        current_room.game_objects,
                                                        main_bg_ptr.value(),
                                                        cells,
                                                        bg_item.value(),
                                                        camera.value());
            current_room.game_objects.data()[i]->draw();
        }   
    } 

    // Load unloaded objects if needed - Is it appropriate to have this here? 
    current_room.monitorUnloadedObjects(camera.value());

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

        // Apply screenshake
        if(screenshake_frames > 0)
        {
            int32 x_shake_offset = random_engine.get_int(MIN_X_SHAKE_RANGE, MAX_X_SHAKE_RANGE);
            int32 y_shake_offset = random_engine.get_int(MIN_Y_SHAKE_RANGE, MAX_Y_SHAKE_RANGE);

            camera.value().set_position(camera.value().x() + (x_shake_offset * screenshake_severity), 
                                        camera.value().y() + (y_shake_offset * screenshake_severity));
        }

    }

    // Update screenshake frame counter
    screenshake_frames--;
    if(screenshake_frames <= 0) 
    {
        screenshake_frames   = 0;
        screenshake_severity = NO_SHAKE;
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

void Level::freeObjects()
{
    #define INDEX_AFTER_PLAYER 1

    int32 last_index = current_room.game_objects.size() - 1;
    for(int32 i = INDEX_AFTER_PLAYER; i <= last_index; i++)
    {
        if(current_room.game_objects.at(i)->is_dead)
        {
            // 1. Update the UnloadedObject with the Unloaded ID:
            int32 object_id = current_room.game_objects.at(i)->object_id;
            int32 unloaded_index = current_room.findUnloadedObjectIndex(object_id);
            if(unloaded_index > -1)
            {current_room.unloaded_objects.at(unloaded_index).loaded_instance_id = UNLOADED_OBJECT_STATE_DEAD;}

            // 2. Remove the dead object:
            delete current_room.game_objects.at(i);
            current_room.game_objects.at(i) = current_room.game_objects.at(last_index);
            current_room.game_objects.pop_back();
            last_index--;
        }

        else if(current_room.game_objects.at(i)->is_inactive)
        {
            // 1. Update the UnloadedObject with the Unloaded ID:
            int32 object_id = current_room.game_objects.at(i)->object_id;
            int32 unloaded_index = current_room.findUnloadedObjectIndex(object_id);
            if(unloaded_index > -1)
            {current_room.unloaded_objects.at(unloaded_index).loaded_instance_id = UNLOADED_OBJECT_STATE_UNLOADED;}

            // 2. Remove the inactive object:
            delete current_room.game_objects.at(i);
            current_room.game_objects.at(i) = current_room.game_objects.at(last_index);
            current_room.game_objects.pop_back();
            last_index--;
        }
    }

    updateIndexes();
}

void Level::updateIndexes()
{    
    for(int32 i = current_room.game_objects.size() - 1; i >= 0; i--)
    {
        // Search for the unloaded object by index, if found, update the ID:
        int32 object_id = current_room.game_objects.at(i)->object_id;
        int32 unloaded_index = current_room.findUnloadedObjectIndex(object_id);
        if(unloaded_index > -1)
        {current_room.unloaded_objects.at(unloaded_index).loaded_instance_id = i;}

        // Update the loaded object's ID:
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
            current_room = Room(current_room.right_neighbor, 
                                camera.value(),
                                object_bg_ptr.value(), 
                                object_bg_item.value(), 
                                object_cells);

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
            current_room = Room(current_room.left_neighbor, 
                                camera.value(),
                                object_bg_ptr.value(), 
                                object_bg_item.value(), 
                                object_cells);

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
            current_room = Room(current_room.top_neighbor, 
                                camera.value(),
                                object_bg_ptr.value(), 
                                object_bg_item.value(), 
                                object_cells);

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
            current_room = Room(current_room.bottom_neighbor, 
                                camera.value(),
                                object_bg_ptr.value(), 
                                object_bg_item.value(),
                                object_cells);

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