#pragma once

#include "Grid.hpp"
#include "WFC.hpp"
#include "ShapeGenerator.hpp"

enum class UIPanel
{
    Left,
    Right
};

class UI
{
public:
    explicit UI(UIPanel panel) : m_panel(panel) {};
    void render(Grid &grid, WFC &wfc, ShapeGenerator &shapegen);

    bool isGridVisible() const;
    bool arePossibilitiesVisible() const;

private:
    UIPanel m_panel = UIPanel::Left;
    bool m_showGrid = false;
    int m_influencePointCount = 6;
    ScopeSize m_scopeSize = ScopeSize::Big;
    float m_sharpness = 2.5f;
    float m_roundness = 0.0f;

    bool m_showPossibilities = false;
};