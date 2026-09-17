#include "UI.hpp"
#include "imgui.h"
#include <string>
#include <cstring>
#include <algorithm>

#include "Utils.hpp"

const char *rulesetItems[] =
    {
        rulesetToString(RulesetType::Tropical),
        rulesetToString(RulesetType::Desert),
        rulesetToString(RulesetType::Forest),
        rulesetToString(RulesetType::Volcanic)};

void drawTileLabel(Tile tile)
{
    ImGui::Text("%s", tileToString(tile));
    ImGui::SameLine();

    ImDrawList *drawList = ImGui::GetWindowDrawList();

    ImVec2 pos = ImGui::GetCursorScreenPos();
    constexpr float size = 12.0f;

    drawList->AddRectFilled(
        pos,
        ImVec2(pos.x + size, pos.y + size),
        tileToColor(tile));

    ImGui::Dummy(ImVec2(size, size));
}

void UI::render(App &app)
{
    const GenerationState state =
        app.getGenerationState();

    const bool canGenerateShape =
        state != GenerationState::Generating &&
        state != GenerationState::GeneratingInstantly;

    const bool canDefineBoundary =
        state == GenerationState::Shape ||
        state == GenerationState::BoundariesDefined;

    const bool canGenerate =
        state == GenerationState::BoundariesDefined;

    const bool canRestoreShape =
        state == GenerationState::Generated;

    if (!m_seedInitialized)
    {
        m_seed = app.getSeed();
        m_seedInitialized = true;
    }

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

        ImGui::Text("Width: %d", app.getWidth());
        ImGui::Text("Height: %d", app.getHeight());

        ImGui::Text("Current state: %s", stateToString(app.getGenerationState()));

        if (!canGenerateShape)
        {
            ImGui::BeginDisabled();
        }

        ImGui::Spacing();
        if (ImGui::Button("Generate Island Shape", ImVec2(-1, 0)))
        {
            app.randomizeSeed();
            m_seed = app.getSeed();
            app.handleEvent(GenerationEvent::GenerateShape);
        }

        if (!canGenerateShape)
        {
            ImGui::EndDisabled();
        }

        ImGui::Separator();

        ImGui::Text("Seed : %u", app.getSeed());

        if (!canGenerateShape)
        {
            ImGui::BeginDisabled();
        }

        ImGui::InputScalar(
            "Seed",
            ImGuiDataType_U32,
            &m_seed);

        if (ImGui::Button("Set Seed"))
        {
            app.setSeed(m_seed);
            m_seed = app.getSeed();
            app.handleEvent(GenerationEvent::GenerateShape);
        }
        if (!canGenerateShape)
        {
            ImGui::EndDisabled();
        }

        ImGui::Separator();

        ImGui::Checkbox("Show grid", &m_showGrid);

        ImGui::Separator();

        if (!canGenerateShape)
        {
            ImGui::BeginDisabled();
        }

        ImGui::SliderInt("Points", &app.getInfluencePointsCount(), 3, 20);
        ImGui::Spacing();

        const char *scopeSizes[] = {
            "Small",
            "Medium",
            "Big"};

        int currentScope = static_cast<int>(app.getScopeSize());

        if (ImGui::Combo(
                "Scope",
                &currentScope,
                scopeSizes,
                IM_ARRAYSIZE(scopeSizes)))
        {
            app.setScopeSize(static_cast<ScopeSize>(currentScope));
            app.handleEvent(GenerationEvent::GenerateShape);
        }

        ImGui::Separator();

        ImGui::SliderFloat(
            "Sharpness",
            &app.getSharpness(),
            0.0f,
            5.0f,
            "%.2f");

        ImGui::SliderFloat(
            "Roundness",
            &app.getRoundness(),
            -1.0f,
            1.0f,
            "%.2f");

        if (!canGenerateShape)
        {
            ImGui::EndDisabled();
        }

        ImGui::Separator();

        ImGui::Checkbox("Show possibilities", &m_showPossibilities);

        if (m_showPossibilities)
        {
            ImDrawList *drawList = ImGui::GetForegroundDrawList();
            const Scope &scope = app.getScope();

            for (int y = scope.y; y < scope.y + scope.height; ++y)
            {
                for (int x = scope.x; x < scope.x + scope.width; ++x)
                {
                    const Cell &cell = app.getCell(x, y);

                    std::string count = std::to_string(cell.entropy);

                    float screenX = 280.0f + x * 10.0f;
                    float screenY = y * 10.0f;
                    if (count != "1")
                    {
                        drawList->AddText(
                            ImVec2(screenX, screenY),
                            IM_COL32(255, 255, 255, 255),
                            count.c_str());
                    }
                }
            }
        }

        ImGui::Text("Boundary Possible Tiles :");
        for (Tile tileNeighbor : app.getAllowedNeighbors(Tile::Water))
        {
            drawTileLabel(tileNeighbor);
        }

        if (!canDefineBoundary)
        {
            ImGui::BeginDisabled();
        }

        const char *boundaryLabel =
            state == GenerationState::Shape
                ? "Define Boundary"
                : "Refine Boundary";

        if (ImGui::Button(boundaryLabel))
        {
            app.handleEvent(GenerationEvent::DefineBoundary);
        }

        if (!canDefineBoundary)
        {
            ImGui::EndDisabled();
        }

        ImGui::Separator();

        if (!canRestoreShape)
        {
            ImGui::BeginDisabled();
        }

        if (ImGui::Button("Restore Island Shape"))
        {
            app.handleEvent(GenerationEvent::RestoreBoundaries);
        }

        if (!canRestoreShape)
        {
            ImGui::EndDisabled();
        }

        ImGui::Separator();

        if (!canGenerate)
        {
            ImGui::BeginDisabled();
        }

        if (ImGui::Button("Generate One Step"))
        {
            app.generateOneStep();
        }

        if (ImGui::Button("Generate Island"))
        {
            app.handleEvent(GenerationEvent::GenerateIsland);
            // m_isIslandGenerated = true;
        }

        if (ImGui::Button("Generate Step By Step"))
        {
            app.handleEvent(GenerationEvent::GenerateStepByStep);
        }

        if (!canGenerate)
        {
            ImGui::EndDisabled();
        }

        const bool canPause =
            state == GenerationState::Generating;

        if (!canPause)
        {
            ImGui::BeginDisabled();
        }

        if (ImGui::Button(app.isGeneratingRunning() ? "Pause" : "Play"))
        {
            app.toggleGeneration();
        }

        if (!canPause)
        {
            ImGui::EndDisabled();
        }

        ImGui::Separator();

        RulesetType currentRuleset = app.getType();

        if (ImGui::BeginCombo(
                "Ruleset",
                rulesetToString(currentRuleset)))
        {
            for (int i = 0; i < static_cast<int>(RulesetType::Count); ++i)
            {
                RulesetType type = static_cast<RulesetType>(i);

                bool selected = (type == currentRuleset);

                if (ImGui::Selectable(
                        rulesetToString(type),
                        selected))
                {
                    currentRuleset = type;
                    m_seed = app.getSeed();
                    app.setSeed(m_seed);
                    app.switchRuleset(type);
                    app.handleEvent(GenerationEvent::RestoreShape);

                    // Régénération ici
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        const bool canExport =
            state == GenerationState::Shape ||
            state == GenerationState::BoundariesDefined ||
            state == GenerationState::Generated;

        if (!canExport)
        {
            ImGui::BeginDisabled();
        }

        ImGui::Separator();

        if (ImGui::Button("Export Island Image"))
        {
            app.exportIslandPNG();
        }

        if (ImGui::Button("Export Island Json"))
        {
            app.exportIslandJson();
        }

        if (!canExport)
        {
            ImGui::EndDisabled();
        }

        ImGui::Separator();

        if (ImGui::Button("Report Bug"))
        {
            app.reportBug();
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

        if (ImGui::BeginTabBar("MyTabs"))
        {
            if (ImGui::BeginTabItem("Ruleset"))
            {
                ImGui::Text("Current Ruleset : %s", app.getName().c_str());

                ImGui::Separator();

                ImGui::Text("Current Tiles : ");
                for (const Tile &tile : app.getTiles())
                {
                    drawTileLabel(tile);
                }

                ImGui::Separator();

                ImGui::Text("Current Rules :");
                for (const Tile &tile : app.getTiles())
                {
                    drawTileLabel(tile);
                    ImGui::Text("  -> ");
                    for (Tile tileNeighbor : app.getAllowedNeighbors(tile))
                    {
                        drawTileLabel(tileNeighbor);
                    }
                    ImGui::Separator();
                }

                if (ImGui::BeginTabBar("TileWeights"))
                {
                    const auto &allWeights = app.getWeights();
                    for (const Tile &tile : app.getTiles())
                    {
                        if (ImGui::BeginTabItem(tileToString(tile)))
                        {
                            const std::vector<Tile> neighbors = app.getAllowedNeighbors(tile); // Cherche les poids correspondant exactement // aux voisins de cette tile.
                            auto it = allWeights.find(neighbors);
                            if (it != allWeights.end())
                            {
                                std::vector<int> &editableWeights = app.getWeights(it->first);
                                for (std::size_t i = 0; i < neighbors.size(); ++i)
                                {
                                    Tile neighbor = neighbors[i];

                                    ImGui::Text("%s", tileToString(neighbor));
                                    ImGui::SameLine();

                                    ImGui::ColorButton(
                                        ("##tileColor_" + std::to_string(i)).c_str(),
                                        ImGui::ColorConvertU32ToFloat4(tileToColor(neighbor)),
                                        ImGuiColorEditFlags_NoTooltip,
                                        ImVec2(14.0f, 14.0f));

                                    ImGui::SameLine();

                                    ImGui::SetNextItemWidth(150.0f);

                                    ImGui::SliderInt(
                                        ("##weight_" + std::to_string(i)).c_str(),
                                        &editableWeights[i],
                                        0,
                                        10);
                                }
                            }
                            ImGui::EndTabItem();
                        }
                    }
                    ImGui::EndTabBar();
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

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