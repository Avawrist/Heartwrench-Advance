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

    // Free all game object pointers
    for(int32 i = game_objects.size() - 1; i >= 0; i--)
    {
        delete game_objects.at(i);
    }

    // Remove all game objects from vector
    game_objects.clear();

    // Free tile colliders array
    // Needs both dimensions initialized
    for(int x = 0; x < ROOM_MAX_WIDTH; x++)
    {
        delete tile_colliders[x][0];
    }

}

void Room::load(RoomName       room_name, 
                bn::camera_ptr camera_ptr)
{

    if(room_name == NO_ROOM) {return;}

    // Init Player FIRST. They will always be updated last.
    Player* player_ptr = new Player();
    addObject(player_ptr, camera_ptr);
    game_objects.back()->setPos(1000, 0);

    //GameObject* temp_ptr = NULL;

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
            /*
            for(int i = 0; i < 0; i++)
            {
                temp_ptr = new TestEnemy();
                temp_ptr->setPos(0, 0);
                addObject(temp_ptr, camera_ptr);
            }
            temp_ptr = NULL;
            */
    
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

void Room::populateTileColliders(bn::regular_bg_ptr                      bg_ptr, 
                                 bn::span<const bn::regular_bg_map_cell> cells,
                                 bn::regular_bg_item                     bg_item)
{
    
    // This function is called by the Level that owns the room, the Level object is
    // responsible for populating the tile colliders of each room on room load and
    // on transition.

    // populateTileColliders will look at the global tile indexes and populate two arrays with data:
    // 1. The first array will contain the room's tile types in a locally indexed array
    // 2. The second array will contain the room's tile colliders in a locally indexed array

    // Initialize Tile Colliders 
    for(int x = 0; x < ROOM_MAX_WIDTH; x++)
    {
        tile_colliders[x][0] = new Collider(0, 0, 0, 0);
    }

    /*
    for(int world_x = room_bounds.left_bound; world_x < room_bounds.right_bound; world_x += 8)
    {
        for(int world_y = room_bounds.top_bound; world_y < room_bounds.bottom_bound; world_y += 8)
        {
            uint32 check_index_x = (world_x / TILE_WIDTH)  - 1;
            uint32 check_index_y = (world_y / TILE_HEIGHT) - 1;

            uint32 tile_index = getTileAtBGIndex(check_index_x, check_index_y, bg_ptr, cells, bg_item);

            switch(tile_index)
            {
                case HARD_BLOCK_INDEX:
                case SOFT_BLOCK_INDEX:
                case UP_SPIKE_BLOCK_INDEX:
                case DOWN_SPIKE_BLOCK_INDEX:
                case LEFT_SPIKE_BLOCK_INDEX:
                case RIGHT_SPIKE_BLOCK_INDEX:

                    tile_colliders[check_index_x][check_index_y] = new Collider(world_x + WORLD_X_OFFSET, 
                                                                                world_y + WORLD_Y_OFFSET,
                                                                                TILE_WIDTH,
                                                                                TILE_HEIGHT);

                break;

                case LEFT_SHALLOW_SLOPE_1_INDEX:
                break;

                case LEFT_SHALLOW_SLOPE_2_INDEX:
                break;

                case LEFT_SHALLOW_SLOPE_3_INDEX:
                break;

                case LEFT_SHALLOW_SLOPE_4_INDEX:
                break;

                case LEFT_STEEP_SLOPE_1_INDEX:
                break;

                case LEFT_STEEP_SLOPE_2_INDEX:
                break;

                case RIGHT_SHALLOW_SLOPE_1_INDEX:
                break;

                case RIGHT_SHALLOW_SLOPE_2_INDEX:
                break;

                case RIGHT_SHALLOW_SLOPE_3_INDEX:
                break;

                case RIGHT_SHALLOW_SLOPE_4_INDEX:
                break;

                case RIGHT_STEEP_SLOPE_1_INDEX:
                break;

                case RIGHT_STEEP_SLOPE_2_INDEX:
                break;

                case ONEWAY_BLOCK_INDEX:
                break;

                default:
                break;
            };
        }
    }
    */

}
