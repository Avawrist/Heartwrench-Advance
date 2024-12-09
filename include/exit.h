#ifndef EXIT_H
#define EXIT_H

// Butano
#include "bn_math.h"

// My Libs
#include "utility.h"

// Assets
#include "bn_sprite_items_exit.h"

// Base Class
#include "game_object.h"

//////////
// Exit //
//////////

#define EXIT_COLLIDER_WIDTH  8
#define EXIT_COLLIDER_HEIGHT 32

struct Exit : GameObject {

	int32 go_to_room_enum;
    bool  is_triggered;

    Exit(int32 _go_to_room_enum, bn::point origin);
    ~Exit();

    void update(bn::vector<GameObject*, MAX_GAME_OBJECTS>& game_objects,
				bn::regular_bg_ptr                         bg_ptr, 
                bn::span<const bn::regular_bg_map_cell>    cells,
                bn::regular_bg_item                        bg_item) override;

};

#endif