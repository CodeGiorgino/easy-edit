#pragma once
#include <cstdint>
#include <string>

#include "gui_color_palette.hh"
#include "gui_component.hh"

namespace gui {
    class button : public component {
        public:
            color_palette palette = {};
            Vector2 padding = {};

            uint8_t fontSize  = 10;
            std::string label = {};

            bool isFocused = false;

            auto bounds() const noexcept -> Rectangle;
            auto update()       noexcept -> void;
            auto draw()   const noexcept -> void;
    };
} // namespace gui
