#include "Grid.hpp"

Grid::Grid(int width, int height)
    : m_width(width),
      m_height(height),
      m_cells(width * height, Cell(Tile::Unknown))
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

Cell &Grid::get(int x, int y)
{
    return m_cells[y * m_width + x];
}

const Cell &Grid::get(int x, int y) const
{
    return m_cells[y * m_width + x];
}

void Grid::set(int x, int y, const Cell &cell)
{
    m_cells[y * m_width + x] = cell;
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

const Scope &Grid::getScope() const
{
    return m_scope;
}

const ScopeSize &Grid::getScopeSize() const{
    return m_scopeSize;
}

void Grid::fill(const Cell &cell)
{
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            set(x, y, cell);
        }
    }
}

const std::vector<CellPosition> &Grid::getUnknownCells() const
{
    return m_unknownCells;
}

void Grid::updateUnknownCells()
{
    m_unknownCells.clear();

    const Scope &scope = m_scope;

    for (int y = scope.y; y < scope.y + scope.height; ++y)
    {
        for (int x = scope.x; x < scope.x + scope.width; ++x)
        {
            if (get(x, y).tile == Tile::Unknown)
            {
                m_unknownCells.push_back({x, y});
            }
        }
    }
}

bool Grid::hasUnknownCells() const
{
    return !m_unknownCells.empty();
}

void Grid::removeUnknownCell(std::size_t index)
{
    m_unknownCells[index] = m_unknownCells.back();
    m_unknownCells.pop_back();
}

void Grid::saveState()
{
    m_savedCells = m_cells;
    m_savedUnknownCells = m_unknownCells;
}

void Grid::restoreState()
{
    m_cells = m_savedCells;
    m_unknownCells = m_savedUnknownCells;
}

void Grid::saveShape()
{
    m_savedBoundary.clear();

    const Scope &scope = m_scope;

    for (int y = scope.y; y < scope.y + scope.height; ++y)
    {
        for (int x = scope.x; x < scope.x + scope.width; ++x)
        {
            if (get(x, y).tile == Tile::Boundary)
            {
                m_savedBoundary.push_back({x, y});
            }
        }
    }
}

void Grid::restoreShape()
{
    fill(Cell(Tile::Unknown));

    for (CellPosition cell : m_savedBoundary)
    {
        set(cell.x, cell.y, Cell(Tile::Boundary));
    }
}

Cell::Cell() : tile(Tile::Unknown), possibilities(), entropy(0) {}

Cell::Cell(Tile tile)
    : Cell()
{
    if (tile != Tile::Unknown)
    {
        this->tile = tile;
        this->possibilities = {tile};
        this->entropy = 1;
    }
}

Cell::Cell(const std::vector<Tile> &tiles) : tile(Tile::Unknown), possibilities(tiles), entropy(static_cast<int>(tiles.size())) {}