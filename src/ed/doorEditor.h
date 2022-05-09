#pragma once

#include "../gbin/gfile.h"
#include "door.h"
#include <string>
#include <vector>

// For editing doors.
struct DoorEditor
{
    std::string dolLocation; // Where the DOL is on PC.
    std::string dolName; // Dol name.
    GFile gFile; // Loaded DOL file.
    bool open; // If the editor is open.
    bool newTableLoc = false; // If the new table location is used.
    std::vector<Door> doors; // Doors to store.

    // Create a new door editor.
    DoorEditor(std::string dolLocation, std::string dolName);

    // Read doors.
    void ReadDoors();

    // Save doors.
    void SaveDoors();

    // Draw the UI.
    void DrawUI();

    // Edit a door.
    void EditDoorUI(Door* d);

    // Update door editor.
    void Update();

};