#include "ShapeGenerator.hpp"
#include "Grid.hpp"

#include <random>
#include <numbers>
#include <cmath>
#include <iostream>
#include <utility>

ShapeGenerator::ShapeGenerator(Grid &grid) : m_grid(grid)
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
        m_grid.set(point.x, point.y, Tile::Sand);
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
            m_grid.set(x0, y0, Tile::Sand);
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