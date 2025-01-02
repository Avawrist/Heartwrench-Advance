#include "room.h"

/////////////////
// Struct Room //
/////////////////

Room::Room(RoomName room_name, bn::camera_ptr camera_ptr)
{
    load(room_name, camera_ptr);
}

Room::~Room()
{
    clear();
}

int32 Room::addObject(GameObject* object_ptr, bn::camera_ptr camera_ptr)
{
    if(game_objects.size() >= MAX_GAME_OBJECTS) {return -1;}

    game_objects.push_back(object_ptr);
    game_objects.back()->setCamera(camera_ptr);
    game_objects.back()->object_id = game_objects.size() - 1;

    return game_objects.back()->object_id;
}

void Room::clear()
{

    // Free game object pointers
    for(int32 i = game_objects.size() - 1; i >= 0; i--)
    {delete game_objects.at(i);}

    // Remove all game objects from vector
    game_objects.clear();

}

void Room::load(RoomName room_name, bn::camera_ptr camera_ptr)
{

    if(room_name == NO_ROOM) {return;}

    // Init Player FIRST. They will always be updated last.
    Player* player_ptr = new Player();
    addObject(player_ptr, camera_ptr);
    game_objects.back()->setPos(0, 0);

    // Initialize Objects
    switch(room_name)
    {
        case ROOM_TEST_1:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TEST_2;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = NO_ROOM;

            room_bounds.top_bound    = -256;
            room_bounds.right_bound  =  512;
            room_bounds.bottom_bound =  256;
            room_bounds.left_bound   = -512;

            // Init Game Objects //
    
        break;

        case ROOM_TEST_2:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TEST_1;

            room_bounds.right_bound  =  1536;
            room_bounds.left_bound   =  512;
            room_bounds.top_bound    = -256;
            room_bounds.bottom_bound =  256;

            // Init Game Objects //
    
        break;

        default:

            BN_LOG("Room creation failed - Room Name not found.");
            return;

        break;
    }

}
