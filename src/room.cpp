#include "room.h"

Room::Room(RoomName room_name, const bn::camera_ptr& camera)
{
    load(room_name, camera);
}

Room::~Room()
{
    clear();
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

void Room::load(RoomName room_name, 
                const bn::camera_ptr& camera)
{
    if(room_name == NO_ROOM) {return;}

    // Record current room
    current_room = room_name;

    // Init Player, Missile and Exits FIRST. They will always be updated last //
    Player* player_ptr = new Player();
    game_objects.push_back(player_ptr);
    game_objects.back()->setCamera(camera);
    game_objects.back()->setPos(player_spawn.x(), player_spawn.y());

    // Create space for Missile next.
    game_objects.push_back(new MissilePlatform(RIGHT, bn::fixed_point(0, 0)));
    game_objects.back()->setCamera(camera);

    // Exit 1
    game_objects.push_back(new Exit(NO_ROOM, bn::point(0, 0)));
    game_objects.back()->setCamera(camera);

    // Exit 2
    game_objects.push_back(new Exit(NO_ROOM, bn::point(0, 0)));
    game_objects.back()->setCamera(camera);

    // Exit 3
    game_objects.push_back(new Exit(NO_ROOM, bn::point(0, 0)));
    game_objects.back()->setCamera(camera);

    // Exit 4
    game_objects.push_back(new Exit(NO_ROOM, bn::point(0, 0)));
    game_objects.back()->setCamera(camera);

    // Initialize Variables
    switch(room_name)
    {
        case ROOM_TEST:

            // Load BG //
            backdrop_ptr = bn::regular_bg_items::test_bg.create_bg(0, 0);
            bg_ptr       = bn::regular_bg_items::test_room.create_bg(0, 0);
            bg_item      = bn::regular_bg_items::test_room;

            // Set Player spawn //
            player_spawn            = bn::point(-480, 96);
            player_ptr->respawn_pos = player_spawn; 
            game_objects.at(PLAYER_OBJECT_LIST_INDEX)->setPos(player_spawn.x(), 
                                                              player_spawn.y());

            // Init Exits //
            //delete game_objects.at(EXIT_1_OBJECT_LIST_INDEX);
            //game_objects.at(EXIT_1_OBJECT_LIST_INDEX) = new Exit(ROOM_TEST_2, bn::point(0, 0));
            //game_objects.at(EXIT_1_OBJECT_LIST_INDEX)->setCamera(camera);

            // Init Game Objects //
    
        break;

        case ROOM_TEST_2:

            // Load BG //
            backdrop_ptr = bn::regular_bg_items::test_bg.create_bg(0, 0);
            bg_ptr       = bn::regular_bg_items::test_room_2.create_bg(0, 0);
            bg_item      = bn::regular_bg_items::test_room_2;

            // Set Player spawn //
            player_spawn            = bn::point(0, -128);
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
    backdrop_ptr->set_camera(camera);
    bg_ptr->set_camera(camera);

}