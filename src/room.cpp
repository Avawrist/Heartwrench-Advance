#include "room.h"

Room::Room(RoomName room_name, bn::point origin_pos)
{
    camera = bn::camera_ptr::create(0, 0);
    load(room_name, origin_pos);
}

Room::~Room()
{
    clear();
}

int32 Room::addObject(GameObject* object_ptr)
{
    if(game_objects.size() >= MAX_GAME_OBJECTS) {return -1;}

    game_objects.push_back(object_ptr);
    game_objects.back()->setCamera(camera.value());
    game_objects.back()->object_id = game_objects.size() - 1;

    return game_objects.back()->object_id;
}

void Room::clear()
{
    
    // Free room pointers
    bg_ptr.reset();
    backdrop_ptr.reset();
    bg_item.reset();

    // Reset non-pointer variables
    player_spawn = bn::point(0, 0);

    // Free game object pointers
    for(int32 i = game_objects.size() - 1; i >= 0; i--)
    {delete game_objects.at(i);}

    // Remove all game objects from vector
    game_objects.clear();

}

void Room::load(RoomName room_name, bn::point origin_pos)
{
    if(room_name == NO_ROOM) {return;}

    // Record current room & pos
    current_room = room_name;
    current_pos  = origin_pos;

    // Init Player, Scythe and Exits FIRST. They will always be updated last //
    Player* player_ptr = new Player();
    addObject(player_ptr);
    game_objects.back()->setPos(origin_pos.x() + player_spawn.x(), 
                                origin_pos.y() + player_spawn.y());

    // Create space for Scythe next.
    addObject(NULL);

    // Exits
    #define EXIT_COUNT 4
    for(uint32 i = 0; i < EXIT_COUNT; i++)
    {addObject(new Exit(NO_ROOM, 
                        bn::point(0, 0), 
                        bn::point(origin_pos.x(), origin_pos.y())));}

    // Initialize Variables
    switch(room_name)
    {
        case ROOM_TEST:

            // Load BG //
            backdrop_ptr = bn::regular_bg_items::test_bg.create_bg(origin_pos.x(), origin_pos.y());
            bg_ptr       = bn::regular_bg_items::test_room.create_bg(origin_pos.x(), origin_pos.y());
            bg_item      = bn::regular_bg_items::test_room;

            // Set Player spawn //
            player_spawn            = bn::point(origin_pos.x() - 480, origin_pos.y() + 96);
            player_ptr->respawn_pos = player_spawn; 
            game_objects.at(PLAYER_OBJECT_LIST_INDEX)->setPos(player_spawn.x(), 
                                                              player_spawn.y());

            // Init Exits // 
            delete game_objects.at(EXIT_1_OBJECT_LIST_INDEX);
            game_objects.at(EXIT_1_OBJECT_LIST_INDEX) = new Exit(ROOM_TEST_2, 
                                                                 bn::point(1024, 0), 
                                                                 bn::point(origin_pos.x() + 504, 
                                                                           origin_pos.y() + 116));
            game_objects.at(EXIT_1_OBJECT_LIST_INDEX)->setCamera(camera.value());

            // Init Game Objects //
    
        break;

        case ROOM_TEST_2:

            // Load BG //
            backdrop_ptr = bn::regular_bg_items::test_bg.create_bg(origin_pos.x(), origin_pos.y());
            bg_ptr       = bn::regular_bg_items::test_room_2.create_bg(origin_pos.x(), origin_pos.y());
            bg_item      = bn::regular_bg_items::test_room_2;

            // Set Player spawn //
            player_spawn            = bn::point(origin_pos.x() + 0, origin_pos.y() - 128);
            player_ptr->respawn_pos = player_spawn;
            game_objects.at(PLAYER_OBJECT_LIST_INDEX)->setPos(player_spawn.x(), 
                                                              player_spawn.y());

            // Init Exits //

            // Init Game Objects //

        break;

        default:
            BN_LOG("Room creation failed - Room Name not found.");
            return;
        break;
    }

    cells = bg_ptr->map().cells_ref().value();
    
    // Set Camera
    backdrop_ptr->set_camera(camera.value());
    bg_ptr->set_camera(camera.value());

}

void Room::updateAndDraw()
{
    for(int32 i = game_objects.size() - 1; i >= 0; i--)
    {
        if(game_objects.data()[i] != NULL)
        {
            game_objects.data()[i]->update(game_objects, 
                                       bg_ptr.value(),
                                       cells,
                                       bg_item.value(),
                                       camera.value());
            game_objects.data()[i]->draw();
        }   
    }
}

void Room::reload()
{
    clear(); 
    load(current_room, current_pos);
}

void Room::updateCamera()
{
    #define HALF_SCREEN_WIDTH  120
    #define HALF_SCREEN_HEIGHT 80
    #define TILESET_HEIGHT     8
    int32 half_room_width_pixels  = bg_ptr.value().dimensions().width()  / 2;
    int32 half_room_height_pixels = bg_ptr.value().dimensions().height() / 2;
    int32 new_cam_x = game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos().x().integer();
    int32 new_cam_y = game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos().y().integer();
    new_cam_x = clamp(-half_room_width_pixels + HALF_SCREEN_WIDTH,  
                       half_room_width_pixels - HALF_SCREEN_WIDTH, 
                       new_cam_x);
    new_cam_y = clamp(-half_room_height_pixels + HALF_SCREEN_HEIGHT + TILESET_HEIGHT, 
                       half_room_height_pixels - HALF_SCREEN_HEIGHT, 
                       new_cam_y);
    camera.value().set_position(new_cam_x, new_cam_y);
}

void Room::checkConditions(Room* next_room_ptr)
{
    // If player died, reload the room
    if(((Player*)(game_objects.at(PLAYER_OBJECT_LIST_INDEX)))->is_dead)
    {reload();}

    // Check Exit 1
    Exit* exit_1_ptr = (Exit*)(game_objects.at(EXIT_1_OBJECT_LIST_INDEX));
    if(exit_1_ptr->is_triggered) 
    {
        // Create next room
        RoomName next_room_name = (RoomName)(exit_1_ptr->go_to_room_enum);
        if(next_room_name != NO_ROOM)
        {
            next_room_ptr = new Room(next_room_name,
                            exit_1_ptr->go_to_room_pos);

            // Transition some data from current room to next room
        }
    }

    // Check Exit 2
    Exit* exit_2_ptr = (Exit*)(game_objects.at(EXIT_2_OBJECT_LIST_INDEX));
    if(exit_2_ptr->is_triggered) 
    {
        // Create next room
        RoomName next_room_name = (RoomName)(exit_2_ptr->go_to_room_enum);
        if(next_room_name != NO_ROOM)
        {
            next_room_ptr = new Room(next_room_name,
                            exit_2_ptr->go_to_room_pos);

            // Transition some data from current room to next room
        }
    }

    // Check Exit 3
    Exit* exit_3_ptr = (Exit*)(game_objects.at(EXIT_3_OBJECT_LIST_INDEX));
    if(exit_3_ptr->is_triggered) 
    {
        // Create next room
        RoomName next_room_name = (RoomName)(exit_3_ptr->go_to_room_enum);
        if(next_room_name != NO_ROOM)
        {
            next_room_ptr = new Room(next_room_name,
                            exit_3_ptr->go_to_room_pos);

            // Transition some data from current room to next room
        }
    }

    // Check Exit 4
    Exit* exit_4_ptr = (Exit*)(game_objects.at(EXIT_4_OBJECT_LIST_INDEX));
    if(exit_4_ptr->is_triggered) 
    {
        // Create next room
        RoomName next_room_name = (RoomName)(exit_4_ptr->go_to_room_enum);
        if(next_room_name != NO_ROOM)
        {
            next_room_ptr = new Room(next_room_name,
                            exit_4_ptr->go_to_room_pos);

            // Transition some data from current room to next room
        }
    }
    
}

void Room::freeInactiveObjects()
{
    // Get an iterator to the gameobjects vector starting after the first two entries
    // if object at current iterator is NULL
    // erase the object :D 

    bn::ivector<GameObject*>::iterator current = game_objects.begin();
    bn::ivector<GameObject*>::iterator last    = game_objects.end();
    current++; // Skip player index
    current++; // Skip scythe index
    while(current != last)
    {
        if((*current)->inactive)
        {
            //delete game_objects.at(index);
            game_objects.erase(current);
            delete *current;
        }
        current++;
    }

    updateIndexes();
}

void Room::updateIndexes()
{    
    for(int32 i = game_objects.size() - 1; i >= 0; i--)
    {
        game_objects.data()[i]->object_id = i;
    }
}