#pragma once

#include <vector>

/** @brief Enum representing different tile types */
enum class Tile
{
    Unknown,
    Boundary,
    Water,
    Sand,
    Grass,
    Forest,
    Rock,
    Snow,
    Lava,
    Contradiction
};

/** @brief Struct representing a cell in the grid */
struct Cell
{
    Tile tile = Tile::Unknown;
    std::vector<Tile> possibilities = {Tile::Water, Tile::Sand, Tile::Grass};
    int entropy = 3;

    Cell();
    Cell(Tile tile);
    Cell(const std::vector<Tile> &tiles);
};

/** @brief Struct representing a position of a cell in the grid */
struct CellPosition
{
    int x;
    int y;
};

/** @brief Enum representing different scope sizes */
enum class ScopeSize
{
    Small,
    Medium,
    Big
};

/** @brief Struct representing a scope in the grid */
struct Scope
{
    int x;
    int y;
    int width;
    int height;
};

/** @brief Class representing a grid */
class Grid
{
public:
    /** @brief Construct a new grid
     * @param width The width of the grid
     * @param height The height of the grid
     */
    Grid(int width, int height);

    /** @brief Get the width of the grid
     * @return The width of the grid
     */
    int getWidth() const;

    /** @brief Get the height of the grid
     * @return The height of the grid
     */
    int getHeight() const;

    /** @brief Get a cell from the grid
     * @param x The x-coordinate of the cell
     * @param y The y-coordinate of the cell
     * @return A reference to the cell at the specified position
     */
    Cell &get(int x, int y);

    /** @brief Get a const cell from the grid
     * @param x The x-coordinate of the cell
     * @param y The y-coordinate of the cell
     * @return A const reference to the cell at the specified position
     */
    const Cell &get(int x, int y) const;

    /** @brief Set a cell in the grid
     * @param x The x-coordinate of the cell
     * @param y The y-coordinate of the cell
     * @param cell The cell to set
     */
    void set(int x, int y, const Cell &cell);

    /** @brief Fill the entire grid with a single cell
     * @param cell The cell to fill the grid with
     */
    void fill(const Cell &cell);

    /** @brief Update unknown cells in the grid
     */
    void updateUnknownCells();

    /** @brief Check if the grid has any unknown cells
     * @return True if there are unknown cells, false otherwise
     */
    bool hasUnknownCells() const;

    /** @brief Get the list of unknown cells in the grid
     * @return A const reference to the vector of unknown cell positions
     */
    const std::vector<CellPosition> &getUnknownCells() const;
    /** @brief Remove an unknown cell from the grid
     * @param index The index of the unknown cell to remove
     */
    void removeUnknownCell(std::size_t index);

    /** @brief Set the scope of the grid
     * @param scope The scope to set
     */
    void setScope(ScopeSize scope);
    /** @brief Get the scope of the grid
     * @return A const reference to the scope
     */
    const Scope &getScope() const;
    /** @brief Get the scope size of the grid
     * @return A const reference to the scope size
     */
    const ScopeSize &getScopeSize() const;

    /** @brief Save the current state of the grid
     */
    void saveState();
    /** @brief Restore the previous state of the grid
     */
    void restoreState();

    /** @brief Save the shape of the grid
     */
    void saveShape();
    /** @brief Restore the shape of the grid
     */
    void restoreShape();

private:
    /** @brief The width of the grid */
    int m_width;
    /** @brief The height of the grid */
    int m_height;

    /** @brief The scope size of the grid */
    ScopeSize m_scopeSize = ScopeSize::Big;
    /** @brief The scope of the grid */
    Scope m_scope = {0, 0, 72, 72};
    /** @brief The cells in the grid */
    std::vector<Cell> m_cells;
    /** @brief The unknown cells in the grid */
    std::vector<CellPosition> m_unknownCells;

    /** @brief The saved cells in the grid */
    std::vector<Cell> m_savedCells;
    /** @brief The saved unknown cells in the grid */
    std::vector<CellPosition> m_savedUnknownCells;

    /** @brief The saved boundary of the grid */
    std::vector<CellPosition> m_savedBoundary;
    /** @brief The saved unknown cells in the shape of the grid */
    std::vector<CellPosition> m_savedUnknownCellsShape;
};