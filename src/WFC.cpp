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
    m_grid.fill(Tile::Unknown);
    for (int y = 0; y < m_grid.getHeight(); ++y)
    {
        for (int x = 0; x < m_grid.getWidth(); ++x)
        {
            if (x == 0 || x == m_grid.getWidth() - 1)
            {
                m_grid.set(x, y, Tile::Water);
            }

            if (y == 0 || y == m_grid.getHeight() - 1)
            {
                m_grid.set(x, y, Tile::Water);
            }
        }
    }
}