#pragma once

#include "Grid.hpp"
#include "WFC.hpp"
#include "ShapeGenerator.hpp"

class UI
{
public:
    void render(const Grid& grid, WFC& wfc, ShapeGenerator& shapegen);

    bool isGridVisible() const;

private:
    bool m_showGrid = false;
    int m_influencePointCount = 6;
};