#pragma once

#include "Grid.hpp"
#include "Ruleset.hpp"

#include <random>

struct Candidate
{
    int x;
    int y;
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
    
    bool hasUnknownCells() const;

    void generateIsland();

private:
    Grid &m_grid;
    Ruleset &m_ruleset;
    std::mt19937 m_generator;
    bool m_contradiction = false;
};