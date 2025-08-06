#include "room_bounds.h"

///////////////////////
// Struct RoomBounds //
///////////////////////

RoomBounds::RoomBounds()
{   
    top_bound    = 0;
    right_bound  = 0;
    bottom_bound = 0;
    left_bound   = 0;
}

RoomBounds::RoomBounds(int32 _top_bound, 
                       int32 _right_bound, 
                       int32 _bottom_bound, 
                       int32 _left_bound)
{
    top_bound    = _top_bound;
    right_bound  = _right_bound;
    bottom_bound = _bottom_bound;
    left_bound   = _left_bound;
}

RoomBounds::RoomBounds(const RoomBounds& other)
{
    top_bound    = other.top_bound;
    right_bound  = other.right_bound;
    bottom_bound = other.bottom_bound;
    left_bound   = other.left_bound;
}

RoomBounds& RoomBounds::operator =(const RoomBounds& other)
{
    top_bound    = other.top_bound;
    right_bound  = other.right_bound;
    bottom_bound = other.bottom_bound;
    left_bound   = other.left_bound;

    return *this;
}

bn::point RoomBounds::center()
{
    return(bn::point(left_bound + ((right_bound - left_bound) * 0.5), top_bound + ((bottom_bound - top_bound) * 0.5)));
}

void RoomBounds::setTopBoundAC(int32 value)
{
    top_bound = value - (LEVEL_HEIGHT / 2);
}

void RoomBounds::setBottomBoundAC(int32 value)
{
    bottom_bound = value - (LEVEL_HEIGHT / 2);
}

void RoomBounds::setLeftBoundAC(int32 value)
{
    left_bound = value - (LEVEL_WIDTH / 2);
}

void RoomBounds::setRightBoundAC(int32 value)
{
    right_bound = value - (LEVEL_WIDTH / 2);
}
