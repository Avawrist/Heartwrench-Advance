#include "room.h"

/////////////////
// Struct Room //
/////////////////

Room::Room()
{

}

Room::Room(RoomName room_name, bn::camera_ptr camera_ptr)
{
    load(room_name, camera_ptr);
}

Room::Room(const Room& other)
{

    for(int i = 0; i < game_objects.size(); i++)
    {
        delete game_objects.at(i);
    }
    game_objects.clear();

    for(int i = 0; i < other.game_objects.size(); i++)
    {
        switch(other.game_objects.at(i)->object_type)
        {
            case PLAYER:
                game_objects.push_back(new Player(*((Player*)(other.game_objects.at(i)))));
            break;

            case TEST_ENEMY:
                game_objects.push_back(new TestEnemy(*((TestEnemy*)(other.game_objects.at(i)))));
            break;

            default:
            break;
        }
    }

    room_bounds = other.room_bounds;

    top_neighbor    = other.top_neighbor;
    right_neighbor  = other.right_neighbor;
    bottom_neighbor = other.bottom_neighbor;
    left_neighbor   = other.left_neighbor;

}

Room::~Room()
{
    clear();
}

void Room::operator =(const Room& other)
{

    for(int i = 0; i < game_objects.size(); i++)
    {
        delete game_objects.at(i);
    }
    game_objects.clear();

    for(int i = 0; i < other.game_objects.size(); i++)
    {
        switch(other.game_objects.at(i)->object_type)
        {
            case PLAYER:
                game_objects.push_back(new Player(*((Player*)(other.game_objects.at(i)))));
            break;

            case TEST_ENEMY:
                game_objects.push_back(new TestEnemy(*((TestEnemy*)(other.game_objects.at(i)))));
            break;

            default:
            break;
        }
    }

    room_bounds = other.room_bounds;

    top_neighbor    = other.top_neighbor;
    right_neighbor  = other.right_neighbor;
    bottom_neighbor = other.bottom_neighbor;
    left_neighbor   = other.left_neighbor;
    
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

    // Free all game object pointers
    for(int32 i = game_objects.size() - 1; i >= 0; i--)
    {
        delete game_objects.at(i);
    }

    // Remove all game objects from vector
    game_objects.clear();

}

void Room::load(RoomName       room_name, 
                bn::camera_ptr camera_ptr)
{
    if(room_name == NO_ROOM) {return;}

    // Init Player FIRST. They will always be updated last.
    Player* player_ptr = new Player();
    addObject(player_ptr, camera_ptr);
    game_objects.back()->setPos(0, 0);

    GameObject* temp_ptr = NULL;

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
            for(int i = 0; i < 10; i++)
            {
                temp_ptr = new TestEnemy();
                temp_ptr->setPos(-496, 0);
                addObject(temp_ptr, camera_ptr);
            }
            temp_ptr = NULL;
            
        break;

        case ROOM_TEST_2:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TEST_1;

            room_bounds.top_bound    = -256;
            room_bounds.right_bound  =  1536;
            room_bounds.bottom_bound =  256;
            room_bounds.left_bound   =  512;

            // Init Game Objects //
    
        break;

        default:

            BN_LOG("Room creation failed - Room Name not found.");
            return;

        break;
    }

}