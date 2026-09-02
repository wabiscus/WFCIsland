#pragma once
#include <vector>

class Grid;

struct InfluencePoint
{
    int x;
    int y;
};

class ShapeGenerator
{
public:
    explicit ShapeGenerator(Grid &grid);

    void generate(int influencePointCount, float sharpness, float roundness);
    void clearPoints();

    void connectPoints();
    
private:
    Grid &m_grid;
    std::vector<InfluencePoint> m_points;

    float m_sharpness = 0.0f;
    float m_roundness = 1.0f;
};
