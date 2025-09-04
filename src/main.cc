#include "enviroment.hh"
#include "gui_button.hh"
#include "gui_flexbox.hh"
#include "raylib.h"

#include <filesystem>
#include <iostream>
#include <print>
#include <ranges>
#include <set>
#include <vector>

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
    constexpr auto monitorSize       = (Vector2) { 2560, 1440 };
    constexpr auto windowSizeDefault = (Vector2) { 1920, 1080 };
    constexpr auto paddingDefault = 10;
    constexpr auto sidebarWidth   = 200;
    
    InitWindow(windowSizeDefault.x, windowSizeDefault.y, "EasyEdit");
    SetWindowPosition(
            monitorSize.x * 0.5 - windowSizeDefault.x * 0.5,
            monitorSize.y * 0.5 - windowSizeDefault.y * 0.5);

    enviroment env;

    gui::flexbox sidebar;
    sidebar.position = (Vector2) { paddingDefault, paddingDefault };
    sidebar.size     = (Vector2) { sidebarWidth, -1 };
    sidebar.gap      = (Vector2) { paddingDefault, paddingDefault };

    const auto sortedEntries = sorted_directory_entries(fs::path("."));
    for (const auto& entry : sortedEntries) {
        gui::button btn;
        btn.size    = (Vector2) { sidebarWidth, 25 };
        btn.padding = (Vector2) { 5, 5 };
        btn.font    = std::make_shared<Font>(env.font());
        btn.label   = entry.path().filename();

        if (entry.is_directory()) {
            btn.palette.fg0 = (Color) { 0xd6, 0x5d, 0x0e, 0xff };
        }

        sidebar.items.push_back(std::make_unique<gui::button>(btn));
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(env.palette().bg0);

            const auto width  = GetScreenWidth();
            const auto height = GetScreenHeight();
            const auto sidebarHeight = 
                static_cast<float>(height - paddingDefault * 2);

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
