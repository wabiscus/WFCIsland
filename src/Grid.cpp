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