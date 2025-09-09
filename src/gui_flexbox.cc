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

    auto flexbox::load_texture() noexcept -> void {
        _targetTexture = LoadRenderTexture(size.x, size.y);
        BeginTextureMode(_targetTexture);
        {
            auto xOffset = 0.f;
            auto yOffset = 0.f;

            ClearBackground(BLANK);
            for (auto& item : items) {
                (*item).position.x = xOffset;
                (*item).position.y = yOffset;

                if ((*item).size.x == 0) {
                    (*item).size.x = size.x;
                }

                if ((*item).size.y == 0) {
                    (*item).size.y = size.y;
                }

                (*item).draw();

                if (direction == flex::COLUMN) {
                    yOffset += (*item).size.y + gap.y;
                } else if (direction == flex::ROW) {
                    xOffset += (*item).size.x + gap.x;
                }
            }
        }
        EndTextureMode();
    }

    auto flexbox::update() noexcept -> void {
        bool childHasChanged = false;
        for (auto& item : items) {
            (*item).hasChanged |= hasChanged;
            (*item).update();
            if ((*item).hasChanged) {
                childHasChanged = true;
            }
        }

        if (childHasChanged) {
            load_texture();
        }

        hasChanged = childHasChanged;
    }

    auto flexbox::draw() const noexcept -> void {
        DrawTextureRec(
                _targetTexture.texture,
                (Rectangle) { 0, 0, size.x, -size.y },
                position,
                WHITE);
    }
} // namespace gui
