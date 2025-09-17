#include "gui.hh"

namespace gui {
    label::label(label::args labelArgs) :
        palette(labelArgs.palette),
        padding(labelArgs.padding),
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
        std::string value {};
        if (const auto pvalue = std::get_if<std::string>(&text)) {
            value = *pvalue;
        } else {
            value = *std::get<std::shared_ptr<std::string>>(text);
        }

        const auto textSize = MeasureTextEx(
                *font, value.c_str(), (*font).baseSize, 1);

        DrawRectangleV(position, size, palette.bg1);
        DrawTextEx(*font, value.c_str(), Vector2 {
                    position.x + padding.x,
                    position.y + padding.y
                }, (*font).baseSize, 1, palette.fg0);
    }
} // namespace gui
