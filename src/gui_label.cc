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
    }

    auto label::draw() const noexcept -> void {
            TraceLog(LOG_INFO, "label size: (%f, %f)", size.x, size.y);
        DrawTextEx(*font, text.c_str(), position, 
                font->baseSize, 1, palette.fg0);
    }
} // namespace gui
