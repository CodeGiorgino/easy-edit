#pragma once
#include "gui_color_palette.hh"
#include "raylib.h"

class enviroment final {
    public:
        enviroment();
        ~enviroment();

        auto font()    const noexcept -> Font;
        auto palette() const noexcept -> gui::color_palette;

    private:
        Font _fontDefault = {};
        gui::color_palette _palette = {};
};
