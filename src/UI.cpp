#include "UI.hpp"

#include "imgui.h"

void UI::render(Grid &grid, WFC &wfc, ShapeGenerator &shapegen)
{
    switch (m_panel)
    {
    case UIPanel::Left:
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(280, ImGui::GetIO().DisplaySize.y));

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
        ImGui::Spacing();

        const char *scopeSizes[] = {
            "Small",
            "Medium",
            "Big"};

        int currentScope = static_cast<int>(m_scopeSize);

        if (ImGui::Combo(
                "Scope",
                &currentScope,
                scopeSizes,
                IM_ARRAYSIZE(scopeSizes)))
        {
            m_scopeSize = static_cast<ScopeSize>(currentScope);
            grid.setScope(m_scopeSize);
            wfc.reset();
            shapegen.generate(m_influencePointCount);
        }

        break;
    }
    case UIPanel::Right:
    {
        ImGui::SetNextWindowPos(ImVec2(1000, 0));
        ImGui::SetNextWindowSize(ImVec2(280, ImGui::GetIO().DisplaySize.y));
        ImGui::Begin(
            "Info",
            nullptr,
            ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoCollapse);
        break;
    }
    default:
    {
        break;
    }
    }

    ImGui::End();
}

bool UI::isGridVisible() const
{
    return m_showGrid;
}