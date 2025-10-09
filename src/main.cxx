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
            gui::flexbox::args {
            .size    = Vector2 { 300, 0 },
            .padding = Vector2 { 5, 5 },
            .gap     = Vector2 { 5, 5 },
            });

    auto ppathLabel = std::make_shared<std::string>(
            fs::current_path().string());
    auto pcontentLabel = std::make_shared<gui::label>(
            gui::label::args {
            .palette = palette,
            .padding = Vector2 { 5, 5 },
            .font    = font,
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
                        gui::button::args {
                        .size    = Vector2 { (*pfilesContainer).size.x, 25 },
                        .palette = accentPalette,
                        .padding = Vector2 { 5, 5 },
                        .font    = font,
                        .label   = "..",
                        .on_click = [&, folderpath]() {
                            *ppathLabel = folderpath.parent_path().string();
                            generate_file_list(folderpath.parent_path());
                        }})
            };

            const auto sortedEntries = sorted_directory_entries(folderpath);
            for (const auto& entry : sortedEntries) {
                gui::button btn(gui::button::args {
                        .size    = Vector2 { (*pfilesContainer).size.x, 25 },
                        .palette = palette,
                        .padding = Vector2 { 5, 5 },
                        .font    = font,
                        .label   = entry.path().filename(),
                        });

                if (entry.is_directory()) {
                    btn.palette = accentPalette;
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

                (*pfilesContainer).items.push_back(std::make_shared<gui::button>(btn));
            }
        };

    generate_file_list(fs::current_path());

    auto peditorSection = std::make_shared<gui::flexbox>(
            gui::flexbox::args {
            .direction = gui::flexbox::flex::ROW,
            .padding   = Vector2 { 10, 10 },
            .gap       = Vector2 { 5, 5 },
            .items = {
            pfilesContainer,
            pcontentLabel,
            }});

    auto main = gui::flexbox(
            gui::flexbox::args {
            .padding = Vector2 { 10, 10 },
            .gap     = Vector2 { 5, 5 },
            .items = {
            std::make_shared<gui::label>(
                    gui::label::args {
                    .size    = Vector2 {
                    0, static_cast<float>((*font).baseSize + 10)
                    },
                    .palette = palette,
                    .padding = Vector2 { 5, 5 },
                    .font    = font,
                    .text    = ppathLabel,
                    }),
            peditorSection
            }});

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(palette.bg0);

            main.size = Vector2 {
                static_cast<float>(GetScreenWidth()),
                static_cast<float>(GetScreenHeight())
            };

            main.update();

            (*peditorSection).size.y = main.size.y 
                - main.padding.y * 2 
                - main.gap.y
                - (*font).baseSize - 10;
            (*pcontentLabel).size.x = (*peditorSection).size.x
                - (*peditorSection).padding.x * 2
                - (*peditorSection).gap.x
                - (*pfilesContainer).size.x;

            main.draw();
        }
        EndDrawing();
    }

    CloseWindow();
    UnloadFont(*font);

    return 0;
}
