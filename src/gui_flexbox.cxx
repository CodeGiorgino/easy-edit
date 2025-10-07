#include "gui.hxx"

namespace gui {
    flexbox::flexbox(flexbox::args flexboxArgs) :
        direction(flexboxArgs.direction),
        padding(flexboxArgs.padding),
        gap(flexboxArgs.gap),
        items(flexboxArgs.items) {
            position   = flexboxArgs.position;
            size       = flexboxArgs.size;
        }

    auto flexbox::bounds() const noexcept -> Rectangle {
        return (Rectangle) {
            .x = position.x,
            .y = position.y,
            .width  = size.x,
            .height = size.y
        };
    }

    auto flexbox::update() noexcept -> void {
        auto xOffset = position.x + padding.x;
        auto yOffset = position.y + padding.y;

        for (auto& pitem : items) {
            (*pitem).position.x = xOffset;
            (*pitem).position.y = yOffset;

            if (direction == flex::COLUMN) {
                yOffset += (*pitem).size.y + gap.y;
            } else if (direction == flex::ROW) {
                xOffset += (*pitem).size.x + gap.x;
            }

            (*pitem).update();
        }
    }

    auto flexbox::draw() const noexcept -> void {
        for (auto& pitem : items) {
            (*pitem).draw();
        }
    }
} // namespace gui
