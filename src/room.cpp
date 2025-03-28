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

    // Copy unloaded objects
    unloaded_objects = other.unloaded_objects;

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

    // Copy unloaded objects
    unloaded_objects = other.unloaded_objects;

    room_bounds = other.room_bounds;

    top_neighbor    = other.top_neighbor;
    right_neighbor  = other.right_neighbor;
    bottom_neighbor = other.bottom_neighbor;
    left_neighbor   = other.left_neighbor;
    
}

int32 Room::addObject(GameObject* object_ptr, const bn::camera_ptr& camera_ptr)
{
    if(game_objects.size() >= MAX_GAME_OBJECTS) {return -1;}

    game_objects.push_back(object_ptr);
    game_objects.back()->setCamera(camera_ptr);
    game_objects.back()->object_id = game_objects.size() - 1;

    return game_objects.back()->object_id;
}

int32 Room::addObject(const UnloadedObject& object, const bn::camera_ptr& camera_ptr)
{
    if(game_objects.size() >= MAX_GAME_OBJECTS) {return -1;}

    GameObject* temp_object_ptr = NULL;

    // Allocate object based on type
    // NOTE: All object types should be represented here. When adding object types 
    //       this list must be updated.
    switch(object.object_type)
    {
        case PLAYER:
            temp_object_ptr = new Player();
        break;

        case TEST_ENEMY:
            temp_object_ptr = new TestEnemy();
        break;

        case DEVIL_PLATFORM:
        case ANGEL_PLATFORM:
        case SCYTHE_PLATFORM:
        case HITBOX_SCYTHE_1:
        case HITBOX_SCYTHE_2:
        case HITBOX_SCYTHE_3:
            BN_LOG("Loading object of type ", object.object_type);
            BN_LOG("is not supported by function Room::addObject");
        break;

        case NO_TYPE:
        default:
            BN_LOG("Failed to load object - No type provided.");
            return -1;
        break;
    }
	
    game_objects.push_back(temp_object_ptr);
    game_objects.back()->setCamera(camera_ptr);
    game_objects.back()->setPos(object.room_pos);
    game_objects.back()->object_id = game_objects.size() - 1;
    game_objects.back()->is_persistent = object.is_persistent;

    return game_objects.back()->object_id;
}

int32 Room::addUnloadedObject(const UnloadedObject& new_object, bool is_persistent)
{
    if(unloaded_objects.size() >= MAX_UNLOADED_OBJECTS) {return 0;}

    unloaded_objects.push_back(new_object);
    unloaded_objects.back().is_persistent = is_persistent;

    return 1;
}

int32 Room::findUnloadedObjectIndex(int32 object_id)
{
    for(int32 i = 0; i < unloaded_objects.size(); i++)
    {
        if(unloaded_objects.at(i).loaded_instance_id == object_id)
        {
            return i;
        }
    }

    return -1;
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

void Room::load(RoomName              room_name, 
                const bn::camera_ptr& camera_ptr)
{
    if(room_name == NO_ROOM) {return;}

    // Init Player FIRST. Player will always be updated last.
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

            // Add UnloadedObjects //
            addUnloadedObject(UnloadedObject(bn::point(344, 56), TEST_ENEMY),  false);
            
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

            // Add UnloadedObjects //
    
        break;

        default:

            BN_LOG("Room creation failed - RoomName not found.");
            return;

        break;
    }

}

void Room::monitorUnloadedObjects(const bn::camera_ptr& camera_ptr)
{
    if(game_objects.at(PLAYER_OBJECT_LIST_INDEX) == NULL) {return;}

    bn::fixed_point camera_center = game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos();

    // Look at all unloaded objects in the list. If the position is within the defined
    // camera boundaries, actually load the object.
    for(int i = 0; i < unloaded_objects.size(); i++)
    {
        if(unloaded_objects.at(i).room_pos.x() >= camera_center.x() - LOAD_RANGE_HALF_W &&
           unloaded_objects.at(i).room_pos.x() <= camera_center.x() + LOAD_RANGE_HALF_W &&
           unloaded_objects.at(i).room_pos.y() >= camera_center.y() - LOAD_RANGE_HALF_H &&
           unloaded_objects.at(i).room_pos.y() <= camera_center.y() + LOAD_RANGE_HALF_H)
        {
            // If there is not already a loaded instance, load one in:
            if(unloaded_objects.at(i).loaded_instance_id == UNLOADED_OBJECT_STATE_UNLOADED)
            {
                unloaded_objects.at(i).loaded_instance_id = addObject(unloaded_objects.at(i),
                                                                      camera_ptr);
            }
        }
    }
}