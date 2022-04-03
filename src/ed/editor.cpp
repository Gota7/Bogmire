#include "editor.h"
#include "../rlImGui/rlImGui.h"

static s64 HeaderDolOffsets[] = { 256, 9408, 0, 0, 0, 0, 0, 2132000, 2132384, 2132672, 2133472, 2133504, 2183424, 3750112, 3783008, 3799584, 0, 0 };
static s64 HeaderRAMOffsets[] = { 2147496192, 2147506016, 0, 0, 0, 0, 0, 2147505344, 2147505728, 2149628608, 2149629408, 2149629440, 2149679360, 2152529760, 2152568960, 2152585536, 0, 0 };
static s64 HeaderSectionSizes[] = { 9152, 2122592, 0, 0, 0, 0, 0, 384, 288, 800, 32, 49920, 1566688, 32896, 16576, 16384, 0, 0 };

void Editor::Init()
{

    // Load the settings.
    settings.Load();

    // Then set up the main window.
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(settings.width, settings.height, "Bogmire");
    icon = LoadImage("icon.png");
    SetWindowIcon(icon);
    SetTargetFPS(60);
    SetupRLImGui(true);
    settings.LoadTheme();

}

int Editor::EditorLoop()
{

    // Keep open.
    while (!WindowShouldClose())
    {

        // Draw code.
        BeginDrawing();
        ClearBackground(BLACK);

        // UI Code.
        BeginRLImGui();
        styleEditor.DrawUI();
        DrawUI();
        EndRLImGui();
        EndDrawing();

        // Update code here.
        Update();

    }

    // Clean up.
    UnloadImage(icon);
    ShutdownRLImGui();
    CloseWindow();
    return 0;

}

void Editor::DrawUI()
{

    // Code to draw the main menu.
    DrawMainMenu();

    // Draw door editors.
    for (auto& de : doorEditors)
    {
        de.DrawUI();
    }

}

void Editor::DrawMainMenu()
{

    // Has all the options.
    if (ImGui::BeginMainMenuBar())
    {

        if (ImGui::Button("Open"))
        {
            ImGui::OpenPopup("Open File");
        }
        if (ImGui::BeginMenu("Window"))
        {
            ImGui::Checkbox("Style Editor", &styleEditor.open);
            ImGui::EndMenu();
        }
        OpenItem();
        ImGui::EndMainMenuBar();

    }

}

void Editor::OpenItem()
{

    // Level selection box.
    static bool dolAlreadyOpenPopup = false;
    if (fb.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(0, 0), ".dol"))
    {
        bool dolAlreadyOpen = false;
        int dolNum = 0;
        for (auto& de : doorEditors)
        {
            if (de.dolLocation == fb.selected_path)
            {
                dolAlreadyOpen = true;
                break;
            }
            if (GetFileName(de.dolLocation.c_str()) == fb.selected_fn) dolNum++;
        }
        if (!dolAlreadyOpen)
        {
            std::string doorName = GetFileName(fb.selected_fn.c_str());
            if (dolNum > 0) doorName += " " + std::to_string(dolNum + 1);
            doorEditors.push_back(DoorEditor(fb.selected_path, doorName));
        }
        else
        {
            ImGui::OpenPopup("DOL Is Already Open!");
            dolAlreadyOpenPopup = true;
        }
    }

    if (ImGui::BeginPopupModal("DOL Is Already Open!", &dolAlreadyOpenPopup, ImGuiWindowFlags_NoResize))
    {
        ImGui::Text("You can not open a DOL that is already open.");
        ImGui::EndPopup();
    }

}

void Editor::Update()
{

    // Update door editors.
    for (auto& de : doorEditors)
    {
        de.Update();
    }

    // Remove closed door editors.
    for (int i = doorEditors.size() - 1; i >= 0; i--)
    {
        if (!doorEditors[i].open) doorEditors.erase(doorEditors.begin() + i);
    }

}

s64 Editor::Ram2Dol(s64 ramAddr)
{
    for (int i = 0; i < IM_ARRAYSIZE(HeaderSectionSizes); i++)
    {
        if (ramAddr >= HeaderRAMOffsets[i] && ramAddr <= HeaderRAMOffsets[i] + HeaderSectionSizes[i])
        {
            return ramAddr - HeaderRAMOffsets[i] + HeaderDolOffsets[i];
        }
    }
    return -1;
}

s64 Editor::Dol2Ram(s64 dolAddr)
{
    for (int i = 0; i < IM_ARRAYSIZE(HeaderSectionSizes); i++)
    {
        if (dolAddr >= HeaderDolOffsets[i] && dolAddr <= HeaderDolOffsets[i] + HeaderSectionSizes[i])
        {
            return dolAddr - HeaderDolOffsets[i] + HeaderRAMOffsets[i];
        }
    }
    return -1;
}