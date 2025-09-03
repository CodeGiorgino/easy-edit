#include "gui_button.hh"
#include "gui_flexbox.hh"
#include "gui_color_palette.hh"
#include "raylib.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <print>
#include <ranges>
#include <set>
#include <vector>

namespace fs = std::filesystem;

auto sorted_directory_entries(fs::path folderpath) -> std::vector<fs::directory_entry> {
    static constexpr auto lexComparator = [](fs::directory_entry a, fs::directory_entry b) {
        return a.path().filename().string().compare(
                b.path().filename().string());
    };

    std::set<fs::directory_entry, decltype(lexComparator)> sortedEntries = {};
    for (const auto& entry : fs::directory_iterator(folderpath)) {
        sortedEntries.insert(entry);
    }

    return sortedEntries
        | std::ranges::to<std::vector>();
}

int main(void)
{
    const gui::color_palette paletteDefault;
    constexpr auto windowPadding = 10.f;

    gui::flexbox sidebar;
    sidebar.position = (Vector2) { windowPadding, windowPadding };
    sidebar.size = (Vector2) { 200, -1 };
    sidebar.gap  = (Vector2) { windowPadding, windowPadding };

    const auto sortedEntries = sorted_directory_entries(fs::path("."));
    for (const auto& entry : sortedEntries) {
        gui::button btn;
        btn.size    = (Vector2) { 150, 30 };
        btn.label   = entry.path().filename();
        btn.padding = (Vector2) { windowPadding, windowPadding };

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
