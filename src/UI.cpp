#include "UI.hpp"
#include "imgui.h"
#include <string>
#include <cstring>

const char *tileToString(Tile tile)
{
    switch (tile)
    {
    case Tile::Water:
        return "Water";

    case Tile::Sand:
        return "Sand";

    case Tile::Grass:
        return "Grass";

        // case Tile::Forest:
        //     return "Forest";

        // case Tile::Rock:
        //     return "Rock";

        // case Tile::Snow:
        //     return "Snow";

        // case Tile::Lava:
        //     return "Lava";

    case Tile::Unknown:
        return "Unknown";

        // case Tile::Contradiction:
        //     return "Contradiction";
    }

    return "Unknown";
}

ImU32 tileToColor(Tile tile)
{
    switch (tile)
    {
    case Tile::Water:
        return IM_COL32(50, 120, 220, 255);

    case Tile::Sand:
        return IM_COL32(220, 190, 120, 255);

    case Tile::Grass:
        return IM_COL32(80, 180, 80, 255);

        // case Tile::Forest:
        //     return IM_COL32(30, 110, 50, 255);

        // case Tile::Rock:
        //     return IM_COL32(100, 100, 100, 255);

        // case Tile::Snow:
        //     return IM_COL32(230, 230, 240, 255);

        // case Tile::Lava:
        //     return IM_COL32(220, 70, 30, 255);

    default:
        return IM_COL32(150, 150, 150, 255);
    }
}

const char *rulesetToString(RulesetType type)
{
    switch (type)
    {
    case RulesetType::Tropical:
        return "Tropical";

    case RulesetType::Desert:
        return "Desert";

    case RulesetType::Forest:
        return "Forest";

    case RulesetType::Volcanic:
        return "Volcanic";

    case RulesetType::Count:
        return "Count";
    }

    return "Unknown";
}

const char *rulesetItems[] =
    {
        rulesetToString(RulesetType::Tropical),
        rulesetToString(RulesetType::Desert),
        rulesetToString(RulesetType::Forest),
        rulesetToString(RulesetType::Volcanic)};

const char *stateToString(GenerationState state)
{
    switch (state)
    {
    case GenerationState::Empty:
        return "Empty";

    case GenerationState::Shape:
        return "Shape";

    case GenerationState::BoundariesDefined:
        return "Boundaries Defines";

    case GenerationState::Generating:
        return "Generating";

    case GenerationState::Generated:
        return "Generated";
    }

    return "Unknown";
}

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

void UI::render(WFC &wfc, ShapeGenerator &shapegen, Ruleset &ruleset, App &app)
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

        ImGui::Text("Width: %d", app.getWidth());
        ImGui::Text("Height: %d", app.getHeight());

        ImGui::Text("Current state: %s", stateToString(app.getGenerationState()));

        ImGui::Spacing();
        if (ImGui::Button("Generate Island Shape"))
        {
            app.handleEvent(GenerationEvent::GenerateShape);
            m_isIslandGenerated = false;
            m_isShapeGenerated = true;
            m_isBoundaryDefined = true;
        }

        ImGui::Separator();

        ImGui::Checkbox("Show grid", &m_showGrid);

        ImGui::Separator();

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
            m_isIslandGenerated = false;
            m_isShapeGenerated = true;
            m_isBoundaryDefined = true;

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
        for (Tile tileNeighbor : ruleset.getAllowedNeighbors(Tile::Water))
        {
            drawTileLabel(tileNeighbor);
        }

        if (!m_isBoundaryDefined)
        {
            ImGui::BeginDisabled();
        }

        if (ImGui::Button(m_isBoundaryNotSet ? "Define Boundary" : "Refine Boundary"))
        {
            app.handleEvent(GenerationEvent::DefineBoundary);
            m_isBoundaryNotSet = false;
        }

        if (!m_isBoundaryDefined)
        {
            ImGui::EndDisabled();
        }

        ImGui::Separator();

        if (m_isIslandGenerated)
        {
            ImGui::BeginDisabled();
        }

        if (ImGui::Button("Generate One Step"))
        {
            wfc.propagateUntilStable();
            wfc.collapse();
        }

        if (m_isIslandGenerated)
        {
            ImGui::EndDisabled();
        }

        if (!m_isShapeGenerated)
        {
            ImGui::BeginDisabled();
        }

        if (ImGui::Button("Generate Island"))
        {
            wfc.generateIsland();
            m_isIslandGenerated = true;
            app.handleEvent(GenerationEvent::Generate);
        }

        if (ImGui::Button("Generate"))
        {
            app.handleEvent(GenerationEvent::Generate);
        }

        if (ImGui::Button(wfc.isGenerating() ? "Pause" : "Play"))
        {
            wfc.toggleGeneration();
        }

        if (ImGui::Button("Restore Island Shape"))
        {
            app.handleEvent(GenerationEvent::RestoreBoundaries);
            m_isIslandGenerated = false;
        }

        if (wfc.isGenerating())
        {
            wfc.generateStep();
            m_isIslandGenerated = true;
        }

        if (!m_isShapeGenerated)
        {
            ImGui::EndDisabled();
        }

        RulesetType currentRuleset = ruleset.getType();

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
                    app.switchRuleset(type);
                    app.handleEvent(GenerationEvent::RestoreBoundaries);

                    // Régénération ici
                }

                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
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
                ImGui::Text("Current Ruleset : %s", ruleset.getName().c_str());

                ImGui::Separator();

                ImGui::Text("Current Tiles : ");
                for (const Tile &tile : ruleset.getTiles())
                {
                    drawTileLabel(tile);
                }

                ImGui::Separator();

                ImGui::Text("Current Rules :");
                for (const Tile &tile : ruleset.getTiles())
                {
                    drawTileLabel(tile);
                    ImGui::Text("  -> ");
                    for (Tile tileNeighbor : ruleset.getAllowedNeighbors(tile))
                    {
                        drawTileLabel(tileNeighbor);
                    }
                    ImGui::Separator();
                }

                if (ImGui::BeginTabBar("TileWeights"))
                {
                    const auto &allWeights = ruleset.getWeights();

                    for (const auto &[possibilities, weights] : allWeights)
                    {
                        std::string tabName = "{";

                        for (std::size_t i = 0; i < possibilities.size(); ++i)
                        {
                            if (i > 0)
                                tabName += ", ";

                            tabName += tileToString(possibilities[i]);
                        }

                        tabName += "}";

                        if (ImGui::BeginTabItem(tabName.c_str()))
                        {
                            std::vector<int> &editableWeights =
                                ruleset.getWeights(possibilities);

                            for (std::size_t i = 0; i < possibilities.size(); ++i)
                            {
                                ImGui::SliderInt(
                                    tileToString(possibilities[i]),
                                    &editableWeights[i],
                                    0,
                                    10);
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

bool UI::isBoundaryDefined() const
{
    return m_isBoundaryDefined;
}