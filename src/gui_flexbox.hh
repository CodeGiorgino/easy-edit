#pragma once
#include "gui_component.hh"
#include "raylib.h"

#include <memory>
#include <vector>

namespace gui {
    class flexbox : public component {
        public:
            enum class flex { COLUMN, ROW };
            flex direction = flex::COLUMN;

            Vector2 gap = {};

            std::vector<std::unique_ptr<component>> items = {};

            auto bounds() const noexcept -> Rectangle;
            auto load_texture() noexcept -> void;
            auto update()       noexcept -> void;
            auto draw()   const noexcept -> void;

        private:
            RenderTexture2D _targetTexture = {};
    };
} // namespace gui
