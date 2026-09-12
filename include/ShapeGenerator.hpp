#pragma once
#include <vector>
#include <random>

#include "Ruleset.hpp"

class Grid;

struct InfluencePoint
{
    int x;
    int y;
};

class ShapeGenerator
{
public:
    explicit ShapeGenerator(Grid &grid, Ruleset &ruleset);

    void generate(int influencePointCount, float sharpness, float roundness);
    void clearPoints();

    void connectPoints();

    void defineBoundary();

    void fillOutsideWithWater();

    void setSeed(unsigned int seed);

private:
    Grid &m_grid;
    std::vector<InfluencePoint> m_points;
    Ruleset &m_ruleset;
    unsigned int m_seed;
    std::mt19937 m_generator;

    float m_sharpness = 0.0f;
    float m_roundness = 1.0f;

    bool isInside(int x, int y) const;
};
