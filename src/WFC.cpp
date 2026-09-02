#include "WFC.hpp"
#include "Grid.hpp"

#include <random>

WFC::WFC(Grid &grid)
    : m_grid(grid)
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