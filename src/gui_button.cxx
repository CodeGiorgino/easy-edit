#include "gui.hxx"

namespace gui {
    button::button(component_style _style,
            std::string _label,
            event _on_click) :
        label(_label),
        on_click(_on_click) {
            style = _style;
        }

    auto button::bounds() const noexcept -> Rectangle {
        return (Rectangle) {
            .x = style.position.x,
            .y = style.position.y,
            .width  = style.size.x,
            .height = style.size.y
        };
    }

    auto button::update() noexcept -> void {
        isFocused = CheckCollisionPointRec(
                GetMousePosition(),
                bounds());

        if (isFocused 
                && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
                && on_click) {
            on_click();
        }
    }

    auto button::draw() const noexcept -> void {
        DrawRectangleV(style.position, style.size,
                isFocused ? style.palette.bg2 : style.palette.bg1);
        DrawTextEx(*style.font, label.c_str(), Vector2 {
                    style.position.x + style.padding.x,
                    style.position.y + style.padding.y
                }, (*style.font).baseSize, 1, style.palette.fg0);
    }
} // namespace gui
