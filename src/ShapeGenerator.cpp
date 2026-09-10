#include "ShapeGenerator.hpp"
#include "Grid.hpp"

#include <random>
#include <numbers>
#include <cmath>
#include <iostream>
#include <utility>

ShapeGenerator::ShapeGenerator(Grid &grid, Ruleset &ruleset) : m_grid(grid), m_ruleset(ruleset)
{
}

void ShapeGenerator::generate(int influencePointCount, float sharpness, float roundness)
{
    m_points.clear();
    std::random_device rd;
    std::mt19937 generator(rd());

    const Scope &scope = m_grid.getScope();

    int centerX = scope.x + scope.width / 2;
    int centerY = scope.y + scope.height / 2;

    float maxRadius =
        std::min(scope.width, scope.height) / 2.0f - 1.0f;

    float angleStep = 2.0f * std::numbers::pi_v<float> / influencePointCount;

    float maxAngleOffset = angleStep * 0.3f * sharpness;

    float minRadius = maxRadius * 0.7f;
    float variation = maxRadius - minRadius;

    variation *= (1.0f - roundness);

    for (int i = 0; i < influencePointCount; ++i)
    {
        float angle = i * angleStep;

        std::uniform_real_distribution<float> offsetDistribution(
            -maxAngleOffset,
            maxAngleOffset);

        angle += offsetDistribution(generator);

        std::uniform_real_distribution<float> radiusDistribution(
            0.0f,
            variation);

        float radius = maxRadius - radiusDistribution(generator);

        float x = centerX + std::cos(angle) * radius;
        float y = centerY + std::sin(angle) * radius;

        InfluencePoint point{
            static_cast<int>(std::round(x)),
            static_cast<int>(std::round(y))};

        m_points.push_back(point);

        if (point.x < 0 || point.x >= m_grid.getWidth() ||
            point.y < 0 || point.y >= m_grid.getHeight())
        {
            std::cout << "OUT: "
                      << point.x << ", "
                      << point.y << '\n';
        }
        m_grid.set(point.x, point.y, Tile::Boundary);
    }
}

void ShapeGenerator::clearPoints()
{
    m_points.clear();
}

void ShapeGenerator::connectPoints()
{
    for (int i = 0; i < m_points.size(); i++)
    {
        int next = (i + 1) % m_points.size();
        InfluencePoint point0 = m_points[i];
        InfluencePoint point1 = m_points[next];

        int x0 = point0.x;
        int y0 = point0.y;
        int x1 = point1.x;
        int y1 = point1.y;

        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;
        while (true)
        { // Ajouter le point courant
            m_grid.set(x0, y0, Tile::Boundary);
            // Arrivée au point final
            if (x0 == x1 && y0 == y1)
                break;
            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y0 += sy;
            }
        }
    }
}

void ShapeGenerator::defineBoundary()
{
    std::random_device rd;
    std::mt19937 generator(rd());

    const std::vector<Tile> &waterNeighbors =
        m_ruleset.getAllowedNeighbors(Tile::Water);

    const std::vector<int> &weights =
        m_ruleset.getWeights(waterNeighbors);

    std::discrete_distribution<std::size_t> distribution(
        weights.begin(),
        weights.end());

    const Scope &scope = m_grid.getScope();

    for (int y = scope.y; y < scope.y + scope.height; ++y)
    {
        for (int x = scope.x; x < scope.x + scope.width; ++x)
        {
            const Cell &cell = m_grid.get(x, y);

            if (cell.tile == Tile::Boundary)
            {
                const Tile selectedTile =
                    waterNeighbors[distribution(generator)];

                m_grid.set(x, y, Cell(selectedTile));
            }
        }
    }
}

bool ShapeGenerator::isInside(int x, int y) const
{
    bool inside = false;

    const int pointCount = static_cast<int>(m_points.size());

    for (int i = 0, j = pointCount - 1; i < pointCount; j = i++)
    {
        const InfluencePoint &current = m_points[i];
        const InfluencePoint &previous = m_points[j];

        const bool crossesY =
            ((current.y > y) != (previous.y > y));

        if (!crossesY)
        {
            continue;
        }

        const float intersectionX =
            static_cast<float>(previous.x - current.x) *
                static_cast<float>(y - current.y) /
                static_cast<float>(previous.y - current.y) +
            static_cast<float>(current.x);

        if (static_cast<float>(x) < intersectionX)
        {
            inside = !inside;
        }
    }

    return inside;
}

void ShapeGenerator::fillOutsideWithWater()
{
    // const Scope &scope = m_grid.getScope();

    for (int y = 0; y < m_grid.getHeight(); ++y)
    {
        for (int x = 0; x < m_grid.getWidth(); ++x)
        {
            Cell &cell = m_grid.get(x, y);

            if (cell.tile == Tile::Unknown && !isInside(x, y))
            {
                cell = Cell(Tile::Water);
            }
        }
    }
}