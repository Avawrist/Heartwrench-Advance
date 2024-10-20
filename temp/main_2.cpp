// Butano
#include "bn_core.h"
#include "bn_profiler.h"
#include "bn_log.h"
#include "bn_format.h"
#include "bn_sprite_text_generator.h"
#include "bn_keypad.h"
#include "bn_camera_ptr.h"
#include "bn_point.h"

// TEST
#include "bn_span.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_cell_info.h"

// Common
#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

// Game Files
#include "room.h"
#include "player.h"
#include "collider.h"

int main()
{
    bn::core::init();

    // Log output test //
    BN_LOG("Butano Initialized.");

    Room room(DEFAULT_ROOM_1);
    Player player;

    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
    room.setCamera(camera);
    player.setCamera(camera);
    
    Collider test_collider(48, 0, COLLIDER_32x);
    test_collider.setCamera(camera);

    while(true)
    {
	// Update Objects //
	player.update(test_collider);
	
	// Update Camera //
	camera.set_position(player.sprite_ptr->position());

	// Text Test //
	bn::sprite_text_generator text_generator(
	    common::variable_8x16_sprite_font);
	text_generator.set_center_alignment();
	
	bn::vector<bn::sprite_ptr, 32> text_sprites;
	text_generator.generate(0, 40, "BG v4.11.24", text_sprites);

	// Draw Sprites //
        player.draw();

	// Get Tile Index from Cell TEST
	bn::span<const bn::regular_bg_map_cell> cells = room.bg_ptr->map().cells_ref().value();
	bn::regular_bg_map_cell cell_index = cells[bn::regular_bg_items::default_room_2_bg.map_item().cell_index(31, 31)]; // This retrieves the requested index
	bn::regular_bg_map_cell_info cell_info(cell_index); // This retrieves the tile index from the requested cell
	// PROBLEM: Tiles are not uniquely indexed - a flipped tile receives the same index from Butano. Flipped tiles need a unique index.
	BN_LOG(bn::format<32>("Tile index at cell ({},{}): {}", 31, 31, cell_info.tile_index()));
	
	// Update Core //
        bn::core::update();

	// Show the profiler
	// TODO
    }
}
