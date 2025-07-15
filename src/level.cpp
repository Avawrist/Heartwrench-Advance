#include "level.h"

Level::Level()
{
    
}

Level::Level(LevelName level_name)
{
    load(level_name);
}

Level::Level(const Level& other)
{
    current_room = other.current_room;
    
    camera         = other.camera;
    main_bg_ptr    = other.main_bg_ptr;
    painted_bg_ptr = other.painted_bg_ptr;
    object_bg_ptr  = other.object_bg_ptr;
    bg_item        = other.bg_item;
    cells          = other.cells;
    object_bg_item = other.object_bg_item;
    object_cells   = other.object_cells;

    painted_bg_anim_ptr = other.painted_bg_anim_ptr;

    default_main_palette_ptr    = other.default_main_palette_ptr;
    default_painted_palette_ptr = other.default_painted_palette_ptr;

    hud_hp_sprite_ptr         = other.hud_hp_sprite_ptr;
    hud_hp_animate_action_ptr = other.hud_hp_animate_action_ptr;

    tile_width  = other.tile_width;
    tile_height = other.tile_height;

    current_level_name    = other.current_level_name;
    player_spawn          = other.player_spawn;
    fade_in               = other.fade_in;
    fade_out              = other.fade_out;
    cam_is_scrolling      = other.cam_is_scrolling;
    cam_x_offset          = other.cam_x_offset;
    cam_y_offset          = other.cam_y_offset;
    cam_look_x_offset     = other.cam_look_x_offset;
    cam_look_dir_x_offset = other.cam_look_dir_x_offset;
    cam_look_y_offset     = other.cam_look_y_offset;
    cam_update_timer      = other.cam_update_timer;

    random_engine = other.random_engine;
}

Level::~Level()
{
    clear();
}

void Level::operator =(const Level& other)
{
    current_room = other.current_room;

    camera         = other.camera;
    main_bg_ptr    = other.main_bg_ptr;
    painted_bg_ptr = other.painted_bg_ptr;
    object_bg_ptr  = other.object_bg_ptr;
    bg_item        = other.bg_item;
    cells          = other.cells;
    object_bg_item = other.object_bg_item;
    object_cells   = other.object_cells;

    painted_bg_anim_ptr = other.painted_bg_anim_ptr;

    default_main_palette_ptr    = other.default_main_palette_ptr;
    default_painted_palette_ptr = other.default_painted_palette_ptr;

    hud_hp_sprite_ptr         = other.hud_hp_sprite_ptr;
    hud_hp_animate_action_ptr = other.hud_hp_animate_action_ptr;

    tile_width  = other.tile_width;
    tile_height = other.tile_height;

    current_level_name    = other.current_level_name;
    player_spawn          = other.player_spawn;
    fade_in               = other.fade_in;
    fade_out              = other.fade_out;
    cam_is_scrolling      = other.cam_is_scrolling;
    cam_x_offset          = other.cam_x_offset;
    cam_y_offset          = other.cam_y_offset;
    cam_look_x_offset     = other.cam_look_x_offset;
    cam_look_dir_x_offset = other.cam_look_dir_x_offset;
    cam_look_y_offset     = other.cam_look_y_offset;
    cam_update_timer      = other.cam_update_timer;

    random_engine = other.random_engine;
}

void Level::clear()
{
    
    // Free level pointers
    camera.reset();
    main_bg_ptr.reset();
    painted_bg_ptr.reset();
    object_bg_ptr.reset();

    painted_bg_anim_ptr.reset();

    default_main_palette_ptr.reset();
    default_painted_palette_ptr.reset();

    hud_hp_sprite_ptr.reset();
    hud_hp_animate_action_ptr.reset();

    bg_item.reset();
    object_bg_item.reset();

    BN_LOG("=== Level cleared ===");
    BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());
    
}

void Level::load(LevelName level_name)
{
    if(level_name == NO_LEVEL) {return;}

    camera       = bn::camera_ptr::create(0, 0);
    player_spawn = bn::point(0, 0);

    fade_in               = false;
    fade_out              = false;
    cam_is_scrolling      = false;
    cam_x_offset          = 0;
    cam_y_offset          = 0;
    cam_look_x_offset     = 0;
    cam_look_dir_x_offset = 0;
    cam_look_y_offset     = 0;
    cam_update_timer      = 0;

    // Record current level & pos
    current_level_name = level_name;

    // Store room name before constructing
    RoomName temp_room_name = NO_ROOM;

    // Initialize Variables
    switch(level_name)
    {
        case LEVEL_TEST:
            
            // Load BGs //
            painted_bg_ptr = bn::regular_bg_items::test_painted_bg.create_bg(0, 0);

            main_bg_ptr    = bn::regular_bg_items::test_level.create_bg(0, 0);
            bg_item        = bn::regular_bg_items::test_level;

            object_bg_ptr  = bn::regular_bg_items::test_object_bg.create_bg(0, 0);
            object_bg_item = bn::regular_bg_items::test_object_bg;

            // Update cells
            cells        = main_bg_ptr->map().cells_ref().value();
            object_cells = object_bg_ptr->map().cells_ref().value();

            // Set Room //
            temp_room_name = ROOM_TEST_1;

        break;

        default:

            BN_LOG("Level creation failed - Level Name not found.");
            return;

        break;
    }

    current_room = Room(temp_room_name, 
                        camera.value(), 
                        object_bg_ptr.value(), 
                        object_bg_item.value(),
                        object_cells);

    // Store default painted bg palette
    default_main_palette_ptr    = main_bg_ptr->palette();
    default_painted_palette_ptr = painted_bg_ptr->palette();
    
    // Set draw priority for BGs
    painted_bg_ptr->set_z_order(PAINTED_BG_ORDER);
    main_bg_ptr->set_z_order(MAIN_BG_ORDER);
    object_bg_ptr->set_z_order(OBJECT_BG_ORDER);

    // Initialize HUD elements
    hud_hp_sprite_ptr         = bn::sprite_items::hud_hp_bar.create_sprite(0, 0);
    hud_hp_animate_action_ptr = bn::create_sprite_animate_action_forever(hud_hp_sprite_ptr.value(),
                                                                         0,
                                                                         bn::sprite_items::hud_hp_bar.tiles_item(),
                                                                         0, 0);
    hud_hp_sprite_ptr->set_z_order(HUD_Z_LAYER);

    // Set Camera
    main_bg_ptr->set_camera(camera.value());
    painted_bg_ptr->set_camera(camera.value());
    object_bg_ptr->set_camera(camera.value());
    object_bg_ptr->set_visible(false);

    hud_hp_sprite_ptr->set_camera(camera.value());

    // Set black screen
    default_main_palette_ptr->set_fade(bn::colors::black, 1);
    default_painted_palette_ptr->set_fade(bn::colors::black, 1);

    // Trigger fade in
    fade_in = true;

    BN_LOG("=== Level loaded ===");
    BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());
    
}

void Level::reload()
{
    clear(); 
    load(current_level_name);
}

void Level::updateAll()
{
    if(global_hitstop_frames) 
    {
        global_hitstop_frames--;
        if(global_hitstop_frames < 0) {global_hitstop_frames = 0;}

        storePlayerInputs();

        return;
    }

    reloadOnDeath();
    updateObjects();
    updateCamera();
    updateBGFlash();
    updatePaintedBG();
    updateFade();
    freeObjects();
    transitionRoom();
    drawObjects();
}

void Level::updateObjects()
{
    // Update all objects
    for(int32 i = current_room.game_objects.size() - 1; i >= 0; i--)
    {
        if(current_room.game_objects.data()[i] != NULL)
        {
            current_room.game_objects.data()[i]->update(current_room.room_bounds,
                                                        current_room.game_objects,
                                                        main_bg_ptr.value(),
                                                        cells,
                                                        bg_item.value(),
                                                        camera.value());
        }   
    } 

    // Load unloaded objects if needed - Is it appropriate to have this here? 
    current_room.monitorUnloadedObjects(camera.value());
}

void Level::updateCamera()
{
    if(global_hitstop_frames)                                          {return;}
    if(current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) == NULL) {return;}

    GameObject* temp_player_ptr = current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);

    #define HALF_SCREEN_WIDTH  120
    #define HALF_SCREEN_HEIGHT 80

    if(cam_is_scrolling)
    {
        // This method does NOT support diagonal scrolling. 
        // Don't even try it babyyyy.

        // No clamp - apply offsets gradually
        if(abs(cam_x_offset) > 0)
        {
            int32 cam_x_offset_unit = (abs(cam_x_offset) / cam_x_offset) * SCROLL_SPEED;
            camera.value().set_x(camera.value().x() + cam_x_offset_unit);

            cam_x_offset -= cam_x_offset_unit;
        }
        else if(abs(cam_y_offset) > 0)
        {
            int32 cam_y_offset_unit = (abs(cam_y_offset) / cam_y_offset) * SCROLL_SPEED;
            camera.value().set_y(camera.value().y() + cam_y_offset_unit);

            cam_y_offset -= cam_y_offset_unit;
        }
        else
        {
            // Both offsets are 0. Scroll is finished.
            cam_is_scrolling = false;
        }

        // Do an unloaded object sweep since it won't happen otherwise if the cam is moving!
        current_room.monitorUnloadedObjects(camera.value());

    }
    else
    {
        // Typical camera behavior - Start with player position, add look offsets, & clamp to the room bounds. //
        int32 new_cam_x = current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos().x().integer();
        int32 new_cam_y = current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos().y().integer() + CAM_PLAYER_Y_OFFSET;

        if(cam_update_timer % 2 == 0)
        {
            ////////////////////////////////////
            // Determine camera X look offset //
            ////////////////////////////////////
            if(temp_player_ptr->rigidbody.final_dir.x() >= PLAYER_MAX_X_SPEED)       {cam_look_x_offset++;}
            else if(temp_player_ptr->rigidbody.final_dir.x() <= -PLAYER_MAX_X_SPEED) {cam_look_x_offset--;}
            else
            {
                if(cam_look_x_offset > 0)      {cam_look_x_offset--;}
                else if(cam_look_x_offset < 0) {cam_look_x_offset++;}
            }

            ///////////////////////////////////
            // Determine camera X dir offset //
            ///////////////////////////////////
            if(temp_player_ptr->x_dir == RIGHT)     {cam_look_dir_x_offset++;}
            else if(temp_player_ptr->x_dir == LEFT) {cam_look_dir_x_offset--;}

            ////////////////////////////////////
            // Determine camera Y look offset //
            ////////////////////////////////////
            if(bn::keypad::up_held())                                              {cam_look_y_offset--;}
            else if(bn::keypad::down_held() && temp_player_ptr->grounded_detected) {cam_look_y_offset++;}
            else
            {
                if(cam_look_y_offset > 0)      {cam_look_y_offset--;}
                else if(cam_look_y_offset < 0) {cam_look_y_offset++;}
            }
        }

        cam_look_x_offset = clamp(-CAM_MAX_LOOK_X, CAM_MAX_LOOK_X, cam_look_x_offset);
        cam_look_dir_x_offset = clamp(-CAM_MAX_DIR_LOOK_X, CAM_MAX_DIR_LOOK_X, cam_look_dir_x_offset);
        cam_look_y_offset = clamp(-CAM_MAX_LOOK_Y, CAM_MAX_LOOK_Y, cam_look_y_offset);

        //////////////////////////
        // Clamp Camera to Room //
        //////////////////////////
        new_cam_x = clamp(current_room.room_bounds.left_bound  + HALF_SCREEN_WIDTH,  
                          current_room.room_bounds.right_bound - HALF_SCREEN_WIDTH, 
                          new_cam_x + cam_look_x_offset + cam_look_dir_x_offset);
        new_cam_y = clamp(current_room.room_bounds.top_bound   + HALF_SCREEN_HEIGHT, 
                          current_room.room_bounds.bottom_bound - HALF_SCREEN_HEIGHT,
                          new_cam_y);

        // Add Y offset late so it bypasses room clamp.
        new_cam_y += cam_look_y_offset;

        ////////////////////////////
        // Set the final position //
        ////////////////////////////

        camera.value().set_position(new_cam_x, new_cam_y);

        ///////////////////////
        // Apply screenshake //
        ///////////////////////
        if(global_screenshake_frames > 0)
        {
            int32 x_shake_offset = random_engine.get_int(MIN_X_SHAKE_RANGE, MAX_X_SHAKE_RANGE);
            int32 y_shake_offset = random_engine.get_int(MIN_Y_SHAKE_RANGE, MAX_Y_SHAKE_RANGE);

            camera.value().set_position(camera.value().x() + (x_shake_offset * global_screenshake_severity), 
                                        camera.value().y() + (y_shake_offset * global_screenshake_severity));
        }

    }

    //////////////////////////////////////
    // Update screenshake frame counter //
    //////////////////////////////////////
    global_screenshake_frames--;
    if(global_screenshake_frames <= 0) 
    {
        global_screenshake_frames   = 0;
        global_screenshake_severity = NO_SHAKE;
    }

    /////////////////////////
    // Update camera timer //
    /////////////////////////
    cam_update_timer++;
    if(cam_update_timer >= 60) {cam_update_timer = 0;}

    ////////////////
    // Update HUD //
    ////////////////  
    drawHUD();

}

void Level::updateBGFlash()
{
    if(global_bg_hitflash_frames)
    {
        // Set flash palette
        bn::bg_palette_ptr flash_palette = bn::bg_palette_items::bg_flash_palette.create_palette();
        main_bg_ptr->set_palette(flash_palette);
    }
    else
    {
        // Set default palette
        main_bg_ptr->set_palette(default_main_palette_ptr.value());
    }

    // Update bg hitflash frames
    global_bg_hitflash_frames--;
    if(global_bg_hitflash_frames < 0)
    {global_bg_hitflash_frames = 0;}
}

void Level::updatePaintedBG()
{   
    if(current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) == NULL) 
    {    
        painted_bg_ptr->set_position(current_room.center());
        return;
    }

    // Parallax Effect
    #define PARALLAX_REDUCTION_FACTOR -32 // The larger the number, the slower the BG will scroll.
    bn::fixed x_offset = (current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX)->x() - current_room.center().x()) / PARALLAX_REDUCTION_FACTOR;
    painted_bg_ptr->set_position(current_room.center().x() + x_offset, current_room.center().y());

    // Animate
    if(painted_bg_anim_ptr.get() != NULL)
    {
        if(global_timer % GLOBAL_TIMER_MAX == 0)
        {
            painted_bg_anim_ptr.reset();
            painted_bg_anim_ptr = bn::create_regular_bg_animate_action_once(painted_bg_ptr.value(),
                                                                            2,
                                                                            bn::regular_bg_items::test_painted_bg.map_item(),
                                                                            1, 1, 2, 2, 3, 3, 4, 4, 0, 0);
        }

        if(!painted_bg_anim_ptr->done()) {painted_bg_anim_ptr->update();}
    }
}

void Level::updateGlobalTimer()
{
    global_timer++;
    if(global_timer >= GLOBAL_TIMER_MAX) {global_timer = 0;}
}

void Level::reloadOnDeath()
{
    // If player died, reload the level
    if(((Player*)(current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX)))->is_dead)
    {
        // Trigger the fade out
        fade_out = true;

        // If fade out is done, we can reload.
        if(default_painted_palette_ptr->fade_intensity() == 1)
        {reload();}
    }
}

void Level::freeObjects()
{
    #define INDEX_AFTER_PLAYER 1

    int32 last_index = current_room.game_objects.size() - 1;
    for(int32 i = INDEX_AFTER_PLAYER; i <= last_index; i++)
    {
        if(current_room.game_objects.at(i)->is_dead)
        {
            // 1. Update the UnloadedObject with the Unloaded ID:
            int32 object_id = current_room.game_objects.at(i)->object_id;
            int32 unloaded_index = current_room.findUnloadedObjectIndex(object_id);
            if(unloaded_index > -1)
            {current_room.unloaded_objects.at(unloaded_index).loaded_instance_id = UNLOADED_OBJECT_STATE_DEAD;}

            // 2. Remove the dead object:
            delete current_room.game_objects.at(i);
            current_room.game_objects.at(i) = current_room.game_objects.at(last_index);
            current_room.game_objects.pop_back();
            last_index--;
        }

        else if(current_room.game_objects.at(i)->is_inactive)
        {
            // 1. Update the UnloadedObject with the Unloaded ID:
            int32 object_id = current_room.game_objects.at(i)->object_id;
            int32 unloaded_index = current_room.findUnloadedObjectIndex(object_id);
            if(unloaded_index > -1)
            {current_room.unloaded_objects.at(unloaded_index).loaded_instance_id = UNLOADED_OBJECT_STATE_UNLOADED;}

            // 2. Remove the inactive object:
            delete current_room.game_objects.at(i);
            current_room.game_objects.at(i) = current_room.game_objects.at(last_index);
            current_room.game_objects.pop_back();
            last_index--;
        }
    }

    current_room.updateIndexes();
}

void Level::transitionRoom()
{
    #define SCREEN_WIDTH  240
    #define SCREEN_HEIGHT 160

    if(current_room.game_objects.size() == 0) {return;}

    Player temp_player = Player(*((Player*)(current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX))));
    
    if(temp_player.pos().x() > current_room.room_bounds.right_bound)
    {
        if(current_room.right_neighbor != NO_ROOM)
        {
            // Create the neighbor room
            current_room = Room(current_room.right_neighbor, 
                                camera.value(),
                                object_bg_ptr.value(), 
                                object_bg_item.value(), 
                                object_cells);

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(temp_player);

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = SCREEN_WIDTH;
            cam_y_offset     = 0;

            return;
        }
    }

    else if(temp_player.pos().x() < current_room.room_bounds.left_bound)
    {
        if(current_room.left_neighbor != NO_ROOM)
        {

            // Create the neighbor room
            current_room = Room(current_room.left_neighbor, 
                                camera.value(),
                                object_bg_ptr.value(), 
                                object_bg_item.value(), 
                                object_cells);

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(temp_player);

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = -SCREEN_WIDTH;
            cam_y_offset     = 0;

            return;
        }
    }

    else if(temp_player.pos().y() < current_room.room_bounds.top_bound)
    {
        if(current_room.top_neighbor != NO_ROOM)
        {
            // Create the neighbor room
            current_room = Room(current_room.top_neighbor, 
                                camera.value(),
                                object_bg_ptr.value(), 
                                object_bg_item.value(), 
                                object_cells);

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(temp_player);

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = 0;
            cam_y_offset     = -SCREEN_HEIGHT;

            return;
        }
    }

    else if(temp_player.pos().y() > current_room.room_bounds.bottom_bound)
    {
        if(current_room.bottom_neighbor != NO_ROOM)
        {
            // Create the neighbor room
            current_room = Room(current_room.bottom_neighbor, 
                                camera.value(),
                                object_bg_ptr.value(), 
                                object_bg_item.value(),
                                object_cells);

            // Free up the default player object that came with the new room,
            // and replace with the player object from the previous room
            delete current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
            current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) = new Player(temp_player);

            // Set camera offsets
            cam_is_scrolling = true;
            cam_x_offset     = 0;
            cam_y_offset     = SCREEN_HEIGHT;

            return;
        }
    }
}

void Level::drawHUD()
{
    // Get temp player pointer
    GameObject* temp_player_ptr = current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
    
    if(temp_player_ptr != NULL)
    {
        if(hud_hp_animate_action_ptr.has_value())
        {
            // Set Position
            hud_hp_sprite_ptr->set_position(camera->x() + HUD_HP_X_OFFSET, camera->y() + HUD_HP_Y_OFFSET);

            // Update Graphic
            hud_hp_animate_action_ptr = bn::create_sprite_animate_action_forever(hud_hp_sprite_ptr.value(),
																                 0,
                                                                                 bn::sprite_items::hud_hp_bar.tiles_item(),
                                                                                 temp_player_ptr->hitpoints, temp_player_ptr->hitpoints);

            hud_hp_animate_action_ptr->update();
        }
    }
}

void Level::drawObjects()
{
    global_tiles_in_VRAM = 0;

    // Update & draw all objects
    for(int32 i = current_room.game_objects.size() - 1; i >= 0; i--)
    {
        if(current_room.game_objects.data()[i] != NULL)
        {
            current_room.game_objects.data()[i]->draw();
        }   
    }
}

void Level::updateFade()
{
    bn::fixed fade_intensity = default_main_palette_ptr->fade_intensity();

    if(fade_in)
    {
        // Fade objects in
        for(int32 i = 0; i < current_room.game_objects.size(); i++)
        {
            GameObject* object_ptr = current_room.game_objects.at(i);

            bn::sprite_palette_ptr object_palette       = object_ptr->sprite_ptr->palette();
            bn::sprite_palette_ptr hit_effect_palette   = object_ptr->hit_effect_sprite_ptr->palette();
            bn::sprite_palette_ptr splat_effect_palette = object_ptr->splat_effect_sprite_ptr->palette();
            bn::sprite_palette_ptr hp_bar_palette       = object_ptr->hp_sprite_ptr->palette();

            object_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));
            hit_effect_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));
            splat_effect_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));
            hp_bar_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));
        }

        // Fade BGs in
        default_main_palette_ptr->set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));
        default_painted_palette_ptr->set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));

        // Fade HUD in
        bn::sprite_palette_ptr hud_hp_palette = hud_hp_sprite_ptr->palette();
        hud_hp_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));

        // End condition
        if(default_main_palette_ptr->fade_intensity() == 0) {fade_in = false;}
    }
    else if(fade_out)
    {
        // Fade objects out
        for(int32 i = 0; i < current_room.game_objects.size(); i++)
        {
            GameObject* object_ptr = current_room.game_objects.at(i);
            
            bn::sprite_palette_ptr object_palette       = object_ptr->sprite_ptr->palette();
            bn::sprite_palette_ptr hit_effect_palette   = object_ptr->hit_effect_sprite_ptr->palette();
            bn::sprite_palette_ptr splat_effect_palette = object_ptr->splat_effect_sprite_ptr->palette();
            bn::sprite_palette_ptr hp_bar_palette       = object_ptr->hp_sprite_ptr->palette();

            object_palette.set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));
            hit_effect_palette.set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));
            splat_effect_palette.set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));
            hp_bar_palette.set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));

            // If fully faded out, stop drawing it and restore the palette.
            // This method assumes a fully faded out GameObject will be deleted. 
            if(object_palette.fade_intensity() == 1) 
            {
                object_palette.set_fade(bn::colors::black, 0);
                hit_effect_palette.set_fade(bn::colors::black, 0);
                splat_effect_palette.set_fade(bn::colors::black, 0);
                hp_bar_palette.set_fade(bn::colors::black, 0);

                object_ptr->sprite_ptr->set_visible(false);
                object_ptr->hit_effect_sprite_ptr->set_visible(false);
                object_ptr->splat_effect_sprite_ptr->set_visible(false);
                object_ptr->hp_sprite_ptr->set_visible(false);
            }
        }

        // Fade BGs out
        default_main_palette_ptr->set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));
        default_painted_palette_ptr->set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));

        // Fade HUD out
        bn::sprite_palette_ptr hud_hp_palette = hud_hp_sprite_ptr->palette();
        hud_hp_palette.set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));

        if(hud_hp_palette.fade_intensity() == 1)
        {
            hud_hp_palette.set_fade(bn::colors::black, 0);
            hud_hp_sprite_ptr->set_visible(false);
        }

        // End condition
        if(default_main_palette_ptr->fade_intensity() == 1) {fade_out = false;}
    }
}

void Level::storePlayerInputs()
{
    // This function exists to take input from the player even during
    // hitstop frames. This way, the player isnt locked out of input
    // just for the sake of juice.

    if(current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) == NULL)
    {return;}

    if(bn::keypad::r_pressed())
    {((Player*)current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX))->roll_requested = true;}

    if(bn::keypad::a_pressed())
    {((Player*)current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX))->jump_requested = true;}

    if(bn::keypad::b_pressed())
    {((Player*)current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX))->attack_requested = true;}
}