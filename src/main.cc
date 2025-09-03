#include <iostream>
#include <filesystem>
#include <memory>
#include <print>
#include <vector>

#include "raylib.h"

namespace fs = std::filesystem;

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

            virtual auto get_size()   const noexcept -> Vector2   = 0;
            virtual auto get_bounds() const noexcept -> Rectangle = 0;
            virtual auto update()           noexcept -> void      = 0;
            virtual auto draw()       const noexcept -> void      = 0;
    };

    class button : public component {
        public:
            std::string label = {};
            Vector2 padding   = {};
            int fontSize = 10;
            color_palette palette = {};
            bool isFocused = false;

            auto get_size() const noexcept -> Vector2 {
                return size;
            }

            auto get_bounds() const noexcept -> Rectangle {
                return (Rectangle) {
                    .x = position.x,
                    .y = position.y,
                    .width  = size.x,
                    .height = size.y
                };
            }

            auto update() noexcept -> void {
                const auto isMouseColliding = 
                    CheckCollisionPointRec(GetMousePosition(), get_bounds());

                hasChanged = isMouseColliding != isFocused;
                isFocused = isMouseColliding;
            }

            auto draw() const noexcept -> void {
                DrawRectangleV(
                    position, 
                    size, 
                    isFocused ? palette.bg2 : palette.bg1);

                DrawText(label.c_str(),
                    position.x + padding.x,
                    position.y + padding.y,
                    fontSize,
                    palette.fg0);
            }
    };

    class flexbox : public component {
        public:
            enum class flex { COLUMN, ROW };

            Vector2 gap = {};
            flex direction = flex::COLUMN;
            std::vector<std::unique_ptr<component>> items = {};

            auto get_size() const noexcept -> Vector2 {
                // TODO: calculate real size
                return size;
            }

            auto get_bounds() const noexcept -> Rectangle {
                // TODO: calculate real bounds
                return (Rectangle) {
                    .x = position.x,
                    .y = position.y,
                    .width  = size.x,
                    .height = size.y
                };
            }

            auto load_texture() noexcept -> void {
                _targetTexture = LoadRenderTexture(size.x, size.y);
                BeginTextureMode(_targetTexture);
                {
                    auto xOffset = 0.f;
                    auto yOffset = 0.f;

                    ClearBackground(BLANK);
                    for (auto& item : items) {
                        const auto itemSize = item->get_size();
                        item->position.x = xOffset;
                        item->position.y = yOffset;
                        item->draw();

                        if (direction == flex::COLUMN) {
                            yOffset += itemSize.y + gap.y;
                        } else if (direction == flex::ROW) {
                            xOffset += itemSize.x + gap.x;
                        }
                    }
                }
                EndTextureMode();
            }

            auto update() noexcept -> void {
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

            auto draw() const noexcept -> void {
                DrawTextureRec(
                    _targetTexture.texture,
                    (Rectangle) { 0, 0, size.x, -size.y },
                    position,
                    WHITE);
            }

        private:
            RenderTexture2D _targetTexture = {};
    };
} // namespace gui

int main(void)
{
    const gui::color_palette paletteDefault;
    constexpr auto windowPadding = 10.f;

    gui::flexbox sidebar;
    sidebar.position = (Vector2) { windowPadding, windowPadding };
    sidebar.size = (Vector2) { 200, -1 };
    sidebar.gap  = (Vector2) { windowPadding, windowPadding };

    for (const auto& entry : fs::directory_iterator(fs::path("."))) {
        gui::button btn;
        btn.size     = (Vector2) { sidebar.size.x, 30 };
        btn.label    = entry.path().filename();
        btn.padding  = (Vector2) { windowPadding, windowPadding };

        if (entry.is_directory()) {
            btn.palette.fg0 = (Color) { 0xd6, 0x5d, 0x0e, 0xff };
        }

        sidebar.items.push_back(std::make_unique<gui::button>(btn));
    }

    InitWindow(800, 450, "notes");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(paletteDefault.bg0);

            const auto width = GetScreenWidth();
            const auto height = GetScreenHeight();
            const auto sidebarHeight = 
                static_cast<float>(height - windowPadding * 2);

            if (sidebarHeight != sidebar.size.y) {
                sidebar.size.y = sidebarHeight;
                sidebar.hasChanged = true;
            }

            sidebar.draw();
            sidebar.update();
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
