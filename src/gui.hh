#pragma once
#include "raylib.h"

#include <string>
#include <memory>
#include <vector>

namespace gui {
    struct color_palette final {
        Color bg0 = (Color) { 0x1d, 0x20, 0x21, 0xff };
        Color bg1 = (Color) { 0x3c, 0x38, 0x36, 0xff };
        Color bg2 = (Color) { 0x50, 0x49, 0x45, 0xff };
        Color fg0 = (Color) { 0xfb, 0xf1, 0xc7, 0xff };
    };

    class component {
        public:
            Vector2 position = {};
            Vector2 size     = {};
            bool hasChanged  = false;

            virtual auto bounds() const noexcept -> Rectangle = 0;
            virtual auto update()       noexcept -> void      = 0;
            virtual auto draw()   const noexcept -> void      = 0;
    };

    class flexbox : public component {
        public:
            enum class flex { COLUMN, ROW };

            flex direction = flex::COLUMN;
            Vector2 gap = {};

            std::vector<std::shared_ptr<component>> items = {};

            auto bounds() const noexcept -> Rectangle;
            auto load_texture() noexcept -> void;
            auto update()       noexcept -> void;
            auto draw()   const noexcept -> void;

        private:
            RenderTexture2D _targetTexture = {};
    };

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

    class label : public component {
        public:
            color_palette palette = {};
            std::shared_ptr<Font> font = std::make_shared<Font>(Font {});
            std::string text {};

            auto bounds() const noexcept -> Rectangle;
            auto update()       noexcept -> void;
            auto draw()   const noexcept -> void;
    };
} // namespace gui
