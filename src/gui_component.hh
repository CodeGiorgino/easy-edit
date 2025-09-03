#pragma once
#include "raylib.h"

namespace gui {
    class component {
        public:
            Vector2 position = {};
            Vector2 size     = {};
            bool hasChanged  = false;

            virtual auto bounds() const noexcept -> Rectangle = 0;
            virtual auto update()       noexcept -> void      = 0;
            virtual auto draw()   const noexcept -> void      = 0;
    };
} // namespace gui
