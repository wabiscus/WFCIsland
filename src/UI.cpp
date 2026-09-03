#include "UI.hpp"
#include "imgui.h"
#include <string>
#include <cstring>

void UI::render(Grid &grid, WFC &wfc, ShapeGenerator &shapegen, Ruleset &ruleset)
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
        if (ImGui::Button("Generate Map"))
        {
            wfc.regenerateMap();
            shapegen.generate(
                m_influencePointCount,
                m_sharpness,
                m_roundness);
            shapegen.connectPoints();
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
            wfc.regenerateMap();
            shapegen.generate(
                m_influencePointCount,
                m_sharpness,
                m_roundness);
            shapegen.connectPoints();
        }

        ImGui::Separator();

        ImGui::SliderFloat(
            "Sharpness",
            &m_sharpness,
            0.0f,
            5.0f,
            "%.2f");

        ImGui::SliderFloat(
            "Roundness",
            &m_roundness,
            -1.0f,
            1.0f,
            "%.2f");

        ImGui::Separator();

        ImGui::Checkbox("Show possibilities", &m_showPossibilities);

        if (m_showPossibilities)
        {
            ImDrawList *drawList = ImGui::GetForegroundDrawList();
            const Scope &scope = grid.getScope();

            for (int y = scope.y; y < scope.y + scope.height; ++y)
            {
                for (int x = scope.x; x < scope.x + scope.width; ++x)
                {
                    const Cell &cell = grid.get(x, y);

                    std::string count = std::to_string(cell.entropy);

                    float screenX = 280.0f + x * 10.0f;
                    float screenY = y * 10.0f;

                    drawList->AddText(
                        ImVec2(screenX, screenY),
                        IM_COL32(255, 255, 255, 255),
                        count.c_str());
                }
            }
        }

        ImGui::Separator();

        if (ImGui::Button("Propagate Step"))
        {
            wfc.propagateStep();
        }
        if (ImGui::Button("Propagate All"))
        {
            wfc.propagateAll();
        }
        if (ImGui::Button("Collapse"))
        {
            wfc.collapse();
        }

        break;
    }
    case UIPanel::Right:
    {
        ImGui::SetNextWindowPos(ImVec2(1000, 0));
        ImGui::SetNextWindowSize(ImVec2(280, ImGui::GetIO().DisplaySize.y));
        ImGui::Begin(
            "Ruleset",
            nullptr,
            ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoCollapse);

        // ImGui::Text("Current Ruleset : %d", ruleset.getName().c_str());
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

bool UI::arePossibilitiesVisible() const
{
    return m_showPossibilities;
}