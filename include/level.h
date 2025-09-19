#ifndef LEVEL_H
#define LEVEL_H

// Butano
#include "bn_log.h"
#include "bn_core.h"
#include "bn_random.h"
#include "bn_math.h"
#include "bn_vector.h"
#include "bn_camera_ptr.h"
#include "bn_span.h"
#include "bn_regular_bg_ptr.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_map_ptr.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_bg_palette_ptr.h"
#include "bn_colors.h"
#include "bn_regular_bg_animate_actions.h"
#include "bn_sound.h"

#include "bn_music_items.h"
#include "bn_sound_items.h"

// Flash Palettes
#include "bn_bg_palette_items_troll_tolls_bg_flash_palette.h"

// BGs
#include "bn_regular_bg_items_name_card_level_bg.h"
#include "bn_regular_bg_items_name_card_object_bg.h"
#include "bn_regular_bg_items_name_card_painted_bg.h"

#include "bn_regular_bg_items_title_screen_level_bg.h"
#include "bn_regular_bg_items_title_screen_object_bg.h"
#include "bn_regular_bg_items_title_screen_painted_bg.h"

#include "bn_regular_bg_items_overworld_level_bg.h"
#include "bn_regular_bg_items_overworld_object_bg.h"
#include "bn_regular_bg_items_overworld_painted_bg.h"

#include "bn_regular_bg_items_troll_tolls_level_bg.h"
#include "bn_regular_bg_items_troll_tolls_object_bg.h"
#include "bn_regular_bg_items_troll_tolls_painted_bg.h"

// Title player sprite
#include "bn_sprite_items_player_title.h"

// Pause Menu
#include "bn_regular_bg_items_pause_screen.h"

// HUD Elements
#include "bn_sprite_items_hud_stripe.h"
#include "bn_sprite_items_hud_hp_bar.h"
#include "bn_sprite_items_currency_number.h"
#include "bn_sprite_items_hud_currency_icon.h"

// My Libs
#include "math.h"
#include "text_box.h"
#include "room.h"
#include "utility.h"
#include "player.h"
#include "checkpoint.h"
#include "finish_seal.h"

#define LEVEL_SFX_MASTER_VOLUME 0.6

/////////////////////
// Struct SaveData //
/////////////////////

struct SaveData 
{
    bool no_data;
    
    int32  ow_player_location_x;
    int32  ow_player_location_y;
    
    uint8 stars_collected;

    bool troll_tolls_complete;
    bool troll_tolls_room_3_star;
    bool troll_tolls_room_5_star;
    bool troll_tolls_room_6_star;
    bool troll_tolls_room_7_star;
    bool troll_tolls_room_9_star;
    bool troll_tolls_room_13_star;
};

//////////////////
// Struct Level //
//////////////////

// 240 and 160 MUST be divisible by SCROLL_SPEED.
#define SCROLL_SPEED 8

#define MIN_X_SHAKE_RANGE -3
#define MAX_X_SHAKE_RANGE  3
#define MIN_Y_SHAKE_RANGE -3
#define MAX_Y_SHAKE_RANGE  3

// BG Z orders (Higher orders are drawn first)
#define PAINTED_BG_ORDER 4
#define BACKDROP_ORDER   3
#define MAIN_BG_ORDER    2
#define OBJECT_BG_ORDER  1

#define PAUSE_BG_ORDER  -5

// Painted BG
#define LEVEL_PARALLAX_REDUCTION_FACTOR -4 // The larger the number, the slower the BG will scroll.

// HUD Macros
#define HUD_STRIPE_Z_LAYER -3
#define HUD_Z_LAYER        -4

#define HUD_STRIPE_1_X_OFFSET -88
#define HUD_STRIPE_1_Y_OFFSET -62
#define HUD_STRIPE_2_X_OFFSET -30
#define HUD_STRIPE_2_Y_OFFSET -62
#define HUD_STRIPE_3_X_OFFSET  30
#define HUD_STRIPE_3_Y_OFFSET -62
#define HUD_STRIPE_4_X_OFFSET  88
#define HUD_STRIPE_4_Y_OFFSET -62

#define HUD_HP_X_OFFSET -83
#define HUD_HP_Y_OFFSET -62

#define HUD_CURRENCY_NUM_1_X_OFFSET -12
#define HUD_CURRENCY_NUM_1_Y_OFFSET -67

#define HUD_CURRENCY_NUM_2_X_OFFSET  -5
#define HUD_CURRENCY_NUM_2_Y_OFFSET -67

#define HUD_CURRENCY_ICON_X_OFFSET -35
#define HUD_CURRENCY_ICON_Y_OFFSET -68

#define HUD_LEVEL_NAME_X_OFFSET  120
#define HUD_LEVEL_NAME_Y_OFFSET -67

// Pause Menu
#define CURSOR_CONTINUE      0
#define CURSOR_RETURN_TO_MAP 1
#define CURSOR_QUIT_GAME     2

// Fade 
#define LEVEL_FADE_INCREMENT 0.05

// Currency
#define LEVEL_MAX_CURRENCY 99

// Name Card Screen
#define LEVEL_NAME_CARD_FRAMES 120

// Title Screen
#define PLAYER_TITLE_X_POS 0
#define PLAYER_TITLE_Y_POS 16

#define TITLE_SCROLL_SPEED 1

// Troll Tolls
#define LEVEL_TROLL_TOLLS_PAINTED_BG_X 192
#define LEVEL_TROLL_TOLLS_PAINTED_BG_Y 1600

// Transitions
#define LEVEL_TITLE_SCREEN_TRANSITION_FRAMES 60

enum LevelName 
{
    NO_LEVEL = 0,
    LEVEL_NAME_CARD,
    LEVEL_TITLE_SCREEN,
    LEVEL_OVERWORLD,
    LEVEL_TROLL_TOLLS
};

//////////////////
// Struct Spawn //
//////////////////

struct Spawn
{
    Spawn();
    Spawn(const Spawn& other);
    ~Spawn();

    void operator =(const Spawn& other);

    bn::fixed_point spawn_pos;
    RoomName        spawn_room;
    LevelName       spawn_level;

    void setSpawnPosAC(int32 spawn_x, int32 spawn_y);
};

//////////////////
// Struct Level //
//////////////////

struct Level
{
    Room current_room;

    // SaveData
    volatile SaveData* save_data_ptr = (volatile SaveData*)SRAM_BASE_ADDRESS;

    // BGs
    bn::optional<bn::camera_ptr> camera;
    
    bn::optional<bn::regular_bg_ptr> main_bg_ptr;
    bn::optional<bn::regular_bg_ptr> painted_bg_ptr;
    bn::optional<bn::regular_bg_ptr> object_bg_ptr;

    bn::optional<bn::regular_bg_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> level_bg_anim_ptr;
	bn::optional<bn::regular_bg_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> painted_bg_anim_ptr;

    bn::optional<bn::bg_palette_ptr> default_main_palette_ptr;
    bn::optional<bn::bg_palette_ptr> default_painted_palette_ptr;
    bn::optional<bn::bg_palette_ptr> default_flash_palette_ptr;
    
    bn::optional<bn::regular_bg_item>       bg_item;
    bn::span<const bn::regular_bg_map_cell> cells;
    
    bn::optional<bn::regular_bg_item> object_bg_item;
    uint8**                           object_cells;

    // Title screen sprites
    bn::optional<bn::sprite_ptr>                                         title_player_sprite_ptr;
    bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> title_player_anim_ptr;

    // Pause Menu
    bn::optional<bn::regular_bg_ptr> pause_screen_bg_ptr;
    bn::optional<bn::regular_bg_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> pause_screen_bg_anim_ptr;

    // HUD
    bn::optional<bn::sprite_palette_ptr> default_hud_palette_ptr;

    bn::optional<bn::sprite_ptr>                                         hud_stripe_1_sprite_ptr;
    bn::optional<bn::sprite_ptr>                                         hud_stripe_2_sprite_ptr;
    bn::optional<bn::sprite_ptr>                                         hud_stripe_3_sprite_ptr;
    bn::optional<bn::sprite_ptr>                                         hud_stripe_4_sprite_ptr;
    bn::optional<bn::sprite_ptr>                                         hud_hp_sprite_ptr;
	bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> hud_hp_animate_action_ptr;

    bn::optional<bn::sprite_ptr>                                         currency_num_1_sprite_ptr;
	bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> currency_num_1_animate_action_ptr;

    bn::optional<bn::sprite_ptr>                                         currency_num_2_sprite_ptr;
	bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> currency_num_2_animate_action_ptr;

    bn::optional<bn::sprite_ptr>                                         currency_icon_sprite_ptr;

    TextBox hud_level_name;

    // Other
    uint32 tile_width;
    uint32 tile_height;

    Spawn player_spawn;
    
    bool  fade_in;
    bool  fade_out;
    bool  cam_is_scrolling;
    bool  menu_open;
    bool  pause_requested;
    bool  level_complete;

    int32 cam_x_offset;
    int32 cam_y_offset;
    int32 cam_look_x_offset;
    int32 cam_look_dir_x_offset;
    int32 cam_look_y_offset;

    int32 cam_update_timer;

    bn::random random_engine;

    uint32 displayed_currency;
    int32  name_card_frame;
    int32  cursor_index;

    // Transitions
    int32 transition_frames;

    LevelName next_level;

    Level();
    Level(LevelName level_name);
    Level(const Level& other);
    ~Level();

    void operator =(const Level& other);

    void clear();
    void load();
    void load(LevelName level_name);
    void reload();
    void loadNew(LevelName level_name);

    void update();
    void updateAll();
    void updateNameCard();
    void updateTitleScreen();
    void updateOverworld();
    void updateObjects();
    void updateCamera();
    void updateGlobalHitstop();
    void updateBGFlash();
    void updatePaintedBG();
    void updateGlobalTimer();
    void updateCurrency();
    void updateFade();
    void updateHUD();
    void updatePauseInputs();
    void updatePauseScreen();
    void updateLevelTransition(LevelName level_index);
    void updateCheckpoint(Checkpoint* checkpoint_ptr);
    void updateCheckpoints();
    void updateLevelComplete();

    void reloadOnDeath();
    void freeObjects();
    void transitionRoom();
    void drawObjects();
    void storePlayerInputs();
    void togglePauseScreen();
    void initObjectCells();
    void populateObjectCells();
    void removeObjectCells();
    void freeObjectCells();

    void saveCurrentFile();
    void loadSave();

    void animateLevelBG();
};

#endif