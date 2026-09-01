#pragma once

#include "Grid.hpp"
#include "WFC.hpp"

class UI
{
public:
    void render(const Grid& grid, WFC& wfc);

    bool isGridVisible() const;

private:
    bool m_showGrid = false;
};