#include "UI.hpp"

#include "imgui.h"

void UI::render(const Grid &grid, WFC &wfc)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetIO().DisplaySize.y));

    ImGui::Begin(
        "Grid",
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Grid Information");
    ImGui::Separator();
    ImGui::Text("Width: %d", grid.getWidth());
    ImGui::Text("Height: %d", grid.getHeight());

    ImGui::Spacing();
    if (ImGui::Button("Reset Grid"))
    {
        wfc.reset();
        wfc.generate();
    }

    ImGui::End();
}