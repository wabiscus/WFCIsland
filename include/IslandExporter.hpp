#pragma once

#include <string>

class Grid;

class IslandExporter
{
public:
    bool exportJson(
        const Grid &grid,
        unsigned int seed,
        const std::string &rulesetName,
        const std::string &filename) const;
};