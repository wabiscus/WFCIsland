#pragma once

#include "Grid.hpp"
#include "Ruleset.hpp"

#include <random>

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

private:
    Grid &m_grid;
    Ruleset &m_ruleset;
    std::mt19937 m_generator;
    bool m_contradiction = false;
};