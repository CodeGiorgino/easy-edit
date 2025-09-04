#include "enviroment.hh"
#include <cstddef>

enviroment::enviroment() {
    _fontDefault = LoadFontEx(
            "./assets/fonts/JetBrainsMono-Regular.ttf", 
            18, NULL, 0);
}

enviroment::~enviroment() {
    UnloadFont(_fontDefault);
}

auto enviroment::font() const noexcept -> Font {
    return _fontDefault;
}

auto enviroment::palette() const noexcept -> gui::color_palette {
    return _palette;
}
