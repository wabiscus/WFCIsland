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

    void generate();

    void regenerateMap();

    bool propagateStep();

    void propagateAll();

    void collapse();

private:
    Grid &m_grid;
    Ruleset &m_ruleset;
    std::mt19937 m_generator;
};