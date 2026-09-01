#pragma once

class Grid;

class ShapeGenerator
{
public:
    explicit ShapeGenerator(Grid &grid);

    void generate(int influencePointCount);

private:
    Grid& m_grid;
};
