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

};