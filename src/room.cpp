#include "room.h"

Room::Room(RoomName room_name, const bn::camera_ptr& camera_ptr, bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects)
{

    // Initialize Variables
    switch(room_name)
    {
        case ROOM_TEST:

            backdrop_ptr = bn::regular_bg_items::test_bg.create_bg(0, 0);
            bg_ptr       = bn::regular_bg_items::test_room.create_bg(0, 0);
    
        break;
        default:
            BN_LOG("Room creation failed - Room Name not found.");
            return;
        break;
    }

    tile_width  = bg_ptr->dimensions().width()  / SINGLE_TILE_WIDTH;
    tile_height = bg_ptr->dimensions().height() / SINGLE_TILE_WIDTH;

    // Set Camera
    backdrop_ptr->set_camera(camera_ptr);
    bg_ptr->set_camera(camera_ptr);
    bn::span<const bn::regular_bg_map_cell> cells = bg_ptr->map().cells_ref().value();

    // Create Game Objects from BG
    for(uint32 y = 0; y < tile_height; y++)
    {
        for(uint32 x = 0; x < tile_width; x++)
        {
            bn::regular_bg_map_cell       cell_index = cells[bn::regular_bg_items::test_room.map_item().cell_index(x, y)];
            bn::regular_bg_map_cell_info  cell_info(cell_index);

            uint32 final_block_width  = 0;
            uint32 final_block_height = 8;

            uint32 i = x;

            switch(cell_info.tile_index())
            {
                case BLOCK_INDEX:

                    while(cell_info.tile_index() == BLOCK_INDEX && i < tile_width)
                    {
                        // Add block width
                        final_block_width += SINGLE_TILE_WIDTH;

                        // Increment i
                        i++;

                        if(i < tile_width)
                        {
                            // Update cell index and cell info for next pass
                            cell_index = cells[bn::regular_bg_items::test_room.map_item().cell_index(i, y)];
                            cell_info  = bn::regular_bg_map_cell_info(cell_index);
                        }
                    }

                    if(final_block_width > 0)
                    {
                        // Create block with the width determined above.
                        int32 converted_x = ((x - (tile_width / 2)) * SINGLE_TILE_WIDTH) + (final_block_width / 2);
                        int32 converted_y = ((y - (tile_height / 2)) * SINGLE_TILE_WIDTH) + (final_block_height / 2);

                        game_objects.push_back(new Block(final_block_width));
                        game_objects.back()->setCamera(camera_ptr);
                        game_objects.back()->setPos(converted_x, converted_y);
                    }

                    // Make sure next check starts after the offset (if any).
                    x = i - 1;

                break;

                case ONEWAYBLOCK_INDEX:
                    
                    while(cell_info.tile_index() == ONEWAYBLOCK_INDEX && i < tile_width)
                    {
                        // Add block width
                        final_block_width += SINGLE_TILE_WIDTH;

                        // Increment i
                        i++;

                        if(i < tile_width)
                        {
                            // Update cell index and cell info for next pass
                            cell_index = cells[bn::regular_bg_items::test_room.map_item().cell_index(i, y)];
                            cell_info  = bn::regular_bg_map_cell_info(cell_index);
                        }
                    }

                    if(final_block_width > 0)
                    {
                        // Create block with the width determined above.
                        int32 converted_x = ((x - (tile_width / 2)) * SINGLE_TILE_WIDTH) + (final_block_width / 2);
                        int32 converted_y = ((y - (tile_height / 2)) * SINGLE_TILE_WIDTH) + (final_block_height / 2);

                        game_objects.push_back(new OneWayBlock(final_block_width));
                        game_objects.back()->setCamera(camera_ptr);
                        game_objects.back()->setPos(converted_x, converted_y);
                    }
                    
                    // Make sure next check starts after the offset (if any).
                    x = i - 1;
                    
                break;

                default:
                break;
            }
        }
    }
}

Room::~Room()
{
    bg_ptr.reset();
    backdrop_ptr.reset();
}