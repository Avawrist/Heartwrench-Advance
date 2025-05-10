#include "test_enemy.h"

TestEnemy::TestEnemy()
{
    // Init Variables //
	object_type = TEST_ENEMY;
    sprite_ptr  = bn::sprite_items::test_enemy.create_sprite(0, 0);
	sprite_ptr->set_z_order(TEST_ENEMY_Z_ORDER);
	default_palette_ptr = sprite_ptr->palette();
    animate_action_ptr = bn::create_sprite_animate_action_forever(sprite_ptr.value(),
								  								  2,
								  								  bn::sprite_items::test_enemy.tiles_item(),
								  								  0,
								  								  0);

	collider          = Collider(x(), y(), TEST_ENEMY_COLLIDER_WIDTH, TEST_ENEMY_COLLIDER_HEIGHT);
	collider_x_axis   = collider;
	collider_y_axis   = collider;
	collider_offset_x = TEST_ENEMY_COLLIDER_OFFSET_X;
	collider_offset_y = TEST_ENEMY_COLLIDER_OFFSET_Y;
    
	hitpoints = TEST_ENEMY_HITPOINTS;
}

TestEnemy::TestEnemy(const TestEnemy& other) : Enemy(other)
{

}

TestEnemy::~TestEnemy()
{

}

TestEnemy& TestEnemy::operator =(const TestEnemy& other)
{
	return *this;
}

void TestEnemy::update(const RoomBounds&                              room_bounds,
            		   bn::vector<GameObject*, MAX_GAME_OBJECTS>&     game_objects,
            		   const bn::regular_bg_ptr&                      bg_ptr, 
            		   const bn::span<const bn::regular_bg_map_cell>& cells,
            		   const bn::regular_bg_item&                     bg_item,
            		   const bn::camera_ptr&                          camera)
{

    ///////////////////
    // State Machine //
    ///////////////////

	switch(state)
    {
        case OBJECT_HITSTUN:

            updateHitstunState();

        break;

        case OBJECT_DEATH:

            udpateDeathState();

        break;

        default:
        break;
    }

    ////////////////////
    // Update Physics //
    ////////////////////

    // Apply Decay to Forces
	rigidbody.applyDecay();

	// Apply forces to enemy
	applyForces();

	///////////////////////
    // Resolve Collision //
	///////////////////////

	resolveCollision(game_objects, bg_ptr, cells, bg_item);

	/////////////////////////////////
	// Generic Object Update stuff //
	/////////////////////////////////
	
	GameObject::update(room_bounds,
					   game_objects,
					   bg_ptr,
					   cells,
					   bg_item,
					   camera);

}