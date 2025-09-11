#include "gui.hh"

namespace gui {
    label::label(label::args labelArgs) :
        palette(labelArgs.palette),
        font(labelArgs.font),
        text(labelArgs.text) {
            position   = labelArgs.position;
            size       = labelArgs.size;
        }

    auto label::bounds() const noexcept -> Rectangle {
        return (Rectangle) {
            .x = position.x,
            .y = position.y,
            .width  = size.x,
            .height = size.y
        };
    }

    auto label::update() noexcept -> void { 
        return;
    }

    auto label::draw() const noexcept -> void {
        DrawTextEx(*font, text.c_str(), position, 
                font->baseSize, 1, palette.fg0);
    }
} // namespace gui
