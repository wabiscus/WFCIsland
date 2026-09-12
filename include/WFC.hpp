#pragma once

#include "Grid.hpp"
#include "Ruleset.hpp"
#include "FSM.hpp"

#include <random>
#include <queue>
#include <utility>

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

    void setRuleset(RulesetType newRulset);

    void resetPossibilities();

    bool isFinished() const;

    void setSeed(unsigned int seed);

private:
    Grid &m_grid;
    Ruleset &m_ruleset;
    unsigned int m_seed;
    std::mt19937 m_generator;
    bool m_contradiction = false;
    std::queue<CellPosition> m_propagationQueue;
};