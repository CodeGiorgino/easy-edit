#include "gui.hh"

namespace gui {
    auto flexbox::bounds() const noexcept -> Rectangle {
        return (Rectangle) {
            .x = position.x,
            .y = position.y,
            .width  = size.x,
            .height = size.y
        };
    }

    auto flexbox::update() noexcept -> void {
        bool childHasChanged = false;
        auto xOffset = position.x + padding.x;
        auto yOffset = position.y + padding.y;

        for (auto& item : items) {
            (*item).position.x = xOffset;
            (*item).position.y = yOffset;

            if (direction == flex::COLUMN) {
                yOffset += (*item).size.y + gap.y;
            } else if (direction == flex::ROW) {
                xOffset += (*item).size.x + gap.x;
            }

            (*item).hasChanged |= hasChanged;
            (*item).update();

            if ((*item).hasChanged) {
                childHasChanged = true;
            }
        }

        hasChanged = childHasChanged;
    }

    auto flexbox::draw() const noexcept -> void {
        for (auto& item : items) {
            (*item).draw();
        }
    }
} // namespace gui
