#include "gui.hxx"

namespace gui {
    flexbox::flexbox(component_style _style,
            flex _direction,
            std::vector<std::shared_ptr<component>> _items) :
        direction(_direction),
        items(_items) {
            style = _style;
        }

    auto flexbox::bounds() const noexcept -> Rectangle {
        return (Rectangle) {
            .x = style.position.x,
            .y = style.position.y,
            .width  = style.size.x,
            .height = style.size.y
        };
    }

    auto flexbox::update() noexcept -> void {
        auto xOffset = style.position.x + style.padding.x;
        auto yOffset = style.position.y + style.padding.y;

        for (auto& pitem : items) {
            (*pitem).style.position.x = xOffset;
            (*pitem).style.position.y = yOffset;

            if (direction == flex::COLUMN) {
                yOffset += (*pitem).style.size.y + style.gap;
                (*pitem).style.size.x = style.size.x - style.padding.x * 2;
            } else if (direction == flex::ROW) {
                xOffset += (*pitem).style.size.x + style.gap;
                (*pitem).style.size.y = style.size.y - style.padding.y * 2;
            }

            (*pitem).update();
        }
    }

    auto flexbox::draw() const noexcept -> void {
        // DrawRectangleLines(position.x, position.y, size.x, size.y, RED);
        for (auto& pitem : items) {
            (*pitem).draw();
        }
    }
} // namespace gui
