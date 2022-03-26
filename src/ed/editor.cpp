#include "editor.h"
#include "../rlImGui/rlImGui.h"

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