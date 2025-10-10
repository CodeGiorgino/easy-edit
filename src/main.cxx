#include "gui.hxx"
#include "raylib.h"

#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
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
    constexpr auto windowSizeDefault = Vector2 { 1280, 720 };
    
    InitWindow(windowSizeDefault.x, windowSizeDefault.y, "easyedit");
    SetWindowPosition(
            GetScreenWidth()  * 0.5 - windowSizeDefault.x * 0.5, 
            GetScreenHeight() * 0.5 - windowSizeDefault.y * 0.5);

    gui::color_palette palette;
    auto font = std::make_shared<Font>(
            LoadFontEx(
                "./assets/fonts/JetBrainsMono-Regular.ttf", 
                18, NULL, 0));

    auto pfilesContainer = std::make_shared<gui::flexbox>(
            gui::component_style {
            .size = Vector2 { 300, 0 },
            .gap  = 5,
            });

    auto ppathLabel = std::make_shared<std::string>(
            fs::current_path().string());
    auto pcontentLabel = std::make_shared<gui::label>(
            gui::component_style {
            .palette = palette,
            .font    = font,
            .padding = Vector2 { 5, 5 },
            });

    const auto set_editor_content =
        [&](fs::path filepath) -> void {
            std::ifstream stream(filepath);
            if (!stream) {
                // TODO: create file or report error (ex. already a directory)
                return;
            }

            (*pcontentLabel).text = std::string(
                    std::istreambuf_iterator<char>(stream),
                    std::istreambuf_iterator<char>());
        };

    const std::function<void(fs::path)> generate_file_list = 
        [&](fs::path folderpath) -> void {
            constexpr auto accentPalette = gui::color_palette {
                .fg0 = Color { 0xd6, 0x5d, 0x0e, 0xff }
            };

            (*pfilesContainer).items = {
                std::make_shared<gui::button>(
                        gui::component_style {
                        .palette = accentPalette,
                        .font    = font,
                        .size    = Vector2 { (*pfilesContainer).style.size.x, 25 },
                        .padding = Vector2 { 5, 5 },
                        },
                        "..", [&, folderpath]() {
                            *ppathLabel = folderpath.parent_path().string();
                            generate_file_list(folderpath.parent_path());
                        })
            };

            const auto sortedEntries = sorted_directory_entries(folderpath);
            for (const auto& entry : sortedEntries) {
                gui::button btn(gui::component_style {
                        .palette = palette,
                        .font    = font,
                        .size    = Vector2 { (*pfilesContainer).style.size.x, 25 },
                        .padding = Vector2 { 5, 5 },
                        }, entry.path().filename());

                if (entry.is_directory()) {
                    btn.style.palette = accentPalette;
                    btn.on_click = [&, entry]() {
                        *ppathLabel = entry.path().string();
                        generate_file_list(entry.path());
                    };
                } else if (entry.is_regular_file()) {
                    btn.on_click = [&, entry]() {
                        *ppathLabel = entry.path().string();
                        set_editor_content(entry.path());
                    };
                }

                (*pfilesContainer).items.push_back(
                        std::make_shared<gui::button>(btn));
            }
        };

    generate_file_list(fs::current_path());

    auto peditorSection = std::make_shared<gui::flexbox>(
            gui::component_style { .gap = 5, },
            gui::flexbox::flex::ROW,
            gui::flexbox::items_collection { pfilesContainer, pcontentLabel });

    auto main = gui::flexbox(
            gui::component_style {
            .padding = Vector2 { 10, 10 },
            .gap     = 5,
            },
            gui::flexbox::flex::COLUMN,
            {
            std::make_shared<gui::label>(
                    gui::component_style {
                    .palette = palette,
                    .font    = font,
                    .size    = Vector2 { 0, static_cast<float>((*font).baseSize + 10) },
                    .padding = Vector2 { 5, 5 },
                    },
                    ppathLabel),
            peditorSection
            });

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(palette.bg0);

            main.style.size = Vector2 {
                static_cast<float>(GetScreenWidth()),
                static_cast<float>(GetScreenHeight())
            };

            main.update();

            (*peditorSection).style.size.y = main.style.size.y 
                - main.style.padding.y * 2 
                - main.style.gap
                - (*font).baseSize - 10;
            (*pcontentLabel).style.size.x = (*peditorSection).style.size.x
                - (*peditorSection).style.padding.x * 2
                - (*peditorSection).style.gap
                - (*pfilesContainer).style.size.x;

            main.draw();
        }
        EndDrawing();
    }

    CloseWindow();
    UnloadFont(*font);

    return 0;
}
