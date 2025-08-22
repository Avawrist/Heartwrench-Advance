#ifndef AUTO_BOUNCE_BELL_H
#define AUTO_BOUNCE_BELL_H

// Butano

// My Libs
#include "utility.h"

// Base Class
#include "game_object.h"

///////////////////////////
// Struct AutoBounceBell //
///////////////////////////

#define AUTO_BOUNCE_BELL_COLLIDER_WIDTH  26
#define AUTO_BOUNCE_BELL_COLLIDER_HEIGHT 48

#define AUTO_BOUNCE_BELL_COLLIDER_OFFSET_X 0
#define AUTO_BOUNCE_BELL_COLLIDER_OFFSET_Y -8

#define AUTO_BOUNCE_BELL_HITPOINTS 0

#define AUTO_BOUNCE_BELL_30_FPS false

#define AUTO_BOUNCE_BELL_SPEED 1
#define AUTO_BOUNCE_BELL_ROLL_FORCE (Force(bn::fixed_point_t<12>(AUTO_BOUNCE_BELL_SPEED * (int32)x_dir, AUTO_BOUNCE_BELL_SPEED * (int32)y_dir), 1))

struct AutoBounceBell : GameObject {

    ///////////////////////////
    // Struct AutoBounceBell //
    ///////////////////////////

    XDirection roll_x_dir;
    YDirection roll_y_dir;

    AutoBounceBell();
    AutoBounceBell(const AutoBounceBell& other);
    ~AutoBounceBell();

    AutoBounceBell& operator =(const AutoBounceBell& other);

    //////////////////////////
    // GameObject Overrides //
    //////////////////////////

	void update(const RoomBounds& 							   room_bounds,
                bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
                const bn::regular_bg_ptr&                      bg_ptr, 
                const bn::span<const bn::regular_bg_map_cell>& cells,
                const bn::regular_bg_item&                     bg_item,
                const bn::camera_ptr&                          camera) override;

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

    void resolveObjectCollision(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects) override;

    void resolveTileCollision(const bn::regular_bg_ptr&                      bg_ptr, 
                              const bn::span<const bn::regular_bg_map_cell>& cells,
                              const bn::regular_bg_item&                     bg_item) override;

	void resolveHGearLeftCollision(const Collider& other_collider)   override;
	void resolveHGearMidCollision(const Collider& other_collider)    override;
	void resolveHGearRightCollision(const Collider& other_collider)  override;
	void resolveVGearTopCollision(const Collider& other_collider)    override;
	void resolveVGearMidCollision(const Collider& other_collider)    override;
	void resolveVGearBottomCollision(const Collider& other_collider) override;

};

#endif