#pragma once

#include "style.h"

// Settings.
struct Settings
{
    EditorStyle style; // Current editor style.
    int width = 1240; // Width of the window.
    int height = 720; // Height of the editor.

    // Load the settings.
    void Load();

    // Load theme.
    void LoadTheme();

    // Save the settings.
    void Save();

};