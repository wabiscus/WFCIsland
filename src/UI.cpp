#include "UI.hpp"

#include "imgui.h"

void UI::render(const Grid& grid)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetIO().DisplaySize.y));

    ImGui::Begin(
        "Grid",
        nullptr,
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse
    );

    ImGui::Text("Width: %d", grid.getWidth());
    ImGui::Text("Height: %d", grid.getHeight());

    ImGui::End();
}