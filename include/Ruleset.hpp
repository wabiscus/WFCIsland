#pragma once
#include "Grid.hpp"
#include <string>

enum class RulesetType
{
    Tropical,
    Desert,
    Forest,
    Volcanic
};

class Ruleset
{
public:
    Ruleset(RulesetType type);

    const std::string &getName() const;
    const std::vector<Tile> &getTiles() const;
    std::vector<Tile> getAllowedNeighbors(Tile tile) const;
    float getWeight(Tile tile) const;

private:
    RulesetType m_type;
    std::string m_name;
    std::vector<Tile> m_tiles;
};