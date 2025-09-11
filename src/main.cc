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
    constexpr auto windowSizeDefault = (Vector2) { 1920, 1080 };
    
    InitWindow(windowSizeDefault.x, windowSizeDefault.y, "EasyEdit");
    SetWindowPosition(
            GetMonitorWidth(0)  * 0.5 - windowSizeDefault.x * 0.5, 
            GetMonitorHeight(0) * 0.5 - windowSizeDefault.y * 0.5);

    fs::path editorFolder(fs::current_path());
    gui::color_palette palette;
    auto font = std::make_shared<Font>(
            LoadFontEx(
                "./assets/fonts/JetBrainsMono-Regular.ttf", 
                18, NULL, 0));

    gui::flexbox main;
    main.padding = (Vector2) { 10, 10 };
    main.gap     = (Vector2) { 0, 5 };

    gui::label pathLabel;
    pathLabel.font    = font; 
    pathLabel.size    = (Vector2) { 0, 30 };
    pathLabel.text    = editorFolder.string();
    pathLabel.palette = palette;

    gui::flexbox content;
    content.direction = gui::flexbox::flex::ROW;
    content.gap       = (Vector2) { 0, 10 };

    gui::flexbox filelist;
    filelist.size = (Vector2) { 300, 0 };
    filelist.gap  = (Vector2) { 0, 5 };

    const auto sortedEntries = sorted_directory_entries(editorFolder);
    for (const auto& entry : sortedEntries) {
        gui::button btn;
        btn.size    = (Vector2) { filelist.size.x, 25 };
        btn.padding = (Vector2) { 5, 5 };
        btn.font    = font;
        btn.label   = entry.path().filename();
        btn.palette = palette;

        if (entry.is_directory()) {
            btn.palette.fg0 = (Color) { 0xd6, 0x5d, 0x0e, 0xff };
        }

        filelist.items.push_back(std::make_shared<gui::button>(btn));
    }

    content.items = { std::make_shared<gui::flexbox>(filelist) };
    main.items = {
        std::make_shared<gui::label>(pathLabel),
        std::make_shared<gui::flexbox>(content),
    };

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
                content.size.x = mainWidth - main.padding.x;
                main.hasChanged  = true;
            }

            if (mainHeight != main.size.y) {
                main.size.y = mainHeight;
                content.size.y = mainHeight - main.padding.y;
                filelist.size.y = content.size.y - 40;
                main.hasChanged  = true;
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
