#include "falling_platform_wide.h"

FallingPlatformWide::FallingPlatformWide()
{

}

FallingPlatformWide::FallingPlatformWide(const FallingPlatformWide& other) : GameObject(other)
{

}

FallingPlatformWide::~FallingPlatformWide()
{

}

FallingPlatformWide& FallingPlatformWide::operator =(const FallingPlatformWide& other)
{
    return *this;
}

void FallingPlatformWide::update(const RoomBounds&                                room_bounds,
                                bn::vector<GameObject*, MAX_GAME_OBJECTS>&        game_objects,
                                const bn::regular_bg_ptr&                         bg_ptr, 
                                const bn::span<const bn::regular_bg_map_cell>&    cells,
                                const bn::regular_bg_item&                        bg_item,
                                const bn::camera_ptr&                             camera)
{

}

void FallingPlatformWide::setState(ObjectState new_state)
{

}