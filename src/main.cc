#include "gui.hh"
#include "raylib.h"

#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <print>
#include <ranges>
#include <set>

namespace fs = std::filesystem;

auto sorted_directory_entries(fs::path folderpath) -> std::vector<fs::directory_entry> {
    constexpr auto lexComparator = 
        [](fs::directory_entry a, fs::directory_entry b) {
            return a.path().filename().string().compare(
                    b.path().filename().string()) < 0;
        };

    if (!fs::exists(folderpath) || !fs::is_directory(folderpath)) throw
        std::invalid_argument("Directory not found.");

    std::set<fs::directory_entry, decltype(lexComparator)> sortedEntries = {};
    for (const auto& entry : fs::directory_iterator(folderpath)) {
        sortedEntries.insert(entry);
    }

    return sortedEntries
        | std::ranges::to<std::vector>();
}

int main(void)
{
    // TODO: move to config files
    // TODO: dimensions relative to font size
    constexpr auto windowSizeDefault = Vector2 { 1920, 1080 };
    
    InitWindow(windowSizeDefault.x, windowSizeDefault.y, "EasyEdit");
    SetWindowPosition(
            GetMonitorWidth(0)  * 0.5 - windowSizeDefault.x * 0.5, 
            GetMonitorHeight(0) * 0.5 - windowSizeDefault.y * 0.5);

    gui::color_palette palette;
    auto font = std::make_shared<Font>(
            LoadFontEx(
                "./assets/fonts/JetBrainsMono-Regular.ttf", 
                18, NULL, 0));

    auto filesContainer = std::make_shared<gui::flexbox>(
            gui::flexbox::args {
            .size = Vector2 { 300, 0 },
            .gap  = Vector2 { 0, 5 },
            });

    auto pfolderpath = std::make_shared<std::string>();
    const std::function<void(fs::path)> generate_file_list = 
        [&](fs::path folderpath) {
            *pfolderpath = std::format("Folder: {:?}", folderpath.string());
            constexpr auto accentPalette = gui::color_palette {
                .fg0 = Color { 0xd6, 0x5d, 0x0e, 0xff }
            };

            (*filesContainer).items = {
                std::make_shared<gui::button>(
                        gui::button::args {
                        .size    = Vector2 { (*filesContainer).size.x, 25 },
                        .palette = accentPalette,
                        .padding = Vector2 { 5, 5 },
                        .font    = font,
                        .label   = "..",
                        .on_click = [&, folderpath]() {
                            generate_file_list(folderpath.parent_path());
                        }})
            };

            const auto sortedEntries = sorted_directory_entries(folderpath);
            for (const auto& entry : sortedEntries) {
                gui::button btn(gui::button::args {
                        .size    = Vector2 { (*filesContainer).size.x, 25 },
                        .palette = palette,
                        .padding = Vector2 { 5, 5 },
                        .font    = font,
                        .label   = entry.path().filename(),
                        });

                if (entry.is_directory()) {
                    btn.palette = accentPalette;
                    btn.on_click = [&, entry]() {
                        generate_file_list(entry.path());
                    };
                }

                (*filesContainer).items.push_back(std::make_shared<gui::button>(btn));
            }
        };

    generate_file_list(fs::current_path());

    auto main = gui::flexbox(
            gui::flexbox::args {
            .padding = Vector2 { 10, 10 },
            .gap     = Vector2 { 0, 5 },
            .items = {
            std::make_shared<gui::label>(
                    gui::label::args {
                    .size    = Vector2 { 0, 30 },
                    .palette = palette,
                    .font    = font,
                    .text    = pfolderpath,
                    }),
            std::make_shared<gui::flexbox>(
                    gui::flexbox::args {
                    .direction = gui::flexbox::flex::ROW,
                    .gap       = Vector2 { 0, 10 },
                    .items = {
                    filesContainer,
                    // TODO: editor
                    }}),
            }});

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(palette.bg0);

            const auto mainWidth =
                static_cast<float>(GetScreenWidth() - 20);
            const auto mainHeight = 
                static_cast<float>(GetScreenHeight() - 20);

            if (mainWidth != main.size.x) {
                main.size.x = mainWidth;
            }

            if (mainHeight != main.size.y) {
                main.size.y = mainHeight;
            }

            main.draw();
            main.update();
        }
        EndDrawing();
    }

    CloseWindow();
    UnloadFont(*font);

    return 0;
}
