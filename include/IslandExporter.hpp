#pragma once

#include <string>

class Grid;

/** @brief Class representing an island exporter */
class IslandExporter
{
public:
    /** @brief Export the grid as a JSON file
     * @param grid The grid to export
     * @param seed The seed for the grid
     * @param rulesetName The name of the ruleset
     * @param filename The filename to save the export as
     * @return True if the export was successful, false otherwise
     */
    bool exportJson(
        const Grid &grid,
        unsigned int seed,
        const std::string &rulesetName,
        const std::string &filename) const;
};