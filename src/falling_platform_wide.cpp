#include "falling_platform_wide.h"

////////////////////////////////
// Struct FallingPlatformWide //
////////////////////////////////

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

//////////////////////////////
// State Function Overrides //
//////////////////////////////

void FallingPlatformWide::setState(ObjectState new_state)
{

}

/////////////////////////
// Collision Overrides //
/////////////////////////