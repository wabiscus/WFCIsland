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

private:
    void setupFSM();

private:
    Grid& m_grid;
    Ruleset& m_ruleset;
    ShapeGenerator& m_shapeGenerator;
    WFC& m_wfc;

    FSM<GenerationState, GenerationEvent> m_fsm{GenerationState::Empty};
};