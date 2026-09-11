#include "WFC.hpp"

#include <set>
#include <iostream>

WFC::WFC(
    Grid &grid,
    Ruleset &ruleset)
    : m_grid(grid),
      m_ruleset(ruleset),
      m_generator(std::random_device{}())
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
    if (m_propagationQueue.empty())
    {
        return false;
    }

    const CellPosition position = m_propagationQueue.front();
    m_propagationQueue.pop();

    Cell &current = m_grid.get(position.x, position.y);

    if (current.tile != Tile::Unknown)
    {
        return false;
    }

    Cell &top = m_grid.get(position.x, position.y - 1);
    Cell &right = m_grid.get(position.x + 1, position.y);
    Cell &bottom = m_grid.get(position.x, position.y + 1);
    Cell &left = m_grid.get(position.x - 1, position.y);

    std::vector<const Cell *> neighbors =
        {
            &top,
            &right,
            &bottom,
            &left};

    bool changed = false;

    for (const Cell *neighbor : neighbors)
    {
        std::set<Tile> allowedByNeighbor;

        for (Tile neighborPossibility : neighbor->possibilities)
        {
            for (Tile allowed :
                 m_ruleset.getAllowedNeighbors(neighborPossibility))
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
            }
            else
            {
                ++it;
            }
        }

        if (current.possibilities.empty())
        {
            current.tile = Tile::Contradiction;
            m_contradiction = true;
            return true;
        }
    }

    if (changed)
    {
        m_propagationQueue.push({position.x, position.y - 1});
        m_propagationQueue.push({position.x + 1, position.y});
        m_propagationQueue.push({position.x, position.y + 1});
        m_propagationQueue.push({position.x - 1, position.y});
    }

    return changed;
}

void WFC::propagateUntilStable()
{
    while (!m_propagationQueue.empty())
    {
        propagateStep();
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
    }

    m_propagationQueue.push({position.x, position.y - 1});
    m_propagationQueue.push({position.x + 1, position.y});
    m_propagationQueue.push({position.x, position.y + 1});
    m_propagationQueue.push({position.x - 1, position.y});

    m_grid.removeUnknownCell(selectedIndex);
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
        return;
    }

    collapse();
    propagateUntilStable();
    // if (!m_grid.hasUnknownCells())
    // {
    //     m_fsm.transitionTo(GenerationState::Generated);
    // }
}

void WFC::setRuleset(RulesetType type)
{
    m_ruleset.setType(type);
}

void WFC::resetPossibilities()
{
    const Scope &scope = m_grid.getScope();

    for (int y = scope.y; y < scope.y + scope.height; ++y)
    {
        for (int x = scope.x; x < scope.x + scope.width; ++x)
        {
            Cell &cell = m_grid.get(x, y);

            if (cell.tile == Tile::Unknown)
            {
                cell.possibilities = m_ruleset.getTiles();
                cell.entropy =
                    static_cast<int>(cell.possibilities.size());
            }
        }
    }
}

// void WFC::startGeneration()
// {
//     m_contradiction = false;
//     m_propagationQueue = {};
//     // éventuelle initialisation de la propagation
// }

bool WFC::isFinished() const
{
    return !m_grid.hasUnknownCells();
}