#include "UI.hpp"

#include "imgui.h"

void UI::render(const Grid &grid, WFC &wfc, ShapeGenerator &shapegen)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetIO().DisplaySize.y));

    ImGui::Begin(
        "Grid",
        nullptr,
        ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Width: %d", grid.getWidth());
    ImGui::Text("Height: %d", grid.getHeight());

    ImGui::Spacing();
    if (ImGui::Button("Reset Grid"))
    {
        wfc.reset();
        shapegen.generate(m_influencePointCount);
    }

    ImGui::Separator();

    ImGui::Checkbox("Show grid", &m_showGrid);

    ImGui::Separator();

    ImGui::SliderInt("Points", &m_influencePointCount, 3, 20);

    ImGui::End();
}

bool UI::isGridVisible() const
{
    return m_showGrid;
}