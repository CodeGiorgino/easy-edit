#pragma once
#include "raylib.h"

#include <functional>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace gui {
    struct color_palette final {
        Color bg0 = Color { 0x1d, 0x20, 0x21, 0xff };
        Color bg1 = Color { 0x3c, 0x38, 0x36, 0xff };
        Color bg2 = Color { 0x50, 0x49, 0x45, 0xff };
        Color fg0 = Color { 0xfb, 0xf1, 0xc7, 0xff };
    };

    class component {
        public:
            Vector2 position = {};
            Vector2 size     = {};

            virtual auto bounds() const noexcept -> Rectangle = 0;
            virtual auto update()       noexcept -> void      = 0;
            virtual auto draw()   const noexcept -> void      = 0;
    };

    class flexbox : public component {
        public:
            enum class flex { COLUMN, ROW };

            struct args {
                Vector2 position;
                Vector2 size;
                flex direction;
                Vector2 padding;
                Vector2 gap;
                std::vector<std::shared_ptr<component>> items;
            };

            flex direction;
            Vector2 padding;
            Vector2 gap;

            std::vector<std::shared_ptr<component>> items = {};

            flexbox(args flexboxArgs = args {
                    .position   = Vector2 { 0, 0 },
                    .size       = Vector2 { 0, 0 },
                    .direction  = flex::COLUMN,
                    .padding    = {},
                    .gap        = {},
                    .items      = {},
                    });

            auto bounds() const noexcept -> Rectangle;
            auto update()       noexcept -> void;
            auto draw()   const noexcept -> void;
    };

    class button : public component {
        public:
            typedef std::function<void(void)> event;

            struct args {
                Vector2 position;
                Vector2 size;
                color_palette palette;
                Vector2 padding;
                std::shared_ptr<Font> font;
                std::string label;
                bool isFocused;
                event on_click;
            };

            color_palette palette;
            Vector2 padding;
            std::shared_ptr<Font> font;
            std::string label;
            bool isFocused;
            event on_click;

            button(args buttonArgs = {
                    .position   = Vector2 { 0, 0 },
                    .size       = Vector2 { 0, 0 },
                    .palette    = {},
                    .font       = std::make_shared<Font>(Font {}),
                    .label      = {},
                    .isFocused  = false,
                    .on_click   = {},
                    });

            auto bounds() const noexcept -> Rectangle;
            auto update()       noexcept -> void;
            auto draw()   const noexcept -> void;

    };

    class label : public component {
        public:
            struct args {
                Vector2 position;
                Vector2 size;
                color_palette palette;
                Vector2 padding;
                std::shared_ptr<Font> font;
                std::variant<std::string, std::shared_ptr<std::string>> text;
            };

            color_palette palette;
            Vector2 padding;
            std::shared_ptr<Font> font;
            std::variant<std::string, std::shared_ptr<std::string>> text;

            label(args labelArgs = {
                    .position   = Vector2 { 0, 0 },
                    .size       = Vector2 { 0, 0 },
                    .palette    = {},
                    .padding    = Vector2 { 0, 0 },
                    .font       = std::make_shared<Font>(Font {}),
                    .text       = {},
                    });

            auto bounds() const noexcept -> Rectangle;
            auto update()       noexcept -> void;
            auto draw()   const noexcept -> void;
    };
} // namespace gui
