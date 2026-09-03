#include "WFC.hpp"

#include <set>

WFC::WFC(Grid &grid, Ruleset &ruleset)
    : m_grid(grid), m_ruleset(ruleset), m_generator(std::random_device{}())
{
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

void WFC::propagateAll()
{
    while (propagateStep())
    {
        collapse();
    }
}

void WFC::collapse()
{
    std::vector<Candidate> candidates;
    int lowestEntropy = INT_MAX;

    const Scope &scope = m_grid.getScope();

    for (int y = scope.y; y < scope.y + scope.height; ++y)
    {
        for (int x = scope.x; x < scope.x + scope.width; ++x)
        {
            Cell &cell = m_grid.get(x, y);
            if (cell.tile != Tile::Unknown)
            {
                continue;
            }

            if (cell.entropy < lowestEntropy)
            {
                lowestEntropy = cell.entropy;
                candidates.clear();
                candidates.push_back({x, y});
            }
            else if (cell.entropy == lowestEntropy)
            {
                candidates.push_back({x, y});
            }
        }
    }

    std::uniform_int_distribution<std::size_t> distribution(0, candidates.size() - 1);
    Candidate selectedCandidate = candidates[distribution(m_generator)];

    Cell &cell = m_grid.get(selectedCandidate.x, selectedCandidate.y);

    std::uniform_int_distribution<std::size_t> tileDistribution(0, cell.possibilities.size() - 1);
    Tile selectedTile = cell.possibilities[tileDistribution(m_generator)];

    m_grid.set(selectedCandidate.x, selectedCandidate.y, Cell(selectedTile));
}