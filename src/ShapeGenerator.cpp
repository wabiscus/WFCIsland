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

    const Scope& scope = m_grid.getScope();

    while (influencePointCount > 0)
    {
        std::uniform_int_distribution<int> xDistribution(scope.x, scope.x + scope.width - 1);

        std::uniform_int_distribution<int> yDistribution(scope.y, scope.y + scope.height - 1);

        int x = xDistribution(generator);
        int y = yDistribution(generator);

        m_grid.set(x, y, Tile::Sand);

        influencePointCount--;
    }
}