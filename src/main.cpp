// Butano
#include "bn_log.h"
#include "bn_profiler.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_camera_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"

// Common
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

// My Libs
#include "utility.h"
#include "game_object.h"
#include "player.h"
#include "block.h"

int main()
{   
    bn::core::init();
    
    // Camera
    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
    
    // Game Objects test
    #define MAX_OBJECTS 17
    GameObject *game_objects[MAX_OBJECTS];
    game_objects[0] = new Player();
    game_objects[0]->setCamera(camera);

    game_objects[1] = new Block();
    game_objects[1]->setPos(-96, 64);
    game_objects[1]->setCamera(camera);

    game_objects[2] = new Block();
    game_objects[2]->setPos(-64, 64);
    game_objects[2]->setCamera(camera);

    game_objects[3] = new Block();
    game_objects[3]->setPos(-32, 64);
    game_objects[3]->setCamera(camera);
    
    game_objects[4] = new Block();
    game_objects[4]->setPos(0, 64);
    game_objects[4]->setCamera(camera);

    game_objects[5] = new Block();
    game_objects[5]->setPos(32, 64);
    game_objects[5]->setCamera(camera);

    game_objects[6] = new Block();
    game_objects[6]->setPos(64, 64);
    game_objects[6]->setCamera(camera);

    game_objects[7] = new Block();
    game_objects[7]->setPos(96, 64);
    game_objects[7]->setCamera(camera);

    game_objects[8] = new Block();
    game_objects[8]->setPos(128, 64);
    game_objects[8]->setCamera(camera);

    game_objects[9] = new Block();
    game_objects[9]->setPos(128, 32);
    game_objects[9]->setCamera(camera);

    game_objects[10] = new Block();
    game_objects[10]->setPos(128, 0);
    game_objects[10]->setCamera(camera);

    game_objects[11] = new Block();
    game_objects[11]->setPos(128, -32);
    game_objects[11]->setCamera(camera);

    game_objects[12] = new Block();
    game_objects[12]->setPos(128, -64);
    game_objects[12]->setCamera(camera);

    game_objects[13] = new Block();
    game_objects[13]->setPos(64, -16);
    game_objects[13]->setCamera(camera);

    game_objects[14] = new Block();
    game_objects[14]->setPos(-96, 32);
    game_objects[14]->setCamera(camera);

    game_objects[15] = new Block();
    game_objects[15]->setPos(-96, 0);
    game_objects[15]->setCamera(camera);

    game_objects[16] = new Block();
    game_objects[16]->setPos(-96, -32);
    game_objects[16]->setCamera(camera);
    
    
    // Game Loop
    while(true)
    {
        // Update Game Objects
	for(uint8 i = 0; i < MAX_OBJECTS; i++)
	{
	    game_objects[i]->update(game_objects, MAX_OBJECTS);
	    game_objects[i]->draw();
	}

	// Update Camera
	camera.set_position(game_objects[0]->pos());

	// Update Core
	bn::core::update();
	//BN_LOG(bn::core::last_missed_frames());
    }
}
