#pragma once

#include "Grid.hpp"
#include "WFC.hpp"
#include "ShapeGenerator.hpp"
#include "App.hpp"

enum class UIPanel
{
    Left,
    Right
};

class UI
{
public:
    explicit UI(UIPanel panel) : m_panel(panel) {};
    void render(App &app);

    bool isGridVisible() const;
    bool arePossibilitiesVisible() const;
    bool isBoundaryDefined() const;

private:
    UIPanel m_panel = UIPanel::Left;
    bool m_showGrid = false;


    bool m_showPossibilities = false;
    bool m_isShapeGenerated = false;
    bool m_isIslandGenerated = false;
    bool m_isBoundaryDefined = false;
    bool m_isBoundaryNotSet = true;
};