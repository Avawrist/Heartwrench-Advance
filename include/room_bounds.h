#ifndef ROOM_BOUNDS_H
#define ROOM_BOUNDS_H

// My Libs
#include "utility.h"

////////////////
// RoomBounds //
////////////////

struct RoomBounds
{
    int32 top_bound;
    int32 right_bound;
    int32 bottom_bound;
    int32 left_bound;

    RoomBounds();
    RoomBounds(int32 _top_bound, 
               int32 _right_bound, 
               int32 _bottom_bound, 
               int32 _left_bound);
    RoomBounds(const RoomBounds& other);

    RoomBounds& operator =(const RoomBounds& other);
};

#endif