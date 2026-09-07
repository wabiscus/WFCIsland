#pragma once

#include "Grid.hpp"
#include "Ruleset.hpp"
#include "FSM.hpp"

#include <random>
#include <queue>
#include <utility>

enum class GenerationState
{
    Empty,
    Shape,
    BoundariesDefined,
    Generating,
    Generated
};

class WFC
{
public:
    explicit WFC(Grid &grid, Ruleset &ruleset);

    void initialize();

    void generate();

    void regenerateMap();

    bool propagateStep();

    void propagateUntilStable();

    void collapse();

    bool hasContradiction() const;

    void generateIsland();

    void generateStep();

    bool isGenerating() const;

    void startGeneration();
    void toggleGeneration();

    void setRuleset(RulesetType newRulset);

    void resetPossibilities();

    GenerationState getState() const;

private:
    Grid &m_grid;
    Ruleset &m_ruleset;
    std::mt19937 m_generator;
    bool m_contradiction = false;
    bool m_generating = false;
    std::queue<CellPosition> m_propagationQueue;

    FSM<GenerationState> m_fsm{GenerationState::Empty};
};