#ifndef NEW_OBJECT_H
#define NEW_OBJECT_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

//////////////////////
// Struct NewObject //
//////////////////////

#define NEW_OBJECT_COLLIDER_WIDTH  32
#define NEW_OBJECT_COLLIDER_HEIGHT 1

struct NewObject : GameObject {

    //////////////////////
    // Struct NewObject //
    //////////////////////

    NewObject();
    NewObject(const NewObject& other);
    ~NewObject();

    NewObject& operator =(const NewObject& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

    // None..

    //////////////////////////////
    // State Function Overrides //
    //////////////////////////////

    // None..

    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    // None..

};

#endif