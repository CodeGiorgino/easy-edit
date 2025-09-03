#include "gui_flexbox.hh"

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
                item->position.x = xOffset;
                item->position.y = yOffset;
                item->draw();

                if (direction == flex::COLUMN) {
                    yOffset += item->size.y + gap.y;
                } else if (direction == flex::ROW) {
                    xOffset += item->size.x + gap.x;
                }
            }
        }
        EndTextureMode();
    }

    auto flexbox::update() noexcept -> void {
        for (auto& item : items) {
            item->update();
            if (item->hasChanged) {
                hasChanged = true;
            }
        }

        if (hasChanged) {
            load_texture();
        }

        hasChanged = false;
    }

    auto flexbox::draw() const noexcept -> void {
        DrawTextureRec(
                _targetTexture.texture,
                (Rectangle) { 0, 0, size.x, -size.y },
                position,
                WHITE);
    }
} // namespace gui
