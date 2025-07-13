#ifndef CANDELABRA_H
#define CANDELABRA_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "prop.h"

///////////////////////
// Struct Candelabra //
///////////////////////

#define CANDELABRA_COLLIDER_WIDTH  1
#define CANDELABRA_COLLIDER_HEIGHT 1

#define CANDELABRA_COLLIDER_OFFSET_X 0
#define CANDELABRA_COLLIDER_OFFSET_Y 0

#define CANDELABRA_30_FPS true

struct Candelabra : Prop 
{

    ///////////////////////
    // Struct Candelabra //
    ///////////////////////

    Candelabra();
    Candelabra(const Candelabra& other);
    ~Candelabra();

    Candelabra& operator =(const Candelabra& other);

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