#include "test_enemy.h"

TestEnemy()
{

}

TestEnemy(const TestEnemy& other)
{

}

~TestEnemy()
{

}

void update(RoomBounds room_bounds,
            bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
            bn::regular_bg_ptr                         bg_ptr, 
            bn::span<const bn::regular_bg_map_cell>    cells,
            bn::regular_bg_item                        bg_item,
            bn::camera_ptr                             camera)
{
    
}