#pragma once

#include "../gbin/gfile.h"
#include <string.h>

// For editing doors.
struct DoorEditor
{
    std::string dolLocation; // Where the DOL is on PC.
    std::string dolName; // Dol name.
    GFile gFile; // Loaded DOL file.
    bool open; // If the editor is open.

    // Create a new door editor.
    DoorEditor(std::string dolLocation, std::string dolName);

    // Draw the UI.
    void DrawUI();

    // Update door editor.
    void Update();

};