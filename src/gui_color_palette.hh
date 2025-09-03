#pragma once
#include "raylib.h"

namespace gui {
    struct color_palette final {
        Color bg0 = (Color) { 0x1d, 0x20, 0x21, 0xff };
        Color bg1 = (Color) { 0x3c, 0x38, 0x36, 0xff };
        Color bg2 = (Color) { 0x50, 0x49, 0x45, 0xff };
        Color fg0 = (Color) { 0xfb, 0xf1, 0xc7, 0xff };
    };
} // namespace gui
