#include "room.h"

Room::Room(RoomName room_name, const bn::camera_ptr& camera_ptr)
{

    // Initialize Variables
    switch(room_name)
    {
        case ROOM_TEST:

            backdrop_ptr = bn::regular_bg_items::test_bg.create_bg(0, 0);
            bg_ptr       = bn::regular_bg_items::test_room.create_bg(0, 0);
            bg_item      = bn::regular_bg_items::test_room;
    
        break;

        case ROOM_TEST_2:

            backdrop_ptr = bn::regular_bg_items::test_bg.create_bg(0, 0);
            bg_ptr       = bn::regular_bg_items::test_room_2.create_bg(0, 0);
            bg_item      = bn::regular_bg_items::test_room_2;

        break;

        default:
            BN_LOG("Room creation failed - Room Name not found.");
            return;
        break;
    }

    cells = bg_ptr->map().cells_ref().value();
    
    // Set Camera
    backdrop_ptr->set_camera(camera_ptr);
    bg_ptr->set_camera(camera_ptr);
}

Room::~Room()
{
    bg_ptr.reset();
    backdrop_ptr.reset();
    bg_item.reset();
}

int32 Room::getTileAtIndex(uint32 x, uint32 y) const
{
    // Returns -1 if input range is invalid. 
    if(x > (uint32)((bg_ptr->dimensions().width() / 8) - 1) ||
       y > (uint32)((bg_ptr->dimensions().height() / 8) - 1))
    {return -1;}

    bn::regular_bg_map_cell       cell_index = cells[bg_item->map_item().cell_index(x, y)];
    bn::regular_bg_map_cell_info  cell_info(cell_index);    

    return cell_info.tile_index();
}