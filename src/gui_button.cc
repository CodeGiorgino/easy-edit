#include "gui.hh"

namespace gui {
    button::button(button::args buttonArgs) :
        palette(buttonArgs.palette),
        font(buttonArgs.font),
        label(buttonArgs.label),
        isFocused(buttonArgs.isFocused) {
            position   = buttonArgs.position;
            size       = buttonArgs.size;
            hasChanged = buttonArgs.hasChanged;
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
        const auto isMouseColliding = 
            CheckCollisionPointRec(GetMousePosition(), bounds());

        hasChanged = isMouseColliding != isFocused;
        isFocused  = isMouseColliding;
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
