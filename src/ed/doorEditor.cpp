#include "doorEditor.h"
#include "imgui.h"

DoorEditor::DoorEditor(std::string dolLocation, std::string dolName)
{
    this->dolLocation = dolLocation;
    this->dolName = dolName;
    gFile = GFile(dolLocation.c_str());
    open = true;
}

void DoorEditor::DrawUI()
{

    // Safety.
    if (!open) return;

    // Draw data.
    ImGui::Begin(("Door Editor - " + dolName).c_str(), &open);
    ImGui::Text("Hello World!");
    ImGui::End();

}

void DoorEditor::Update()
{

    // Close resources.
    if (!open)
    {
        gFile.Close();
    }

}