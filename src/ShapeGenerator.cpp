#include "ShapeGenerator.hpp"
#include "Grid.hpp"

#include <random>

ShapeGenerator::ShapeGenerator(Grid &grid) : m_grid(grid)
{
}

void ShapeGenerator::generate(int influencePointCount)
{
    std::random_device rd;
    std::mt19937 generator(rd());

    while (influencePointCount > 0)
    {
        std::uniform_int_distribution<int> xDistribution(0, m_grid.getWidth() - 1);

        std::uniform_int_distribution<int> yDistribution(0, m_grid.getHeight() - 1);

        int x = xDistribution(generator);
        int y = yDistribution(generator);

        m_grid.set(x, y, Tile::Sand);

        influencePointCount--;
    }
}