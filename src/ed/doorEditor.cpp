#include "doorEditor.h"
#include "../rlImGui/utils.h"
#include "editor.h"
#include "imgui.h"
#include <fstream>

// Static vars.
char** RoomNames = nullptr;
char** RoomDescriptions = nullptr;
int NumRooms;

// Models for each of the door.
static const char* DoorModels [] =
{
    "Invisible",
    "Mansion Door 1",
    "Mansion Door 2",
    "Parlor Double Door",
    "Anteroom Door",
    "Lab Door",
    "Gallery Door",
    "Nursery Door",
    "Twins' Room Door",
    "Wooden Door",
    "Basement Hallway Door",
    "Hearts Double Door",
    "Clubs Door",
    "Diamonds Door",
    "Spades Door",
    "Door Spikes"
};

// for string delimiter
std::vector<std::string> Split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos)
    {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

DoorEditor::DoorEditor(std::string dolLocation, std::string dolName)
{
    this->dolLocation = dolLocation;
    this->dolName = dolName;
    gFile = GFile(dolLocation.c_str());
    gFile.SetEndian(true);
    ReadDoors();
    open = true;
    if (RoomNames == nullptr)
    {
        std::ifstream file("object_data/rooms.txt");
        std::string str;
        std::vector<std::string> roomNames;
        std::vector<std::string> roomDescriptions;
        while (std::getline(file, str))
        {
            auto split = Split(str, ";");
            roomNames.push_back(split[0]);
            roomDescriptions.push_back(split[1]);
        }
        RoomDescriptions = GenImGuiStringList(roomDescriptions, &NumRooms);
        RoomNames = GenImGuiStringList(roomNames, &NumRooms);
    }
}

void DoorEditor::ReadDoors()
{

    // Get door table position.
    gFile.position = (u64)Editor::MAP2_DOOR_TABLE_PTR;
    s64 pos = Editor::Ram2Dol(gFile.ReadU32());
    if (pos == Editor::OLD_DOOR_TABLE)
    {
        newTableLoc = false;
    }
    else if (pos == Editor::NEW_DOOR_TABLE)
    {
        newTableLoc = true;
    }
    else
    {
        printf("ERROR: Unrecognized door pointer table %x!\n", pos);
        open = false;
        return;
    }

    // Read the actual doors.
    doors.clear();
    gFile.position = (u64)pos;
    while (true)
    {
        Door d;
        d.Read(&gFile);
        if (d.orientation == 0) break;
        doors.push_back(d);
    }

}

void DoorEditor::DrawUI()
{

    // Safety.
    if (!open) return;

    // Draw data.
    ImGui::Begin(("Door Editor - " + dolName).c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
    EditDoorUI(&doors[0]);
    ImGui::End();

}

void DoorEditor::EditDoorUI(Door* d)
{

    // Orientation.
    ImGui::PushItemWidth(150);
    int orientation = d->orientation;
    if (ImGui::RadioButton("Front Facing", &orientation, 1)) d->orientation = 1;
    ImGui::SameLine();
    if (ImGui::RadioButton("Side Facing", &orientation, 2)) d->orientation = 2;
    ImGuiTooltip("The direction the door should face.");

    // Type.
    int type = d->type;
    if (ImGui::RadioButton("Door", &type, 0)) d->type = 0;
    ImGui::SameLine();
    if (ImGui::RadioButton("Viewport", &type, 1)) d->type = 1;
    ImGui::SameLine();
    if (ImGui::RadioButton("Window", &type, 2)) d->type = 2;
    ImGuiTooltip("If the door is really a door or something else.\nA viewport views the player coming from the door.");

    // Data.
    ImGui::InputScalar("JMP Door Number", ImGuiDataType_U16, &d->jmpDoorNum);
    ImGuiTooltip("This is the door number referenced in iteminfotable, objinfo, keyinfo, etc.");
    int currDoor = d->modelId;
    if (ImGui::Combo("Door Model", &currDoor, DoorModels, IM_ARRAYSIZE(DoorModels))) d->modelId = (u8)currDoor;
    ImGuiTooltip("Model of the door to use.\nSee https://wiki.lmhack.in/index.php/Documentation:Door_Models");
    ImGui::InputScalar("Door Index", ImGuiDataType_S8, &d->doorIndex);
    ImGuiTooltip("Used by the door loading list for rooms.");

    // Position.
    ImGui::PushItemWidth(50);
    ImGui::InputScalar("X", ImGuiDataType_S32, &d->xPos);
    ImGui::SameLine();
    ImGui::InputScalar("Y", ImGuiDataType_S32, &d->yPos);
    ImGui::SameLine();
    ImGui::InputScalar("Z", ImGuiDataType_S32, &d->zPos);
    ImGuiTooltip("Position of the door on the map.\nY-Positions:\n\tBasement = -400\n\tFloor 1 = 150\n\tFloor 2 = 700\n\tFloor 3 = 1250");

    // Viewport stuff.
    if (d->type == 0)
    {
        d->viewportX = orientation == 1 ? 200 : 0;
        d->viewportY = 300;
        d->viewportZ = orientation == 1 ? 0 : 200;
    }
    else
    {
        ImGui::InputScalar("Viewport X Size", ImGuiDataType_S16, &d->viewportX);
        ImGuiTooltip("X size of the viewport.");
        ImGui::InputScalar("Viewport Y Size", ImGuiDataType_S16, &d->viewportY);
        ImGuiTooltip("Y size of the viewport.");
        ImGui::InputScalar("Viewport Z Size", ImGuiDataType_S16, &d->viewportZ);
        ImGuiTooltip("Z size of the viewport.");
    }
    ImGui::PopItemWidth();

    // Room indices.
    int nextRoomIndex = d->nextRoomIndex;
    int currRoomIndex = d->currRoomIndex;
    if (ImGui::Combo("Next Room", &nextRoomIndex, RoomNames, NumRooms))
    {
        d->nextRoomIndex = (u8)nextRoomIndex;
    }
    ImGuiTooltip("For forward facing doors, next room refers to the room to the south of the door.\nFor sideways facing doors, next room refers to the room to the west of the door.");
    if (ImGui::Combo("Current Room", &currRoomIndex, RoomNames, NumRooms))
    {
        d->currRoomIndex = (u8)currRoomIndex;
    }
    ImGuiTooltip("For forward facing doors, current room refers to the room to the north of the door.\nFor sideways facing doors, current room refers to the room to the east of the door.");

    // Style.
    ImGui::PopItemWidth();

}

void DoorEditor::Update()
{

    // Close resources.
    doors.clear();
    if (!open)
    {
        gFile.Close();
    }

}