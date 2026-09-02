#pragma once

#include <vector>

enum class Tile
{
    Unknown,
    Water,
    Sand,
    Grass
};

struct Cell
{
    Tile tile = Tile::Unknown;
    std::vector<Tile> possibilities = {Tile::Water, Tile::Sand, Tile::Grass};
    int possibilityCount = 3;

    Cell();
    Cell(Tile tile);
};

enum class ScopeSize
{
    Small,
    Medium,
    Big
};

struct Scope
{
    int x;
    int y;
    int width;
    int height;
};

class Grid
{
public:
    Grid(int width, int height);

    int getWidth() const;
    int getHeight() const;

    Cell &get(int x, int y);
    const Cell &get(int x, int y) const;
    void set(int x, int y, Tile tile);
    void fill(Tile tile);

    void setScope(ScopeSize scope);
    const Scope &getScope() const;

private:
    int m_width;
    int m_height;

    ScopeSize m_scopeSize = ScopeSize::Big;
    Scope m_scope = {0, 0, 72, 72};
    std::vector<Cell> m_cells;
};