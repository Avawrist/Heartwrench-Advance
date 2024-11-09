#include "room.h"

Room::Room(RoomName room_name, const camera_ptr& camera_ptr, bn::vector<GameObject*>& game_objects)
{
    switch(room_name)
    {
        case ROOM_TEST:

            backdrop_ptr = bn::regular_bg_items::test_bg.create_bg(0, 0);
            bg_ptr = bn::regular_bg_items::test_room.create_bg(0, 0);
    
        break;
        default:
        BN_LOG("Room creation failed - Room Name not found.");
        break;
    }

    backdrop_ptr.set_camera(camera_ptr);
    bg_ptr.set_camera(camera_ptr);
    width  = ;
    height = ;
}