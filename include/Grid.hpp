#pragma once

#include <vector>

enum class Tile
{
    Water,
    Sand,
    Grass
};

class Grid
{
public:
    Grid(int width, int height);

    int getWidth() const;
    int getHeight() const;

    Tile get(int x, int y) const;
    void set(int x, int y, Tile tile);

private:
    int m_width;
    int m_height;

    std::vector<Tile> m_cells;
};