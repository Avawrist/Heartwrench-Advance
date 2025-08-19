#include "level.h"

//////////////////
// Struct Spawn //
//////////////////

Spawn::Spawn()
{
    spawn_pos   = bn::fixed_point(0, 0);
    spawn_room  = NO_ROOM;
    spawn_level = NO_LEVEL;
}

Spawn::Spawn(const Spawn& other)
{
    spawn_pos   = other.spawn_pos;
    spawn_room  = other.spawn_room;
    spawn_level = other.spawn_level;
}

Spawn::~Spawn()
{}

void Spawn::operator =(const Spawn& other)
{
    spawn_pos   = other.spawn_pos;
    spawn_room  = other.spawn_room;
    spawn_level = other.spawn_level;
}

void Spawn::setSpawnPosAC(int32 spawn_x, int32 spawn_y)
{
    spawn_pos.set_x(spawn_x - (LEVEL_WIDTH / 2));
    spawn_pos.set_y(spawn_y - (LEVEL_HEIGHT / 2));
}

//////////////////
// Struct Level //
//////////////////

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

    painted_bg_anim_ptr = other.painted_bg_anim_ptr;

    default_main_palette_ptr    = other.default_main_palette_ptr;
    default_painted_palette_ptr = other.default_painted_palette_ptr;
    default_flash_palette_ptr   = other.default_flash_palette_ptr;

    pause_screen_bg_ptr = other.pause_screen_bg_ptr;
    pause_screen_bg_anim_ptr = other.pause_screen_bg_anim_ptr;

    hud_hp_sprite_ptr         = other.hud_hp_sprite_ptr;
    hud_hp_animate_action_ptr = other.hud_hp_animate_action_ptr;

    currency_num_1_sprite_ptr         = other.currency_num_1_sprite_ptr;
	currency_num_1_animate_action_ptr = other.currency_num_1_animate_action_ptr;

    currency_num_2_sprite_ptr         = other.currency_num_2_sprite_ptr;
	currency_num_2_animate_action_ptr = other.currency_num_2_animate_action_ptr;

    currency_icon_sprite_ptr = other.currency_icon_sprite_ptr;

    default_hud_palette_ptr = other.default_hud_palette_ptr;

    hud_level_name = other.hud_level_name;

    tile_width  = other.tile_width;
    tile_height = other.tile_height;

    player_spawn          = other.player_spawn;
    fade_in               = other.fade_in;
    fade_out              = other.fade_out;
    cam_is_scrolling      = other.cam_is_scrolling;
    menu_open             = other.menu_open;
    pause_requested       = other.pause_requested;
    level_complete        = other.level_complete;

    cam_x_offset          = other.cam_x_offset;
    cam_y_offset          = other.cam_y_offset;
    cam_look_x_offset     = other.cam_look_x_offset;
    cam_look_dir_x_offset = other.cam_look_dir_x_offset;
    cam_look_y_offset     = other.cam_look_y_offset;
    cam_update_timer      = other.cam_update_timer;

    random_engine = other.random_engine;

    name_card_frame    = other.name_card_frame;
    displayed_currency = other.displayed_currency;
    transition_frames  = other.transition_frames;
    cursor_index       = other.cursor_index;

    next_level = other.next_level;
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

    painted_bg_anim_ptr = other.painted_bg_anim_ptr;

    default_main_palette_ptr    = other.default_main_palette_ptr;
    default_painted_palette_ptr = other.default_painted_palette_ptr;
    default_flash_palette_ptr   = other.default_flash_palette_ptr;

    pause_screen_bg_ptr = other.pause_screen_bg_ptr;
    pause_screen_bg_anim_ptr = other.pause_screen_bg_anim_ptr;

    hud_hp_sprite_ptr         = other.hud_hp_sprite_ptr;
    hud_hp_animate_action_ptr = other.hud_hp_animate_action_ptr;

    currency_num_1_sprite_ptr         = other.currency_num_1_sprite_ptr;
	currency_num_1_animate_action_ptr = other.currency_num_1_animate_action_ptr;

    currency_num_2_sprite_ptr         = other.currency_num_2_sprite_ptr;
	currency_num_2_animate_action_ptr = other.currency_num_2_animate_action_ptr;

    currency_icon_sprite_ptr = other.currency_icon_sprite_ptr;

    default_hud_palette_ptr = other.default_hud_palette_ptr;

    hud_level_name = other.hud_level_name;

    tile_width  = other.tile_width;
    tile_height = other.tile_height;

    player_spawn          = other.player_spawn;
    fade_in               = other.fade_in;
    fade_out              = other.fade_out;
    cam_is_scrolling      = other.cam_is_scrolling;
    menu_open             = other.menu_open;
    pause_requested       = other.pause_requested;
    level_complete        = other.level_complete;

    cam_x_offset          = other.cam_x_offset;
    cam_y_offset          = other.cam_y_offset;
    cam_look_x_offset     = other.cam_look_x_offset;
    cam_look_dir_x_offset = other.cam_look_dir_x_offset;
    cam_look_y_offset     = other.cam_look_y_offset;
    cam_update_timer      = other.cam_update_timer;

    random_engine = other.random_engine;

    name_card_frame    = other.name_card_frame;
    displayed_currency = other.displayed_currency;
    transition_frames  = other.transition_frames;
    cursor_index       = other.cursor_index;

    next_level = other.next_level;
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
    default_flash_palette_ptr.reset();

    pause_screen_bg_ptr.reset();
    pause_screen_bg_anim_ptr.reset();

    hud_hp_sprite_ptr.reset();
    hud_hp_animate_action_ptr.reset();

    currency_num_1_sprite_ptr.reset();
	currency_num_1_animate_action_ptr.reset();

    currency_num_2_sprite_ptr.reset();
	currency_num_2_animate_action_ptr.reset();

    currency_icon_sprite_ptr.reset();

    default_hud_palette_ptr.reset();

    bg_item.reset();
    object_bg_item.reset();

    hud_level_name.initSprites();

    BN_LOG("=== Level cleared ===");
    BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());
    
}

void Level::load()
{
    if(player_spawn.spawn_level == NO_LEVEL) {return;}

    camera = bn::camera_ptr::create(0, 0);

    fade_in               = false;
    fade_out              = false;
    cam_is_scrolling      = false;
    menu_open             = false;
    pause_requested       = false;
    level_complete        = false;

    cam_x_offset          = 0;
    cam_y_offset          = 0;
    cam_look_x_offset     = 0;
    cam_look_dir_x_offset = 0;
    cam_look_y_offset     = 0;
    cam_update_timer      = 0;
    name_card_frame       = 0;
    //global_level_currency = 0;
    //displayed_currency    = 0;
    transition_frames     = -1;
    cursor_index          = 0;

    next_level = NO_LEVEL;

    // Initialize Variables
    switch(player_spawn.spawn_level)
    {
        case LEVEL_NAME_CARD:

            // Load BGs //
            main_bg_ptr    = bn::regular_bg_items::name_card_level_bg.create_bg(0, 0);
            bg_item        = bn::regular_bg_items::name_card_level_bg;

            object_bg_ptr  = bn::regular_bg_items::name_card_object_bg.create_bg(0, 0);
            object_bg_item = bn::regular_bg_items::name_card_object_bg;

            painted_bg_ptr      = bn::regular_bg_items::name_card_painted_bg.create_bg(0, 0);
            painted_bg_anim_ptr = bn::create_regular_bg_animate_action_forever(painted_bg_ptr.value(),
                                                                               0,
                                                                               bn::regular_bg_items::name_card_painted_bg.map_item(),
                                                                               0, 0);

            // Update flash palette
            default_flash_palette_ptr = bn::bg_palette_items::ziggurat_1_bg_flash_palette.create_palette();

            // Update cells
            cells = main_bg_ptr->map().cells_ref().value();

            // Init name card frames
            name_card_frame = LEVEL_NAME_CARD_FRAMES;

            // Next level
            next_level = LEVEL_TITLE_SCREEN;

        break;

        case LEVEL_TITLE_SCREEN:

            // Load BGs //
            main_bg_ptr    = bn::regular_bg_items::title_screen_level_bg.create_bg(0, 0);
            bg_item        = bn::regular_bg_items::title_screen_level_bg;

            object_bg_ptr  = bn::regular_bg_items::title_screen_object_bg.create_bg(0, 0);
            object_bg_item = bn::regular_bg_items::title_screen_object_bg;

            painted_bg_ptr      = bn::regular_bg_items::title_screen_painted_bg.create_bg(0, 0);
            painted_bg_anim_ptr = bn::create_regular_bg_animate_action_forever(painted_bg_ptr.value(),
                                                                               6,
                                                                               bn::regular_bg_items::title_screen_painted_bg.map_item(),
                                                                               0, 0, 0, 0, 0, 1, 1, 1, 1);

            // Update flash palette
            default_flash_palette_ptr = bn::bg_palette_items::ziggurat_1_bg_flash_palette.create_palette();

            // Update cells
            cells        = main_bg_ptr->map().cells_ref().value();

            // Next level
            next_level = LEVEL_ZIGGURAT_1;

        break;

        case LEVEL_ZIGGURAT_1:
            
            // Load BGs //
            main_bg_ptr    = bn::regular_bg_items::ziggurat_1_level_bg.create_bg(0, 0);
            bg_item        = bn::regular_bg_items::ziggurat_1_level_bg;

            object_bg_ptr  = bn::regular_bg_items::ziggurat_1_object_bg.create_bg(0, 0);
            object_bg_item = bn::regular_bg_items::ziggurat_1_object_bg;

            painted_bg_ptr      = bn::regular_bg_items::ziggurat_1_painted_bg.create_bg(0, 0);
            painted_bg_anim_ptr = bn::create_regular_bg_animate_action_forever(painted_bg_ptr.value(),
                                                                                3,
                                                                                bn::regular_bg_items::ziggurat_1_painted_bg.map_item(),
                                                                                0, 0, 0, 1, 1, 1, 2, 2, 2);

            // Update flash palette
            default_flash_palette_ptr = bn::bg_palette_items::ziggurat_1_bg_flash_palette.create_palette();

            // Update cells
            cells = main_bg_ptr->map().cells_ref().value();

            // Update HUD level name text box
            hud_level_name.setSpritesFromString("ETERNAL_ZIGGURAT", 16);

            // Next level
            next_level = LEVEL_TITLE_SCREEN;
            
        break;

        default:

            BN_LOG("Level creation failed - Level Name not found.");
            return;

        break;
    }

    // Populate object cells
    //populateObjectCells();

    // Init room
    current_room = Room(player_spawn.spawn_room, 
                        camera.value(), 
                        object_bg_ptr.value(), 
                        object_bg_item.value(),
                        object_cells,
                        player_spawn.spawn_pos);

    // Hide Player on the title screen
    if(player_spawn.spawn_level == LEVEL_TITLE_SCREEN)
    {current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX)->hideSprites();}

    // Store default painted bg palette
    default_main_palette_ptr    = main_bg_ptr->palette();
    default_painted_palette_ptr = painted_bg_ptr->palette();
    
    // Set draw priority for BGs
    painted_bg_ptr->set_z_order(PAINTED_BG_ORDER);
    main_bg_ptr->set_z_order(MAIN_BG_ORDER);
    object_bg_ptr->set_z_order(OBJECT_BG_ORDER);

    // Initialize Pause Menu
    pause_screen_bg_ptr = bn::regular_bg_items::pause_screen.create_bg(0, 0);
    pause_screen_bg_ptr->set_z_order(PAUSE_BG_ORDER);
    pause_screen_bg_ptr->set_visible(false);

    pause_screen_bg_anim_ptr = bn::create_regular_bg_animate_action_forever(pause_screen_bg_ptr.value(),
                                                                            0,
                                                                            bn::regular_bg_items::pause_screen.map_item(),
                                                                            0, 0);

    // Initialize HUD elements
    hud_hp_sprite_ptr = bn::sprite_items::hud_hp_bar.create_sprite(HUD_HP_X_OFFSET, HUD_HP_Y_OFFSET);
    hud_hp_sprite_ptr->set_z_order(HUD_Z_LAYER);
    hud_hp_sprite_ptr->set_visible(false);
    hud_hp_animate_action_ptr = bn::create_sprite_animate_action_forever(hud_hp_sprite_ptr.value(),
                                                                         0,
                                                                         bn::sprite_items::hud_hp_bar.tiles_item(),
                                                                         0, 0);

    currency_num_1_sprite_ptr = bn::sprite_items::currency_number.create_sprite(HUD_CURRENCY_NUM_1_X_OFFSET, HUD_CURRENCY_NUM_1_Y_OFFSET);
    currency_num_1_sprite_ptr->set_z_order(HUD_Z_LAYER);
    currency_num_1_sprite_ptr->set_visible(false);
	currency_num_1_animate_action_ptr = bn::create_sprite_animate_action_forever(currency_num_1_sprite_ptr.value(),
                                                                                 0,
                                                                                 bn::sprite_items::currency_number.tiles_item(),
                                                                                 0, 0);

    currency_num_2_sprite_ptr = bn::sprite_items::currency_number.create_sprite(HUD_CURRENCY_NUM_2_X_OFFSET, HUD_CURRENCY_NUM_2_Y_OFFSET);
    currency_num_2_sprite_ptr->set_z_order(HUD_Z_LAYER);
    currency_num_2_sprite_ptr->set_visible(false);
	currency_num_2_animate_action_ptr = bn::create_sprite_animate_action_forever(currency_num_1_sprite_ptr.value(),
                                                                                 0,
                                                                                 bn::sprite_items::currency_number.tiles_item(),
                                                                                 0, 0);

    currency_icon_sprite_ptr = bn::sprite_items::hud_currency_icon.create_sprite(HUD_CURRENCY_ICON_X_OFFSET, HUD_CURRENCY_ICON_Y_OFFSET);
    currency_icon_sprite_ptr->set_z_order(HUD_Z_LAYER);
    currency_icon_sprite_ptr->set_visible(false);

    default_hud_palette_ptr = hud_hp_sprite_ptr->palette();

    hud_level_name.setPosUR(HUD_LEVEL_NAME_X_OFFSET, HUD_LEVEL_NAME_Y_OFFSET);
    hud_level_name.setVisible(false);

    // Set Camera
    main_bg_ptr->set_camera(camera.value());
    painted_bg_ptr->set_camera(camera.value());
    object_bg_ptr->set_camera(camera.value());
    object_bg_ptr->set_visible(false);

    pause_screen_bg_ptr->set_camera(camera.value());

    hud_hp_sprite_ptr->set_camera(camera.value());

    currency_num_1_sprite_ptr->set_camera(camera.value());
    currency_num_2_sprite_ptr->set_camera(camera.value());

    currency_icon_sprite_ptr->set_camera(camera.value());

    hud_level_name.setCamera(camera.value());

    // Set black screen
    default_main_palette_ptr->set_fade(bn::colors::black, 1);
    default_painted_palette_ptr->set_fade(bn::colors::black, 1);

    // Trigger fade in
    fade_in = true;

    BN_LOG("=== Level loaded ===");
    BN_LOG("Bytes allocated in EWRAM: ", bn::memory::used_alloc_ewram());
    
}

void Level::load(LevelName level_name)
{
    if(level_name == NO_LEVEL) {return;}

    camera = bn::camera_ptr::create(0, 0);

    player_spawn.spawn_pos   = bn::fixed_point(0, 0);
    player_spawn.spawn_room  = NO_ROOM; 
    player_spawn.spawn_level = level_name;

    fade_in               = false;
    fade_out              = false;
    cam_is_scrolling      = false;
    menu_open             = false;
    pause_requested       = false;
    level_complete        = false;

    cam_x_offset          = 0;
    cam_y_offset          = 0;
    cam_look_x_offset     = 0;
    cam_look_dir_x_offset = 0;
    cam_look_y_offset     = 0;
    cam_update_timer      = 0;
    name_card_frame       = 0;
    global_level_currency = 0;
    displayed_currency    = 0;
    transition_frames     = -1;
    cursor_index          = 0;

    next_level = NO_LEVEL;

    // Initialize Variables
    switch(player_spawn.spawn_level)
    {
        case LEVEL_NAME_CARD:

            // Player Spawn //
            player_spawn.spawn_room = ROOM_NAME_CARD;

            // Load BGs //
            main_bg_ptr    = bn::regular_bg_items::name_card_level_bg.create_bg(0, 0);
            bg_item        = bn::regular_bg_items::name_card_level_bg;

            object_bg_ptr  = bn::regular_bg_items::name_card_object_bg.create_bg(0, 0);
            object_bg_item = bn::regular_bg_items::name_card_object_bg;

            painted_bg_ptr      = bn::regular_bg_items::name_card_painted_bg.create_bg(0, 0);
            painted_bg_anim_ptr = bn::create_regular_bg_animate_action_forever(painted_bg_ptr.value(),
                                                                               0,
                                                                               bn::regular_bg_items::name_card_painted_bg.map_item(),
                                                                               0, 0);

            // Update flash palette
            default_flash_palette_ptr = bn::bg_palette_items::ziggurat_1_bg_flash_palette.create_palette();

            // Update cells
            cells        = main_bg_ptr->map().cells_ref().value();

            // Init name card frames
            name_card_frame = LEVEL_NAME_CARD_FRAMES;

            // Update HUD level name text box
            hud_level_name.setSpritesFromString("", 0);

            // Next level
            next_level = LEVEL_TITLE_SCREEN;

        break;

        case LEVEL_TITLE_SCREEN:

            // Player Spawn //
            player_spawn.spawn_room = ROOM_TITLE_SCREEN;

            // Load BGs //
            main_bg_ptr    = bn::regular_bg_items::title_screen_level_bg.create_bg(0, 0);
            bg_item        = bn::regular_bg_items::title_screen_level_bg;

            object_bg_ptr  = bn::regular_bg_items::title_screen_object_bg.create_bg(0, 0);
            object_bg_item = bn::regular_bg_items::title_screen_object_bg;

            painted_bg_ptr      = bn::regular_bg_items::title_screen_painted_bg.create_bg(0, 0);
            painted_bg_anim_ptr = bn::create_regular_bg_animate_action_forever(painted_bg_ptr.value(),
                                                                               6,
                                                                               bn::regular_bg_items::title_screen_painted_bg.map_item(),
                                                                               0, 0, 0, 0, 0, 1, 1, 1, 1);

            // Update flash palette
            default_flash_palette_ptr = bn::bg_palette_items::ziggurat_1_bg_flash_palette.create_palette();

            // Update cells
            cells        = main_bg_ptr->map().cells_ref().value();

            // Update HUD level name text box
            hud_level_name.setSpritesFromString("", 0);

            // Next level
            next_level = LEVEL_ZIGGURAT_1;

        break;

        case LEVEL_ZIGGURAT_1:
            
            // Player Spawn //
            player_spawn.setSpawnPosAC(64, 592);
            player_spawn.spawn_room = ROOM_TEST_1;

            // Load BGs //
            main_bg_ptr    = bn::regular_bg_items::ziggurat_1_level_bg.create_bg(0, 0);
            bg_item        = bn::regular_bg_items::ziggurat_1_level_bg;

            object_bg_ptr  = bn::regular_bg_items::ziggurat_1_object_bg.create_bg(0, 0);
            object_bg_item = bn::regular_bg_items::ziggurat_1_object_bg;

            painted_bg_ptr      = bn::regular_bg_items::ziggurat_1_painted_bg.create_bg(0, 0);
            painted_bg_anim_ptr = bn::create_regular_bg_animate_action_forever(painted_bg_ptr.value(),
                                                                               3,
                                                                               bn::regular_bg_items::ziggurat_1_painted_bg.map_item(),
                                                                               0, 0, 0, 1, 1, 1, 2, 2, 2);

            // Update flash palette
            default_flash_palette_ptr = bn::bg_palette_items::ziggurat_1_bg_flash_palette.create_palette();

            // Update cells
            cells = main_bg_ptr->map().cells_ref().value();

            // Update HUD level name text box
            hud_level_name.setSpritesFromString("ETERNAL_ZIGGURAT", 16);

            // Next level
            next_level = LEVEL_TITLE_SCREEN;
            
        break;

        default:

            BN_LOG("Level creation failed - Level Name not found.");
            return;

        break;
    }

    // Populate object cells
    populateObjectCells();

    // Init room
    current_room = Room(player_spawn.spawn_room, 
                        camera.value(), 
                        object_bg_ptr.value(), 
                        object_bg_item.value(),
                        object_cells,
                        player_spawn.spawn_pos);

    // Hide Player on the title screen
    if(player_spawn.spawn_level == LEVEL_TITLE_SCREEN)
    {current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX)->hideSprites();}

    // Store default painted bg palette
    default_main_palette_ptr    = main_bg_ptr->palette();
    default_painted_palette_ptr = painted_bg_ptr->palette();
    
    // Set draw priority for BGs
    painted_bg_ptr->set_z_order(PAINTED_BG_ORDER);
    main_bg_ptr->set_z_order(MAIN_BG_ORDER);
    object_bg_ptr->set_z_order(OBJECT_BG_ORDER);

    // Initialize Pause Menu
    pause_screen_bg_ptr = bn::regular_bg_items::pause_screen.create_bg(0, 0);
    pause_screen_bg_ptr->set_z_order(PAUSE_BG_ORDER);
    pause_screen_bg_ptr->set_visible(false);

    pause_screen_bg_anim_ptr = bn::create_regular_bg_animate_action_forever(pause_screen_bg_ptr.value(),
                                                                            0,
                                                                            bn::regular_bg_items::pause_screen.map_item(),
                                                                            0, 0);

    // Initialize HUD elements
    hud_hp_sprite_ptr = bn::sprite_items::hud_hp_bar.create_sprite(HUD_HP_X_OFFSET, HUD_HP_Y_OFFSET);
    hud_hp_sprite_ptr->set_z_order(HUD_Z_LAYER);
    hud_hp_sprite_ptr->set_visible(false);
    hud_hp_animate_action_ptr = bn::create_sprite_animate_action_forever(hud_hp_sprite_ptr.value(),
                                                                         0,
                                                                         bn::sprite_items::hud_hp_bar.tiles_item(),
                                                                         0, 0);

    currency_num_1_sprite_ptr = bn::sprite_items::currency_number.create_sprite(HUD_CURRENCY_NUM_1_X_OFFSET, HUD_CURRENCY_NUM_1_Y_OFFSET);
    currency_num_1_sprite_ptr->set_z_order(HUD_Z_LAYER);
    currency_num_1_sprite_ptr->set_visible(false);
	currency_num_1_animate_action_ptr = bn::create_sprite_animate_action_forever(currency_num_1_sprite_ptr.value(),
                                                                                 0,
                                                                                 bn::sprite_items::currency_number.tiles_item(),
                                                                                 0, 0);

    currency_num_2_sprite_ptr = bn::sprite_items::currency_number.create_sprite(HUD_CURRENCY_NUM_2_X_OFFSET, HUD_CURRENCY_NUM_2_Y_OFFSET);
    currency_num_2_sprite_ptr->set_z_order(HUD_Z_LAYER);
    currency_num_2_sprite_ptr->set_visible(false);
	currency_num_2_animate_action_ptr = bn::create_sprite_animate_action_forever(currency_num_1_sprite_ptr.value(),
                                                                                 0,
                                                                                 bn::sprite_items::currency_number.tiles_item(),
                                                                                 0, 0);

    currency_icon_sprite_ptr = bn::sprite_items::hud_currency_icon.create_sprite(HUD_CURRENCY_ICON_X_OFFSET, HUD_CURRENCY_ICON_Y_OFFSET);
    currency_icon_sprite_ptr->set_z_order(HUD_Z_LAYER);
    currency_icon_sprite_ptr->set_visible(false);

    default_hud_palette_ptr = hud_hp_sprite_ptr->palette();

    hud_level_name.setPosUR(HUD_LEVEL_NAME_X_OFFSET, HUD_LEVEL_NAME_Y_OFFSET);
    hud_level_name.setVisible(false);

    // Set Camera
    main_bg_ptr->set_camera(camera.value());
    painted_bg_ptr->set_camera(camera.value());
    object_bg_ptr->set_camera(camera.value());
    object_bg_ptr->set_visible(false);

    pause_screen_bg_ptr->set_camera(camera.value());

    hud_hp_sprite_ptr->set_camera(camera.value());

    currency_num_1_sprite_ptr->set_camera(camera.value());
    currency_num_2_sprite_ptr->set_camera(camera.value());

    currency_icon_sprite_ptr->set_camera(camera.value());

    hud_level_name.setCamera(camera.value());

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
    load();
}

void Level::loadNew(LevelName level_name)
{
    clear(); 
    load(level_name);
}

void Level::update()
{
    // Update Global Timer
    updateGlobalHitstop();
    updateGlobalTimer();

    if(global_hitstop_frames <= 0)
    {
             if(player_spawn.spawn_level == LEVEL_NAME_CARD)    {updateNameCard();}
        else if(player_spawn.spawn_level == LEVEL_TITLE_SCREEN) {updateTitleScreen();}
        else if(level_complete)                                 {updateLevelComplete();}
        else if(menu_open)                                      {updatePauseScreen();}
        else if(cam_is_scrolling)                               {updateCamera(); 
                                                                 updateCheckpoints();}
        else                                                    {updateAll();}
    }

    updateLevelTransition(next_level);
}

void Level::updateAll()
{
    reloadOnDeath();
    updateObjects();
    removeObjectCells();
    updateCurrency();
    updateCamera();
    updateBGFlash();
    freeObjects();
    transitionRoom();
    drawObjects();
}

void Level::updateNameCard()
{
    // Update Name Card timer
    name_card_frame--;
    name_card_frame = clamp(0, LEVEL_NAME_CARD_FRAMES, name_card_frame);

    // Update fade
    updateFade();

    // Hide HUD
    hud_hp_sprite_ptr->set_visible(false);
    currency_num_1_sprite_ptr->set_visible(false);
    currency_num_2_sprite_ptr->set_visible(false);
    currency_icon_sprite_ptr->set_visible(false);

    // Delete Player
    if(current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) != NULL)
    {
        delete current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
        current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) = NULL;
    }

    // Center Camera
    camera.value().set_position(0, 0);

    // Get Input //
    if((bn::keypad::start_pressed() || bn::keypad::a_pressed() || name_card_frame <= 0) &&
        transition_frames < 0)
    {
        // Start Fade and Level Transition
        fade_out = true;
        transition_frames = LEVEL_TITLE_SCREEN_TRANSITION_FRAMES;
    }

    // Draw Screen
    updatePaintedBG();
}

void Level::updateTitleScreen()
{
    // Update fade
    updateFade();

    // Hide HUD
    hud_hp_sprite_ptr->set_visible(false);
    currency_num_1_sprite_ptr->set_visible(false);
    currency_num_2_sprite_ptr->set_visible(false);
    currency_icon_sprite_ptr->set_visible(false);

    // Delete Player
    if(current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) != NULL)
    {
        delete current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
        current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) = NULL;
    }

    // Center Camera
    camera.value().set_position(0, 0);

    // Get Input //
    if((bn::keypad::start_pressed() || bn::keypad::a_pressed()) && transition_frames < 0)
    {
        // Start Fade and Level Transition
        fade_out = true;
        transition_frames = LEVEL_TITLE_SCREEN_TRANSITION_FRAMES;

        painted_bg_anim_ptr = bn::create_regular_bg_animate_action_forever(painted_bg_ptr.value(),
                                                                           0,
                                                                           bn::regular_bg_items::title_screen_painted_bg.map_item(),
                                                                           0, 0, 1, 1);
    }

    // Draw Screen
    updatePaintedBG();
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
            
            // Update spawn point //
            if(current_room.game_objects.data()[i]->object_type == CHECKPOINT)
            {updateCheckpoint((Checkpoint*)current_room.game_objects.data()[i]);}

            // Check if level complete //
            else if(current_room.game_objects.data()[i]->object_type == FINISH_SEAL)
            {
                if(((FinishSeal*)current_room.game_objects.data()[i])->level_complete) 
                {level_complete = true;}
            }
        }   
    } 

    // Review object requests
    current_room.monitorObjectRequests(camera.value());

    // Load unloaded objects if needed
    current_room.monitorUnloadedObjects(camera.value());

    // Toggle Pause Menu
    if(bn::keypad::start_pressed() || pause_requested) 
    {
        pause_requested = false; 
        togglePauseScreen();
    }
}

void Level::updateCamera()
{
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

        ////////////////////////////////////
        // Determine camera X look offset //
        ////////////////////////////////////
        if(temp_player_ptr->rigidbody.final_dir.x()      >=  PLAYER_MAX_X_SPEED) {cam_look_x_offset++;}
        else if(temp_player_ptr->rigidbody.final_dir.x() <= -PLAYER_MAX_X_SPEED) {cam_look_x_offset--;}
        else
        {
            if(cam_look_x_offset > 0)      {cam_look_x_offset--;}
            else if(cam_look_x_offset < 0) {cam_look_x_offset++;}
        }

        ///////////////////////////////////
        // Determine camera X dir offset //
        ///////////////////////////////////
        if(temp_player_ptr->rigidbody.normalized_dir.x() != 0)
        {
            if(temp_player_ptr->x_dir == RIGHT)     {cam_look_dir_x_offset++;}
            else if(temp_player_ptr->x_dir == LEFT) {cam_look_dir_x_offset--;}
        }

        ////////////////////////////////////
        // Determine camera Y look offset //
        ////////////////////////////////////
        if(bn::keypad::up_held()        && temp_player_ptr->grounded_detected) {cam_look_y_offset--;}
        else if(bn::keypad::down_held() && temp_player_ptr->grounded_detected) {cam_look_y_offset++;}
        else
        {
            if(cam_look_y_offset > 0)      {cam_look_y_offset--;}
            else if(cam_look_y_offset < 0) {cam_look_y_offset++;}
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
        new_cam_y = clamp(current_room.room_bounds.top_bound    + HALF_SCREEN_HEIGHT, 
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

    // Update Fade //
    updateFade();

    // Update HUD //
    updateHUD();

    // Update Painted BG //
    updatePaintedBG();
}

void Level::updateGlobalHitstop()
{
    global_hitstop_frames--;
    if(global_hitstop_frames < 0) {global_hitstop_frames = 0;}

    storePlayerInputs();
}

void Level::updateBGFlash()
{
    if(fade_out) {main_bg_ptr->set_palette(default_main_palette_ptr.value()); return;}

    if(global_bg_hitflash_frames)
    {
        // Set flash palette
        main_bg_ptr->set_palette(default_flash_palette_ptr.value());
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
    // Parallax Effect
    #define PARALLAX_REDUCTION_FACTOR -4 // The larger the number, the slower the BG will scroll.
    bn::fixed x_offset = camera->x() / PARALLAX_REDUCTION_FACTOR;
    painted_bg_ptr->set_position(camera->x() + x_offset, current_room.room_bounds.center().y());

    painted_bg_anim_ptr->update();
}

void Level::updateGlobalTimer()
{
    global_timer++;
    if(global_timer >= GLOBAL_TIMER_MAX) {global_timer = 0;}
}

void Level::updateCurrency()
{
    // Cap currency
    if(global_level_currency > LEVEL_MAX_CURRENCY) {global_level_currency = LEVEL_MAX_CURRENCY;}

    // Update currency ticks
    if(global_timer % 4 == 0) 
    {
        if(displayed_currency < global_level_currency)      {displayed_currency++;}
        else if(displayed_currency > global_level_currency) {displayed_currency--;}
    }
}

void Level::updateHUD()
{
    // Get temp player pointer
    GameObject* temp_player_ptr = current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX);
    
    if(temp_player_ptr != NULL)
    {
        /////////////
        // Draw HP //
        /////////////

        // Update palette
        bn::sprite_palette_ptr white_palette = bn::sprite_palette_items::sprite_white_palette.create_palette();

        if(global_hud_hp_flash_frames) {hud_hp_sprite_ptr->set_palette(white_palette);}
        else                           {hud_hp_sprite_ptr->set_palette(default_hud_palette_ptr.value());}

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

        ///////////////////
        // Draw Currency //
        ///////////////////

        // Update palette
        if(global_hud_currency_flash_frames) {currency_icon_sprite_ptr->set_palette(white_palette);}
        else                                 {currency_icon_sprite_ptr->set_palette(default_hud_palette_ptr.value());}

        // Numbers
        currency_num_1_sprite_ptr->set_position(camera->x() + HUD_CURRENCY_NUM_1_X_OFFSET, camera->y() + HUD_CURRENCY_NUM_1_Y_OFFSET);
        bn::fixed unrounded_num_1 = displayed_currency / 10;
        int32 num_1               = clamp(0, 9, unrounded_num_1.floor_integer());
	    currency_num_1_animate_action_ptr = bn::create_sprite_animate_action_once(currency_num_1_sprite_ptr.value(),
                                                                                  0,
                                                                                  bn::sprite_items::currency_number.tiles_item(),
                                                                                  num_1, num_1);

        currency_num_2_sprite_ptr->set_position(camera->x() + HUD_CURRENCY_NUM_2_X_OFFSET, camera->y() + HUD_CURRENCY_NUM_2_Y_OFFSET);
        int32 num_2 = displayed_currency % 10;
	    currency_num_2_animate_action_ptr = bn::create_sprite_animate_action_once(currency_num_2_sprite_ptr.value(),
                                                                                  0,
                                                                                  bn::sprite_items::currency_number.tiles_item(),
                                                                                  num_2, num_2);

        // Use red nums if currency is maxed out
        if(num_1 == 9 && num_2 == 9)
        {
            currency_num_1_animate_action_ptr = bn::create_sprite_animate_action_once(currency_num_1_sprite_ptr.value(),
                                                                            0,
                                                                            bn::sprite_items::currency_number.tiles_item(),
                                                                            10, 10);

            currency_num_2_animate_action_ptr = bn::create_sprite_animate_action_once(currency_num_2_sprite_ptr.value(),
                                                                            0,
                                                                            bn::sprite_items::currency_number.tiles_item(),
                                                                            10, 10);
        }

        currency_num_1_animate_action_ptr->update();
        currency_num_2_animate_action_ptr->update();

        // Icon
        currency_icon_sprite_ptr->set_position(camera->x() + HUD_CURRENCY_ICON_X_OFFSET, 
                                               camera->y() + HUD_CURRENCY_ICON_Y_OFFSET);

        /////////////////////
        // Draw Level Name //
        /////////////////////

        hud_level_name.setPosUR(camera->x().integer() + HUD_LEVEL_NAME_X_OFFSET, 
                                camera->y().integer() + HUD_LEVEL_NAME_Y_OFFSET);
        hud_level_name.draw();
    }

        /////////////////////////////
        // Update HUD Flash Frames //
        /////////////////////////////

        global_hud_hp_flash_frames--;
        global_hud_hp_flash_frames = clamp(0, HUD_FLASH_FRAMES, global_hud_hp_flash_frames);

        global_hud_currency_flash_frames--;
        global_hud_currency_flash_frames = clamp(0, HUD_FLASH_FRAMES, global_hud_currency_flash_frames);
}

void Level::updateFade()
{
    bn::fixed fade_intensity = default_main_palette_ptr->fade_intensity();

    if(fade_in)
    {
        // Reveal HUD
        hud_hp_sprite_ptr->set_visible(true);
        currency_num_1_sprite_ptr->set_visible(true);
        currency_num_2_sprite_ptr->set_visible(true);
        currency_icon_sprite_ptr->set_visible(true);
        hud_level_name.setVisible(true);

        // Fade objects in
        for(int32 i = 0; i < current_room.game_objects.size(); i++)
        {
            GameObject* object_ptr = current_room.game_objects.at(i);
            if(object_ptr != NULL)
            {
                bn::sprite_palette_ptr object_palette       = object_ptr->sprite_ptr->palette();
                bn::sprite_palette_ptr hit_effect_palette   = object_ptr->hit_effect_sprite_ptr->palette();
                bn::sprite_palette_ptr splat_effect_palette = object_ptr->splat_effect_sprite_ptr->palette();
                bn::sprite_palette_ptr hp_bar_palette       = object_ptr->hp_sprite_ptr->palette();

                object_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));
                hit_effect_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));
                splat_effect_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));
                hp_bar_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));
            }
        }

        // Fade BGs in
        default_main_palette_ptr->set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));
        default_painted_palette_ptr->set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));
        default_flash_palette_ptr->set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));

        // Fade HUD in
        bn::sprite_palette_ptr hud_hp_palette = hud_hp_sprite_ptr->palette();
        hud_hp_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));

        bn::sprite_palette_ptr hud_currency_num_1_palette = currency_num_1_sprite_ptr->palette();
        hud_currency_num_1_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));

        bn::sprite_palette_ptr hud_currency_num_2_palette = currency_num_1_sprite_ptr->palette();
        hud_currency_num_2_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));

        bn::sprite_palette_ptr hud_currency_icon_palette = currency_icon_sprite_ptr->palette();
        hud_currency_icon_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));

        // Fade Pause Screen in
        bn::bg_palette_ptr pause_screen_palette = pause_screen_bg_ptr->palette();
        pause_screen_palette.set_fade(bn::colors::black, max(0, fade_intensity - LEVEL_FADE_INCREMENT));

        // End condition
        if(default_main_palette_ptr->fade_intensity() == 0) {fade_in = false;}
    }
    else if(fade_out)
    {
        // Fade objects out
        for(int32 i = 0; i < current_room.game_objects.size(); i++)
        {
            GameObject* object_ptr = current_room.game_objects.at(i);
            
            if(object_ptr != NULL)
            {

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
        }

        // Fade BGs out
        default_main_palette_ptr->set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));
        default_painted_palette_ptr->set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));
        default_flash_palette_ptr->set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));

        // Fade HUD out
        bn::sprite_palette_ptr hud_hp_palette = hud_hp_sprite_ptr->palette();
        hud_hp_palette.set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));

        bn::sprite_palette_ptr hud_currency_num_1_palette = currency_num_1_sprite_ptr->palette();
        hud_currency_num_1_palette.set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));

        bn::sprite_palette_ptr hud_currency_num_2_palette = currency_num_1_sprite_ptr->palette();
        hud_currency_num_2_palette.set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));

        bn::sprite_palette_ptr hud_currency_icon_palette = currency_icon_sprite_ptr->palette();
        hud_currency_icon_palette.set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));

        if(hud_hp_palette.fade_intensity() == 1)
        {
            hud_hp_palette.set_fade(bn::colors::black, 0);
            hud_hp_sprite_ptr->set_visible(false);

            hud_currency_num_1_palette.set_fade(bn::colors::black, 0);
            currency_num_1_sprite_ptr->set_visible(false);

            hud_currency_num_2_palette.set_fade(bn::colors::black, 0);
            currency_num_2_sprite_ptr->set_visible(false);

            hud_currency_icon_palette.set_fade(bn::colors::black, 0);
            currency_icon_sprite_ptr->set_visible(false);

            hud_level_name.setVisible(false);
        }

        // Fade Pause Screen out
        bn::bg_palette_ptr pause_screen_palette = pause_screen_bg_ptr->palette();
        pause_screen_palette.set_fade(bn::colors::black, min(1, fade_intensity + LEVEL_FADE_INCREMENT));

        // End condition
        if(default_main_palette_ptr->fade_intensity() == 1) {fade_out = false;}
    }
}

void Level::updatePauseScreen()
{
    // Update Fade //
    updateFade();

    // Get Input
    if(transition_frames < 0)
    {
        if(bn::keypad::up_pressed())   {cursor_index--;}
        if(bn::keypad::down_pressed()) {cursor_index++;}

        if(cursor_index > CURSOR_QUIT_GAME)     {cursor_index = CURSOR_CONTINUE;}
        else if(cursor_index < CURSOR_CONTINUE) {cursor_index = CURSOR_QUIT_GAME;}
        
        if(bn::keypad::b_pressed()) {togglePauseScreen();}

        else if(bn::keypad::a_pressed() || bn::keypad::start_pressed())
        {
            switch(cursor_index)
            {
                case CURSOR_CONTINUE:

                    togglePauseScreen();

                break;

                case CURSOR_RETURN_TO_MAP:
                break;

                case CURSOR_QUIT_GAME:

                    // Start Fade and Level Transition
                    fade_out          = true;
                    transition_frames = LEVEL_TITLE_SCREEN_TRANSITION_FRAMES;
                    next_level        = LEVEL_TITLE_SCREEN;

                break;

                default:
                break;
            }
        }
    }

    // Draw Pause Screen //
    pause_screen_bg_anim_ptr = bn::create_regular_bg_animate_action_forever(pause_screen_bg_ptr.value(),
                                                                            0,
                                                                            bn::regular_bg_items::pause_screen.map_item(),
                                                                            cursor_index, cursor_index);
    pause_screen_bg_anim_ptr->update();
}

void Level::updateLevelTransition(LevelName level_index)
{
    if(level_index < LEVEL_NAME_CARD || level_index > LEVEL_ZIGGURAT_1) {return;}

    // Transition Level //
    if(transition_frames > 0)
    {
        transition_frames--;
        transition_frames = clamp(0, LEVEL_TITLE_SCREEN_TRANSITION_FRAMES, transition_frames);

        if(transition_frames == 0)
        {loadNew(level_index);}
    }
}

void Level::updateCheckpoint(Checkpoint* checkpoint_ptr)
{
    if(checkpoint_ptr->state  == CHECKPOINT_IDLE_OFF && 
       player_spawn.spawn_pos == checkpoint_ptr->pos())
    {checkpoint_ptr->setState(CHECKPOINT_IDLE_ON);}

    if(checkpoint_ptr->state == CHECKPOINT_ACTIVE &&
       global_level_currency >= checkpoint_ptr->cost)
    {
        // Take cost
        global_level_currency -= checkpoint_ptr->cost;

        // Update spawn data
        player_spawn.spawn_pos   = checkpoint_ptr->pos();
        player_spawn.spawn_room  = current_room.room_name;

        // Animate checkpoint
        checkpoint_ptr->setState(CHECKPOINT_OVERWRITE);
    }
}

void Level::updateCheckpoints()
{
    for(int32 i = current_room.game_objects.size() - 1; i > PLAYER_OBJECT_LIST_INDEX; i--)
    {
        if(current_room.game_objects.data()[i] != NULL &&
           current_room.game_objects.data()[i]->object_type == CHECKPOINT)
        {           
            updateCheckpoint((Checkpoint*)current_room.game_objects.data()[i]);
            current_room.game_objects.data()[i]->draw();
        }
    }
}

void Level::updateLevelComplete()
{
    updateAll();
    
    // 1. Set a victory theme to play once
    // ...

    // 2. Set player victory animation to play once
    // ...

    // 3. if victory theme and player animation are both done, trigger transition:
    if(current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) != NULL &&
       current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX)->animate_action_ptr->done())
    {
        fade_out          = true;
        transition_frames = LEVEL_TITLE_SCREEN_TRANSITION_FRAMES;
    }
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
                                object_cells,
                                player_spawn.spawn_pos);

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
                                object_cells,
                                player_spawn.spawn_pos);

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
                                object_cells,
                                player_spawn.spawn_pos);

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
                                object_cells,
                                player_spawn.spawn_pos);

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

void Level::drawObjects()
{
    if(menu_open || player_spawn.spawn_level == LEVEL_TITLE_SCREEN) {return;}

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

void Level::storePlayerInputs()
{
    // This function exists to take input from the player even during
    // hitstop frames. This way, the player isnt locked out of input
    // just for the sake of juice.

    // Store start input in level object:
    if(bn::keypad::start_pressed() && !menu_open) {pause_requested = true;}

    // Store all other inputs in Player object:
    if(current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX) == NULL) {return;}

    if(bn::keypad::a_pressed() && !menu_open)
    {((Player*)current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX))->a_requested = true;}

    if(bn::keypad::b_pressed() && !menu_open)
    {((Player*)current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX))->b_requested = true;}

    if(bn::keypad::r_pressed() && !menu_open)
    {((Player*)current_room.game_objects.at(PLAYER_OBJECT_LIST_INDEX))->r_requested = true;}
}

void Level::togglePauseScreen()
{
    if(menu_open) 
    {
        menu_open = false;
        pause_screen_bg_ptr->set_visible(false);

        // Reveal HUD Sprites //
        hud_hp_sprite_ptr->set_visible(true);
        currency_num_1_sprite_ptr->set_visible(true);
        currency_num_2_sprite_ptr->set_visible(true);
        currency_icon_sprite_ptr->set_visible(true);
        hud_level_name.setVisible(true);

        // Reveal GameObjects //
        for(int32 i = 0; i < current_room.game_objects.size(); i++)
        {
            if(current_room.game_objects.at(i) != NULL)
            {
                current_room.game_objects.at(i)->revealSprites();
            }
        }
    }

    else          
    {
        menu_open    = true;
        cursor_index = CURSOR_CONTINUE;
        pause_screen_bg_ptr->set_visible(true);

        // Update Position //
        pause_screen_bg_ptr->set_position(camera.value().x(), camera.value().y());

        // Hide HUD Sprites //
        hud_hp_sprite_ptr->set_visible(false);
        currency_num_1_sprite_ptr->set_visible(false);
        currency_num_2_sprite_ptr->set_visible(false);
        currency_icon_sprite_ptr->set_visible(false);
        hud_level_name.setVisible(false);

        // Hide GameObjects //
        for(int32 i = 0; i < current_room.game_objects.size(); i++)
        {
            if(current_room.game_objects.at(i) != NULL)
            {current_room.game_objects.at(i)->hideSprites();}
        }
    }
}

void Level::populateObjectCells()
{
    // Look at the object tile index of the UL corner of each 32x32 tile.
    // Object tiles must be placed in the UL corner to be recorded.

    #define OBJECT_CELLS_REDUCTION_FACTOR 4

    uint32 cell_width  = object_bg_ptr.value().dimensions().width()  / TILE_WIDTH / OBJECT_CELLS_REDUCTION_FACTOR;
    uint32 cell_height = object_bg_ptr.value().dimensions().height() / TILE_HEIGHT / OBJECT_CELLS_REDUCTION_FACTOR;

    for(uint32 x = 0; x < cell_width; x++)
    {
        for(uint32 y = 0; y < cell_height; y++)
        {
            object_cells[x][y] = object_bg_item.value().map_item().cell(x * 2, y * 2);
        }
    }
}

void Level::removeObjectCells()
{
    int32 half_level_width_pixels  = object_bg_ptr.value().dimensions().width()  / 2;
	int32 half_level_height_pixels = object_bg_ptr.value().dimensions().height() / 2;
    int32 object_id;
    int32 unloaded_index;
    uint32 cell_x;
    uint32 cell_y;

    for(int32 i = 0; i < current_room.game_objects.size(); i++)
    {
        if(current_room.game_objects.at(i)->state == OBJECT_DEATH && 
           current_room.game_objects.at(i)->is_persistent)
        {
            // Search for the unloaded object by index, if found, remove the object tile:
            object_id      = current_room.game_objects.at(i)->object_id;
            unloaded_index = current_room.findUnloadedObjectIndex(object_id);

            if(unloaded_index > -1)
            {
                cell_x = (current_room.unloaded_objects.at(unloaded_index).room_pos.x() + half_level_width_pixels)  / TILE_WIDTH  / 2;
                cell_y = (current_room.unloaded_objects.at(unloaded_index).room_pos.y() + half_level_height_pixels + 1) / TILE_HEIGHT / 2;

                setDynamicTileAtBGIndex(cell_x, cell_y, (uint8)NO_TYPE, object_cells);
            }

        }
    }
}