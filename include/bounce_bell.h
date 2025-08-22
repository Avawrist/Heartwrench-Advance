#ifndef BOUNCE_BELL_H
#define BOUNCE_BELL_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

///////////////////////
// Struct BounceBell //
///////////////////////

#define BOUNCE_BELL_COLLIDER_WIDTH  28
#define BOUNCE_BELL_COLLIDER_HEIGHT 36

#define BOUNCE_BELL_COLLIDER_OFFSET_X 0
#define BOUNCE_BELL_COLLIDER_OFFSET_Y 2

#define BOUNCE_BELL_HITPOINTS 0

#define BOUNCE_BELL_30_FPS false

struct BounceBell : GameObject {

    ///////////////////////
    // Struct BounceBell //
    ///////////////////////

    BounceBell();
    BounceBell(const BounceBell& other);
    ~BounceBell();

    BounceBell& operator =(const BounceBell& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

	void update(const RoomBounds& 							   room_bounds,
                bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
                const bn::camera_ptr&                          camera) override;

    void applyHit(int32 _damage, int32 knockback_x_dir, int32 knockback_y_dir) override;

    //////////////////////////////
    // State Function Overrides //
    //////////////////////////////

	void updateStateMachine(bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                            const bn::regular_bg_ptr&                      bg_ptr, 
                            const bn::span<const bn::regular_bg_map_cell>& cells,
                            const bn::regular_bg_item&                     bg_item,
                            const bn::camera_ptr&                          camera) override;

	void setState(ObjectState new_state) override;


    /////////////////////////
    // Collision Overrides //
    /////////////////////////

    // None..

};

#endif