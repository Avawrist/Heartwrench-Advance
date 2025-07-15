// Butano
#include "bn_sprites.h"
#include "bn_log.h"
#include "bn_profiler.h"
#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_camera_ptr.h"
#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_memory.h"
#include "bn_vector.h"

// My Libs
#include "utility.h"
#include "level.h"

int main()
{   
    bn::core::init();
    
    // Create Test Level
    Level current_level(LEVEL_TEST);

    // Game Loop
    while(true)
    {
        // Update Level
        if(current_level.cam_is_scrolling) 
        {
            current_level.updateCamera();
            current_level.updateFade();
        }
        else                             
        {
            current_level.updateAll();
        }

        // Update Global Timer
        current_level.updateGlobalTimer();
       
        bn::core::update();
    }
}