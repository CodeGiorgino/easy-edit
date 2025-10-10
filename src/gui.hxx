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

    struct component_style final {
        color_palette palette = {};
        std::shared_ptr<Font> font = {};
        Vector2 position = {};
        Vector2 size     = {};
        Vector2 padding  = {};
        int gap = 0;
    };

    class component {
        public:
            component_style style = {};

            virtual auto bounds() const noexcept -> Rectangle = 0;
            virtual auto update()       noexcept -> void      = 0;
            virtual auto draw()   const noexcept -> void      = 0;
    };

    class flexbox : public component {
        public:
            enum class flex { COLUMN, ROW };
            flex direction;

            typedef std::vector<std::shared_ptr<component>> items_collection;
            items_collection items = {};

            flexbox(component_style style = {},
                    flex direction = flex::COLUMN,
                    std::vector<std::shared_ptr<component>> items = {});

            auto bounds() const noexcept -> Rectangle;
            auto update()       noexcept -> void;
            auto draw()   const noexcept -> void;
    };

    class button : public component {
        public:
            typedef std::function<void(void)> event;
            event on_click;

            std::string label;
            bool isFocused;

            button(component_style style = {},
                    std::string label    = {},
                    event on_click       = {});

            auto bounds() const noexcept -> Rectangle;
            auto update()       noexcept -> void;
            auto draw()   const noexcept -> void;

    };

    class label : public component {
        public:
            std::variant<std::string, std::shared_ptr<std::string>> text;

            label(component_style style = {},
                    std::variant<std::string, std::shared_ptr<std::string>> text = {});

            auto bounds() const noexcept -> Rectangle;
            auto update()       noexcept -> void;
            auto draw()   const noexcept -> void;
    };
} // namespace gui
