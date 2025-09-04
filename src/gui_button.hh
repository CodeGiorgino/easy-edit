#pragma once
#include <string>
#include <memory>

#include "gui_color_palette.hh"
#include "gui_component.hh"
#include "raylib.h"

namespace gui {
    class button : public component {
        public:
            color_palette palette = {};
            Vector2 padding = {};

            std::shared_ptr<Font> font = std::make_shared<Font>(Font {});
            std::string label = {};

            bool isFocused = false;

            auto bounds() const noexcept -> Rectangle;
            auto update()       noexcept -> void;
            auto draw()   const noexcept -> void;
    };
} // namespace gui
