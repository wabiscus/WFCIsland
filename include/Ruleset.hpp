#pragma once
#include "Grid.hpp"
#include <string>
#include <map>

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
    const std::map<std::vector<Tile>, std::vector<int>> &getWeights() const;
    std::vector<int> &getWeights(const std::vector<Tile> &possibilities);
    void setWeights(
        const std::vector<Tile> &tile,
        const std::vector<int> &weights);

    void setType(RulesetType newRulset);

private:
    RulesetType m_type;
    std::string m_name;
    std::vector<Tile> m_tiles;
    std::map<std::vector<Tile>, std::vector<int>> m_weights;
};