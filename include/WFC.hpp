#pragma once

#include "Grid.hpp"
#include "Ruleset.hpp"

class WFC
{
public:
    explicit WFC(Grid &grid, Ruleset &ruleset);

    void generate();

    void reset();

    bool propagateStep();

    void propagateAll();

private:
    Grid &m_grid;
    Ruleset &m_ruleset;
};