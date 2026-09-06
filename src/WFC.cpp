#include "WFC.hpp"

#include <set>
#include <iostream>

WFC::WFC(Grid &grid, Ruleset &ruleset)
    : m_grid(grid), m_ruleset(ruleset), m_generator(std::random_device{}())
{
}

void WFC::initialize()
{
    const Scope &scope = m_grid.getScope();

    for (int y = scope.y; y < scope.y + scope.height; ++y)
    {
        for (int x = scope.x; x < scope.x + scope.width; ++x)
        {
            m_grid.set(x, y, Cell(m_ruleset.getTiles()));
        }
    }
}

void WFC::generate()
{
    std::uniform_int_distribution<int> xDistribution(
        0,
        m_grid.getWidth() - 1);

    std::uniform_int_distribution<int> yDistribution(
        0,
        m_grid.getHeight() - 1);

    int x = xDistribution(m_generator);
    int y = yDistribution(m_generator);

    m_grid.set(x, y, Tile::Grass);
}

void WFC::regenerateMap()
{
    Cell water(Tile::Water);
    m_grid.fill(water);

    m_contradiction = false;

    Cell newCell(m_ruleset.getTiles());

    const Scope &scope = m_grid.getScope();

    int left = scope.x;
    int right = scope.x + scope.width - 1;
    int top = scope.y;
    int bottom = scope.y + scope.height - 1;

    for (int y = top + 1; y < bottom; ++y)
    {
        for (int x = left + 1; x < right; ++x)
        {
            m_grid.set(x, y, newCell);
        }
    }
}

bool WFC::propagateStep()
{
    bool changed = false;

    const Scope &scope = m_grid.getScope();

    for (int y = scope.y; y < scope.y + scope.height; ++y)
    {
        for (int x = scope.x; x < scope.x + scope.width; ++x)
        {
            Cell &current = m_grid.get(x, y);

            if (current.tile != Tile::Unknown)
            {
                continue;
            }
            if (x > scope.x &&
                x < scope.x + scope.width - 1 &&
                y > scope.y &&
                y < scope.y + scope.height - 1)
            {
                Cell &top = m_grid.get(x, y - 1);
                Cell &bottom = m_grid.get(x, y + 1);
                Cell &left = m_grid.get(x - 1, y);
                Cell &right = m_grid.get(x + 1, y);

                std::vector<const Cell *> neighbors = {&top, &right, &bottom, &left};

                for (const Cell *neighbor : neighbors)
                {
                    std::set<Tile> allowedByNeighbor;
                    for (Tile neighborPossibility : neighbor->possibilities)
                    {
                        for (Tile allowed : m_ruleset.getAllowedNeighbors(neighborPossibility))
                        {
                            allowedByNeighbor.insert(allowed);
                        }
                    }
                    for (auto it = current.possibilities.begin();
                         it != current.possibilities.end();)
                    {
                        if (!allowedByNeighbor.contains(*it))
                        {
                            it = current.possibilities.erase(it);
                            --current.entropy;
                            changed = true;

                            if (current.possibilities.empty())
                            {
                                current.tile = Tile::Contradiction;
                                m_contradiction = true;
                            }
                        }
                        else
                        {
                            ++it;
                        }
                    }
                }
            }
        }
    }

    return changed;
}

void WFC::propagateUntilStable()
{
    while (propagateStep())
    {
    }
}

void WFC::collapse()
{
    int lowestEntropy = INT_MAX;
    std::vector<std::size_t> candidates;

    for (std::size_t i = 0; i < m_grid.getUnknownCells().size(); ++i)
    {
        const CellPosition &position = m_grid.getUnknownCells()[i];
        const Cell &cell = m_grid.get(position.x, position.y);

        if (cell.entropy < lowestEntropy)
        {
            lowestEntropy = cell.entropy;
            candidates.clear();
            candidates.push_back(i);
        }
        else if (cell.entropy == lowestEntropy)
        {
            candidates.push_back(i);
        }
    }

    std::uniform_int_distribution<std::size_t> candidateDistribution(
        0,
        candidates.size() - 1);

    const std::size_t selectedIndex = candidates[candidateDistribution(m_generator)];

    const CellPosition position =
        m_grid.getUnknownCells()[selectedIndex];

    Cell &cell = m_grid.get(position.x, position.y);

    const std::vector<Tile> &possibilities = cell.possibilities;

    if (cell.entropy == 1)
    {
        m_grid.set(position.x, position.y, Cell(cell.possibilities[0]));

        m_grid.removeUnknownCell(selectedIndex);
    }
    else
    {
        const std::vector<int> &weights =
            m_ruleset.getWeights(possibilities);

        std::discrete_distribution<std::size_t> distribution(
            weights.begin(),
            weights.end());

        const Tile selectedTile =
            possibilities[distribution(m_generator)];

        m_grid.set(position.x, position.y, Cell(selectedTile));

        m_grid.removeUnknownCell(selectedIndex);
    }
}

bool WFC::hasContradiction() const
{
    return m_contradiction;
}

void WFC::generateIsland()
{
    propagateUntilStable();

    while (m_grid.hasUnknownCells())
    {
        collapse();

        propagateUntilStable();

        if (hasContradiction())
        {
            std::cout << "Contradiction!\n";
            return;
        }
    }
}

void WFC::generateStep()
{
    if (!m_grid.hasUnknownCells())
    {
        m_generating = false;
        return;
    }

    collapse();
    propagateUntilStable();

    if (hasContradiction())
    {
        m_generating = false;
    }
}

bool WFC::isGenerating() const
{
    return m_generating;
}

void WFC::startGeneration()
{
    m_generating = true;
}

void WFC::toggleGeneration()
{
    m_generating = !m_generating;
}