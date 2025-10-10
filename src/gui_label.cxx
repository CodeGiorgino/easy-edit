#include "gui.hxx"

namespace gui {
    label::label(component_style _style,
            std::variant<std::string, std::shared_ptr<std::string>> _text) :
        text(_text) {
            style = _style;
        }

    auto label::bounds() const noexcept -> Rectangle {
        return (Rectangle) {
            .x = style.position.x,
            .y = style.position.y,
            .width  = style.size.x,
            .height = style.size.y
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
                *style.font, value.c_str(), (*style.font).baseSize, 1);

        DrawRectangleV(style.position, style.size, style.palette.bg1);
        DrawTextEx(*style.font, value.c_str(), Vector2 {
                    style.position.x + style.padding.x,
                    style.position.y + style.padding.y
                }, (*style.font).baseSize, 1, style.palette.fg0);
    }
} // namespace gui
