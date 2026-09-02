#include "WFC.hpp"

#include <random>

WFC::WFC(Grid &grid, Ruleset &ruleset)
    : m_grid(grid), m_ruleset(ruleset)
{
}

void WFC::generate()
{
    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_int_distribution<int> xDistribution(
        0,
        m_grid.getWidth() - 1);

    std::uniform_int_distribution<int> yDistribution(
        0,
        m_grid.getHeight() - 1);

    int x = xDistribution(generator);
    int y = yDistribution(generator);

    m_grid.set(x, y, Tile::Grass);
}

void WFC::reset()
{
    m_grid.fill(Tile::Water);

    const Scope &scope = m_grid.getScope();

    int left = scope.x;
    int right = scope.x + scope.width - 1;
    int top = scope.y;
    int bottom = scope.y + scope.height - 1;

    for (int y = top + 1; y < bottom; ++y)
    {
        for (int x = left + 1; x < right; ++x)
        {
            m_grid.set(x, y, Tile::Unknown);
        }
    }
}

bool WFC::propagateStep()
{
    bool changed = false;

    const Scope &scope = m_grid.getScope();

    for (int y = scope.y; y < scope.y + scope.height; ++y)
    {
        for (int x = scope.x; x < scope.x + scope.width; ++x)
        {
            if (x > scope.x &&
                x < scope.x + scope.width - 1 &&
                y > scope.y &&
                y < scope.y + scope.height - 1)
            {
                Cell &top = m_grid.get(x, y - 1);
                Cell &bottom = m_grid.get(x, y + 1);
                Cell &left = m_grid.get(x - 1, y);
                Cell &right = m_grid.get(x + 1, y);
            }
        }
    }

    return changed;
}

void WFC::propagateAll()
{
    while (propagateStep())
    {
    }
}