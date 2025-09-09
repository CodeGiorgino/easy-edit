#include "gui.hh"
#include "raylib.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <print>
#include <ranges>
#include <set>

namespace fs = std::filesystem;

auto sorted_directory_entries(fs::path folderpath) -> std::vector<fs::directory_entry> {
    static constexpr auto lexComparator = 
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
    constexpr auto monitorSize       = (Vector2) { 2560, 1440 };
    constexpr auto windowSizeDefault = (Vector2) { 1920, 1080 };
    
    InitWindow(windowSizeDefault.x, windowSizeDefault.y, "EasyEdit");
    SetWindowPosition(
            monitorSize.x * 0.5 - windowSizeDefault.x * 0.5,
            monitorSize.y * 0.5 - windowSizeDefault.y * 0.5);

    fs::path editorFolder(fs::current_path());
    gui::color_palette palette;
    auto font = LoadFontEx(
            "./assets/fonts/JetBrainsMono-Regular.ttf", 
            18, NULL, 0);

    gui::flexbox content;
    content.position = (Vector2) { 10, 10 };
    content.gap      = (Vector2) { 0, 10 };

    // TODO: define a constructor with all default values for easier initialisation
    gui::label pathLabel;
    pathLabel.text    = editorFolder.string();
    pathLabel.palette = palette;

    gui::flexbox fileList;
    fileList.size = (Vector2) { 300, 0 };
    fileList.gap  = (Vector2) { 0, 5 };

    const auto sortedEntries = sorted_directory_entries(editorFolder);
    for (const auto& entry : sortedEntries) {
        gui::button btn;
        btn.size    = (Vector2) { 0, 25 };
        btn.padding = (Vector2) { 5, 5 };
        btn.font    = std::make_shared<Font>(font);
        btn.label   = entry.path().filename();
        btn.palette = palette;

        if (entry.is_directory()) {
            btn.palette.fg0 = (Color) { 0xd6, 0x5d, 0x0e, 0xff };
        }

        fileList.items.push_back(std::make_shared<gui::button>(btn));
    }

    content.items = {
        std::make_shared<gui::label>(pathLabel),
        std::make_shared<gui::flexbox>(fileList)
    };

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(palette.bg0);

            const auto contentHeight = 
                static_cast<float>(GetScreenHeight() - 45);

            if (contentHeight != content.size.y) {
                content.size.y = contentHeight;
                content.hasChanged = true;
            }

            content.draw();
            content.update();
        }
        EndDrawing();
    }

    CloseWindow();
    UnloadFont(font);

    return 0;
}
