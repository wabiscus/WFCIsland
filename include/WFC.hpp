#pragma once

#include "Grid.hpp"
#include "Ruleset.hpp"
#include "FSM.hpp"

#include <random>
#include <queue>
#include <utility>

/**
 * @brief Implements the Wave Function Collapse algorithm
 *
 */

class WFC
{
public:
    /**
     * @brief Construct a new WFC object
     *
     * @param grid The grid to operate on
     * @param ruleset The ruleset to use for tile placement
     */
    explicit WFC(Grid &grid, Ruleset &ruleset);

    /**
     * @brief Initialize the WFC algorithm
     */
    void initialize();

    /**
     * @brief Regenerate the map
     */
    void regenerateMap();

    /**
     * @brief Perform a single propagation step
     * @return true if any changes were made, false otherwise
     */
    bool propagateStep();

    /**
     * @brief Propagate until no more changes can be made
     */
    void propagateUntilStable();

    /**
     * @brief Collapse the wave function
     */
    void collapse();

    /**
     * @brief Check if the algorithm has encountered a contradiction
     * @return true if a contradiction was found, false otherwise
     */
    bool hasContradiction() const;

    /**
     * @brief Generate an island directly
     */
    void generateIsland();

    /**
     * @brief Perform a single generation step
     */
    void generateStep();

    /**
     * @brief Set the ruleset for the WFC algorithm
     * @param newRuleset The new ruleset to use
     */
    void setRuleset(RulesetType newRuleset);

    /**
     * @brief Reset the possibilities for all cells
     */
    void resetPossibilities();

    /**
     * @brief Check if the generation process is finished
     * @return true if finished, false otherwise
     */
    bool isFinished() const;

    /**
     * @brief Set the seed for the random number generator
     * @param seed The seed to use
     */
    void setSeed(unsigned int seed);

private:
    /**
     * @brief The grid to operate on
     */
    Grid &m_grid;

    /**
     * @brief The ruleset to use for tile placement
     */
    Ruleset &m_ruleset;

    /**
     * @brief The seed for the random number generator
     */
    unsigned int m_seed;

    /**
     * @brief The random number generator
     */
    std::mt19937 m_generator;

    /**
     * @brief Flag indicating if a contradiction was found
     */
    bool m_contradiction = false;

    /**
     * @brief The queue for propagation steps
     */
    std::queue<CellPosition> m_propagationQueue;
};