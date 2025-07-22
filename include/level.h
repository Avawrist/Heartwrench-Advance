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

// Flash Palette
#include "bn_bg_palette_items_bg_flash_palette.h"

// BGs
#include "bn_regular_bg_items_title_screen_level_bg.h"
#include "bn_regular_bg_items_title_screen_object_bg.h"
#include "bn_regular_bg_items_title_screen_painted_bg.h"

#include "bn_regular_bg_items_ziggurat_1_level_bg.h"
#include "bn_regular_bg_items_ziggurat_1_object_bg.h"
#include "bn_regular_bg_items_ziggurat_1_painted_bg.h"

// Pause Menu
#include "bn_regular_bg_items_pause_screen.h"

// HUD Elements
#include "bn_sprite_items_hud_hp_bar.h"
#include "bn_sprite_items_currency_number.h"
#include "bn_sprite_items_hud_currency_icon.h"

// My Libs
#include "math.h"
#include "room.h"
#include "utility.h"
#include "player.h"

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

// HUD Macros
#define HUD_Z_LAYER -4

#define HUD_HP_X_OFFSET -86
#define HUD_HP_Y_OFFSET -61

#define HUD_CURRENCY_NUM_1_X_OFFSET -30
#define HUD_CURRENCY_NUM_1_Y_OFFSET -68

#define HUD_CURRENCY_NUM_2_X_OFFSET -23
#define HUD_CURRENCY_NUM_2_Y_OFFSET -68

#define HUD_CURRENCY_ICON_X_OFFSET -46
#define HUD_CURRENCY_ICON_Y_OFFSET -66

// Pause Menu
#define CURSOR_CONTINUE      0
#define CURSOR_RETURN_TO_MAP 1
#define CURSOR_QUIT_GAME     2

// Fade 
#define LEVEL_FADE_INCREMENT 0.05

// Currency
#define LEVEL_MAX_CURRENCY 100

// Transitions
#define LEVEL_TITLE_SCREEN_TRANSITION_FRAMES 60

enum LevelName 
{
    NO_LEVEL = 0,
    LEVEL_TITLE_SCREEN,
    LEVEL_ZIGGURAT_1
};

struct Level
{
    Room current_room;

    // BGs
    bn::optional<bn::camera_ptr> camera;
    
    bn::optional<bn::regular_bg_ptr> main_bg_ptr;
    bn::optional<bn::regular_bg_ptr> painted_bg_ptr;
    bn::optional<bn::regular_bg_ptr> object_bg_ptr;

	bn::optional<bn::regular_bg_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> painted_bg_anim_ptr;

    bn::optional<bn::bg_palette_ptr>        default_main_palette_ptr;
    bn::optional<bn::bg_palette_ptr>        default_painted_palette_ptr;
    
    bn::optional<bn::regular_bg_item>       bg_item;
    bn::span<const bn::regular_bg_map_cell> cells;
    
    bn::optional<bn::regular_bg_item>       object_bg_item;
    bn::span<const bn::regular_bg_map_cell> object_cells;

    // Pause Menu
    bn::optional<bn::regular_bg_ptr> pause_screen_bg_ptr;
    bn::optional<bn::regular_bg_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> pause_screen_bg_anim_ptr;

    // HUD
    bn::optional<bn::sprite_palette_ptr> default_hud_palette_ptr;

    bn::optional<bn::sprite_ptr>                                         hud_hp_sprite_ptr;
	bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> hud_hp_animate_action_ptr;

    bn::optional<bn::sprite_ptr>                                         currency_num_1_sprite_ptr;
	bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> currency_num_1_animate_action_ptr;

    bn::optional<bn::sprite_ptr>                                         currency_num_2_sprite_ptr;
	bn::optional<bn::sprite_animate_action<GAME_OBJECT_MAX_ANIM_FRAMES>> currency_num_2_animate_action_ptr;

    bn::optional<bn::sprite_ptr>                                         currency_icon_sprite_ptr;

    // Other
    uint32 tile_width;
    uint32 tile_height;

    LevelName current_level_name;

    bn::fixed_point player_spawn;
    
    bool  fade_in;
    bool  fade_out;
    bool  cam_is_scrolling;
    bool  menu_open;

    int32 cam_x_offset;
    int32 cam_y_offset;
    int32 cam_look_x_offset;
    int32 cam_look_dir_x_offset;
    int32 cam_look_y_offset;

    int32 cam_update_timer;

    bn::random random_engine;

    uint32 displayed_currency;
    uint32 currency;

    int32 cursor_index;

    // Transitions
    int32 transition_frames;

    Level();
    Level(LevelName level_name);
    Level(const Level& other);
    ~Level();

    void operator =(const Level& other);

    void clear();
    void load(LevelName level_name);
    void reload();
    void loadNew(LevelName level_name);

    void update();
    void updateAll();
    void updateObjects();
    void updateCamera();
    void updateBGFlash();
    void updatePaintedBG();
    void updateGlobalTimer();
    void updateCurrency();
    void reloadOnDeath();
    void freeObjects();
    void transitionRoom();
    void updateHUD();
    void drawObjects();
    void updateFade();
    void storePlayerInputs();
    void updateTitleScreen();
    void togglePauseScreen();
    void updatePauseScreen();
    void updateLevelTransition(LevelName level_index);
};

#endif