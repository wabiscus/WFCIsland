#include "Grid.hpp"

Grid::Grid(int width, int height)
    : m_width(width),
      m_height(height),
      m_cells(width * height, Tile::Water)
{
}

int Grid::getWidth() const
{
    return m_width;
}

int Grid::getHeight() const
{
    return m_height;
}

Tile Grid::get(int x, int y) const
{
    return m_cells[y * m_width + x];
}

void Grid::set(int x, int y, Tile tile)
{
    m_cells[y * m_width + x] = tile;
}

void Grid::setScope(ScopeSize size)
{
    m_scopeSize = size;
    switch (size)
    {
    case ScopeSize::Small:
        m_scope = {24, 24, 24, 24};
        break;
    case ScopeSize::Medium:
        m_scope = {12, 12, 48, 48};
        break;
    case ScopeSize::Big:
        m_scope = {0, 0, 72, 72};
        break;
    default:
        break;
    }
}

const Scope& Grid::getScope() const
{
    return m_scope;
}

void Grid::fill(Tile tile)
{
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            set(x, y, tile);
        }
    }
}