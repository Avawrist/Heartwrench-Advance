#ifndef BN_SOUND_ITEMS_INFO_H
#define BN_SOUND_ITEMS_INFO_H

#include "bn_span.h"
#include "bn_sound_item.h"
#include "bn_string_view.h"

namespace bn::sound_items_info
{
    constexpr inline pair<sound_item, string_view> array[] = {
        make_pair(sound_item(0), string_view("bell_hit")),
        make_pair(sound_item(1), string_view("checkpoint")),
        make_pair(sound_item(2), string_view("cursor_move")),
        make_pair(sound_item(3), string_view("gear_get")),
        make_pair(sound_item(4), string_view("generic_hit")),
        make_pair(sound_item(5), string_view("heart_get")),
        make_pair(sound_item(6), string_view("jump")),
        make_pair(sound_item(24), string_view("ow_step")),
        make_pair(sound_item(25), string_view("pause")),
        make_pair(sound_item(26), string_view("player_bonk")),
        make_pair(sound_item(27), string_view("player_climb")),
        make_pair(sound_item(28), string_view("player_death")),
        make_pair(sound_item(29), string_view("player_hit")),
        make_pair(sound_item(30), string_view("screw_click")),
        make_pair(sound_item(31), string_view("select")),
        make_pair(sound_item(32), string_view("spin_attack")),
        make_pair(sound_item(33), string_view("star_jar_get")),
        make_pair(sound_item(44), string_view("vase_hit")),
        make_pair(sound_item(45), string_view("wall_hit")),
    };

    constexpr inline span<const pair<sound_item, string_view>> span(array);
}

#endif

