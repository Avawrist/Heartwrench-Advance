#ifndef TEXT_BOX_H
#define TEXT_BOX_H

// Butano
#include "bn_optional.h"
#include "bn_camera_ptr.h"
#include "bn_vector.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_sprite_palette_actions.h"

#include "bn_fixed.h"
#include "bn_fixed_point.h"

// My Libs
#include "utility.h"

// Assets
#include "bn_sprite_items_alphabet.h"

#define TEXT_BOX_Z_ORDER -3

#define TEXT_BOX_SPAN     9
#define TEXT_BOX_ROWS     2

#define TEXT_BOX_LETTER_WIDTH  9
#define TEXT_BOX_LETTER_HEIGHT 9

#define TEXT_BOX_MAX_ANIM_FRAMES 2

///////////////////
// Enum Alphabet //
///////////////////

enum Alphabet
{
    BLANK = 0,
    SPACE,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z
};

////////////////////
// Struct TextBox //
////////////////////

struct TextBox
{

    bn::vector<bn::sprite_ptr,                                      TEXT_BOX_SPAN * TEXT_BOX_ROWS> sprite_vector;
    bn::vector<bn::sprite_animate_action<TEXT_BOX_MAX_ANIM_FRAMES>, TEXT_BOX_SPAN * TEXT_BOX_ROWS> animate_vector;

    bn::fixed_point pos;

	TextBox();
    TextBox(int32 x, int32 y);
	TextBox(const TextBox& other);
   ~TextBox();

    TextBox& operator =(const TextBox& other);

    bn::fixed_point getPos() const;

    void setPosUL(int32 new_x, int32 new_y);
    void setPosUL(bn::fixed_point new_point);

    void initSprites();
    void setSpritesFromString(const char* char_string_ptr, int32 string_size);
    void setCamera(const bn::camera_ptr& camera);
    void draw();
    void setVisible(bool is_visible);

    void updateSpritePositions();
};

#endif 

