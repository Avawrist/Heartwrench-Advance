#include "room.h"

Room::Room(RoomType type)
{
    setBG(type);
}

RoomType Room::getType()
{
    return current_type;
}

void Room::setBG(RoomType type)
{
    // Set bg based on room type
    switch(type)
    {
    case DEFAULT_ROOM_1:
	bg_ptr = bn::regular_bg_items::default_room_1_bg.create_bg(0, 0);
	break;
    case DEFAULT_ROOM_2:
	bg_ptr = bn::regular_bg_items::default_room_2_bg.create_bg(0, 0);
	break;
    default:
	break;
    }
}

void Room::setCamera(const bn::camera_ptr& camera)
{
    bg_ptr->set_camera(camera);
    for(unsigned int i = 0; i < MAX_ROOM_COLLIDERS; i++)
    {
	colliders[i].setCamera(camera);
    }
}
