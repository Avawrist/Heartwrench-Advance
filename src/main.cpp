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

    // Initialize Level
    Level level(LEVEL_NAME_CARD);

    // Game Loop
    while(true)
    {
        // Update Level
        level.update();

        // Update Butano
        bn::core::update();

        BN_LOG(bn::core::last_missed_frames());
    }
}