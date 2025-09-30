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
        make_pair(sound_item(3), string_view("gate_open")),
        make_pair(sound_item(4), string_view("gear_get")),
        make_pair(sound_item(5), string_view("gear_spend")),
        make_pair(sound_item(6), string_view("generic_hit")),
        make_pair(sound_item(7), string_view("heart_get")),
        make_pair(sound_item(8), string_view("jump")),
        make_pair(sound_item(9), string_view("ow_step")),
        make_pair(sound_item(10), string_view("pause")),
        make_pair(sound_item(11), string_view("player_bonk")),
        make_pair(sound_item(12), string_view("player_climb")),
        make_pair(sound_item(13), string_view("player_death")),
        make_pair(sound_item(14), string_view("player_hit")),
        make_pair(sound_item(15), string_view("player_wrench_clamp")),
        make_pair(sound_item(16), string_view("player_wrench_pump")),
        make_pair(sound_item(17), string_view("screw_click")),
        make_pair(sound_item(18), string_view("select")),
        make_pair(sound_item(19), string_view("spin_attack")),
        make_pair(sound_item(20), string_view("star_jar_get")),
        make_pair(sound_item(52), string_view("vase_hit")),
        make_pair(sound_item(53), string_view("wall_hit")),
    };

    constexpr inline span<const pair<sound_item, string_view>> span(array);
}

#endif

