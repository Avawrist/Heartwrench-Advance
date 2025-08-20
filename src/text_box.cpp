#include "text_box.h"

////////////////////
// Struct TextBox //
////////////////////

TextBox::TextBox()
{
    pos = bn::fixed_point(0, 0);

    initSprites();
}

TextBox::TextBox(int32 x, int32 y)
{
    pos = bn::fixed_point(x, y);

    initSprites();
}

TextBox::TextBox(const TextBox& other)
{
    sprite_vector.clear();
    animate_vector.clear();

    for(int32 i = 0; i < other.sprite_vector.size(); i++)
    {sprite_vector.push_back(other.sprite_vector[i]);}

    for(int32 i = 0; i < other.animate_vector.size(); i++)
    {animate_vector.push_back(other.animate_vector[i]);}

    pos = other.pos;
}

TextBox::~TextBox()
{

}

TextBox& TextBox::operator =(const TextBox& other)
{
    sprite_vector.clear();
    animate_vector.clear();

    for(int32 i = 0; i < other.sprite_vector.size(); i++)
    {sprite_vector.push_back(other.sprite_vector[i]);}

    for(int32 i = 0; i < other.animate_vector.size(); i++)
    {animate_vector.push_back(other.animate_vector[i]);}

    pos = other.pos;

    return *this;
}

bn::fixed_point TextBox::getPos() const
{
    return pos;
}

void TextBox::setPosUR(int32 new_x, int32 new_y)
{
    pos.set_x(new_x);
    pos.set_y(new_y);

    updateSpritePositions();
}

void TextBox::setPosUR(bn::fixed_point new_point)
{
    pos = new_point;

    updateSpritePositions();    
}

void TextBox::initSprites()
{
    sprite_vector.clear();
    animate_vector.clear();

    for(int32 i = 0; i < sprite_vector.max_size(); i++)
    {
        sprite_vector.push_back(bn::sprite_items::alphabet.create_sprite(0, 0));
        sprite_vector.back().set_z_order(TEXT_BOX_Z_ORDER);

        animate_vector.push_back(bn::create_sprite_animate_action_once(sprite_vector[i],
								                                       0,
								                                       bn::sprite_items::alphabet.tiles_item(),
								                                       BLANK, BLANK));
    }

    updateSpritePositions();
}

void TextBox::setSpritesFromString(const char* char_string_ptr, int32 string_size)
{
    if(string_size > sprite_vector.max_size()) 
    {
        BN_LOG("Failed to all sprites - Char string oversized.");
        return;
    }

    for(int32 i = 0; i < string_size; i++)
    {
        switch(char_string_ptr[i])
        {
            case ' ':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          BLANK, BLANK);

            break;

            case '_':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          SPACE, SPACE);

            break;

            case 'A':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          A, A);

            break;

            case 'B':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          B, B);

            break;

            case 'C':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          C, C);

            break;

            case 'D':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          D, D);

            break;

            case 'E':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          E, E);

            break;

            case 'F':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          F, F);

            break;

            case 'G':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          G, G);

            break;

            case 'H':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          H, H);

            break;

            case 'I':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          I, I);

            break;

            case 'J':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          J, J);

            break;

            case 'K':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          K, K);

            break;

            case 'L':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          L, L);

            break;

            case 'M':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          M, M);

            break;

            case 'N':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          N, N);

            break;

            case 'O':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          O, O);

            break;

            case 'P':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          P, P);

            break;

            case 'Q':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          Q, Q);

            break;

            case 'R':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          R, R);

            break;

            case 'S':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          S, S);

            break;

            case 'T':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          T, T);

            break;

            case 'U':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          U, U);

            break;

            case 'V':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          V, V);

            break;

            case 'W':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          W, W);

            break;

            case 'X':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          X, X);

            break;

            case 'Y':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          Y, Y);

            break;

            case 'Z':

                animate_vector[i] = bn::create_sprite_animate_action_once(sprite_vector[i],
                                                                          0,
                                                                          bn::sprite_items::alphabet.tiles_item(),
                                                                          Z, Z);

            break;

            default:
            break;
        }
    }
}

void TextBox::setCamera(const bn::camera_ptr& camera)
{
    for(int32 i = 0; i < sprite_vector.size(); i++)
    {
        sprite_vector[i].set_camera(camera);
    }
}

void TextBox::draw()
{
    for(int32 i = 0; i < animate_vector.size(); i++)
    {
        if(!animate_vector[i].done())
        {animate_vector[i].update();}
    }
}

void TextBox::setZOrder(int32 z_order)
{
    for(int i = 0; i < sprite_vector.size(); i++)
    {
        sprite_vector[i].set_z_order(z_order);
    }
}

void TextBox::setVisible(bool is_visible)
{
    for(int i = 0; i < sprite_vector.size(); i++)
    {
        sprite_vector[i].set_visible(is_visible);
    }
}

void TextBox::updateSpritePositions()
{
    for(int32 i = 0; i < sprite_vector.size(); i++)
    {
        int32 x_offset = (sprite_vector.size() - i) * -1;
        int32 y_offset = 0;
        if(x_offset >= TEXT_BOX_SPAN) 
        {
            x_offset -= TEXT_BOX_SPAN;
            y_offset += 1;
        };

        sprite_vector[i].set_position(bn::fixed_point(pos.x() + (x_offset * TEXT_BOX_LETTER_WIDTH), 
                                                      pos.y() + (y_offset * TEXT_BOX_LETTER_HEIGHT)));
    }
}
