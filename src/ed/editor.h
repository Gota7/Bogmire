#pragma once

#include "../rlImGui/fileBrowser.h"
#include "../rlImGui/focusData.h"
#include "doorEditor.h"
#include "settings.h"
#include "styleEditor.h"
#include "raylib.h"
#include <vector>

// Main editor code.
struct Editor
{
    Image icon; // Editor icon.
    Settings settings; // Editor settings.
    FocusData focus; // Focus information for the currently displayed window.
    StyleEditor styleEditor = StyleEditor(this); // For editing style.
    imgui_addons::ImGuiFileBrowser fb; // File browser for selecting files.
    std::vector<DoorEditor> doorEditors; // Editors that edit doors.

    // Map2 room pointer.
    static const s64 MAP2_ROOM_ROOM_PTR = 0x300790;

    // Map2 door pointer.
    static const s64 MAP2_DOOR_TABLE_PTR = 0x300794;

    // Old door table location.
    static const s64 OLD_DOOR_TABLE = 0x2FFF80;

    // New door table location.
    static const s64 NEW_DOOR_TABLE = 0x336000;

    // Initialize the editor.
    void Init();

    // Main editor loop.
    int EditorLoop();

    // Draw editor UI.
    void DrawUI();

    // Draw the main menu.
    void DrawMainMenu();

    // Open item menu.
    void OpenItem();

    // Update code.
    void Update();

    // Convert a RAM address to a DOL address.
    static s64 Ram2Dol(s64 ramAddr);

    // Convert a DOL address to a RAM address.
    static s64 Dol2Ram(s64 dolAddr);

};