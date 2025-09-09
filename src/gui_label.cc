#include "gui.hh"

namespace gui {
    auto label::bounds() const noexcept -> Rectangle {
        return (Rectangle) {
            .x = position.x,
            .y = position.y,
            .width  = size.x,
            .height = size.y
        };
    }

    auto label::update() noexcept -> void { 
        hasChanged = false;
        return;
    }


    auto label::draw() const noexcept -> void {
        DrawTextEx(*font, text.c_str(), (Vector2) {
                    position.x,
                    position.y
                }, font->baseSize, 1, palette.fg0);
    }
} // namespace gui
