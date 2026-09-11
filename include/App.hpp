#pragma once

#include "FSM.hpp"
#include "WFC.hpp"
#include "ShapeGenerator.hpp"

enum class GenerationState
{
    Empty,
    Shape,
    BoundariesDefined,
    Generating,
    GeneratingInstantly,
    Generated
};

enum class GenerationEvent
{
    GenerateShape,
    DefineBoundary,
    GenerateStepByStep,
    RestoreShape,
    RestoreBoundaries,
    GenerateIsland
};

class App
{
public:
    App(Grid &grid, Ruleset &ruleset, ShapeGenerator &shape, WFC &wfc);

    void handleEvent(GenerationEvent event);

    GenerationState getGenerationState() const;

    void update();
    void render();
    void renderUI();

    //// Grid interface
    int getWidth() const;
    int getHeight() const;

    Cell &getCell(int x, int y);

    ////
    int &getInfluencePointsCount();

    //// WFC interface

    void generateOneStep();
    void generateIsland();
    void generateStepbyStep();
    void toggleGeneration();
    bool isGeneratingRunning() const;

    //// Ruleset interface
    std::vector<Tile> getAllowedNeighbors(Tile tile) const;
    const std::vector<Tile> &getTiles() const;
    const std::string &getName() const;
    const RulesetType getType() const;
    const std::map<std::vector<Tile>, std::vector<int>> &getWeights() const;
    std::vector<int> &getWeights(const std::vector<Tile> &possibilities);
    ////
    const ScopeSize &getScopeSize() const;
    const Scope &getScope() const;
    void setScopeSize(ScopeSize scope);

    float &getSharpness();
    float &getRoundness();

    void switchRuleset(RulesetType type);

private:
    void setupFSM();

    Grid &m_grid;
    Ruleset &m_ruleset;
    ShapeGenerator &m_shapeGenerator;
    WFC &m_wfc;

    FSM<GenerationState, GenerationEvent> m_fsm{GenerationState::Empty};

    int m_influencePointCount = 6;
    float m_sharpness = 2.5f;
    float m_roundness = 0.0f;
    bool m_generationRunning = false;
};