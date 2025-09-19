#ifndef BN_SOUND_ITEMS_H
#define BN_SOUND_ITEMS_H

#include "bn_sound_item.h"

namespace bn::sound_items
{
    constexpr inline sound_item bell_hit(0);
    constexpr inline sound_item checkpoint(1);
    constexpr inline sound_item cursor_move(2);
    constexpr inline sound_item gear_get(3);
    constexpr inline sound_item generic_hit(4);
    constexpr inline sound_item heart_get(5);
    constexpr inline sound_item jump(6);
    constexpr inline sound_item ow_step(24);
    constexpr inline sound_item pause(25);
    constexpr inline sound_item player_bonk(26);
    constexpr inline sound_item player_climb(27);
    constexpr inline sound_item player_death(28);
    constexpr inline sound_item player_hit(29);
    constexpr inline sound_item screw_click(30);
    constexpr inline sound_item select(31);
    constexpr inline sound_item spin_attack(32);
    constexpr inline sound_item star_jar_get(33);
    constexpr inline sound_item vase_hit(44);
    constexpr inline sound_item wall_hit(45);
}

#endif

