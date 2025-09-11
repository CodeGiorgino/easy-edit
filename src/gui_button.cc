#include "gui.hh"

namespace gui {
    button::button(button::args buttonArgs) :
        palette(buttonArgs.palette),
        padding(buttonArgs.padding),
        font(buttonArgs.font),
        label(buttonArgs.label),
        isFocused(buttonArgs.isFocused),
        on_click(buttonArgs.on_click) {
            position   = buttonArgs.position;
            size       = buttonArgs.size;
        }

    auto button::bounds() const noexcept -> Rectangle {
        return (Rectangle) {
            .x = position.x,
            .y = position.y,
            .width  = size.x,
            .height = size.y
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
        DrawRectangleV(position, size, isFocused 
                ? palette.bg2 : palette.bg1);

        DrawTextEx(*font, label.c_str(), (Vector2) {
                    position.x + padding.x,
                    position.y + padding.y
                }, font->baseSize, 1, palette.fg0);
    }
} // namespace gui
