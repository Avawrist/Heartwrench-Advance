#ifndef BN_MUSIC_ITEMS_INFO_H
#define BN_MUSIC_ITEMS_INFO_H

#include "bn_span.h"
#include "bn_music_item.h"
#include "bn_string_view.h"

namespace bn::music_items_info
{
    constexpr inline pair<music_item, string_view> array[] = {
        make_pair(music_item(0), string_view("title_screen")),
        make_pair(music_item(1), string_view("turn_on_a_chime")),
    };

    constexpr inline span<const pair<music_item, string_view>> span(array);
}

#endif

