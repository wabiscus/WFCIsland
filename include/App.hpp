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
    Generated
};

enum class GenerationEvent
{
    GenerateShape,
    DefineBoundary,
    Generate,
    RestoreShape,
    RestoreBoundaries
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
};