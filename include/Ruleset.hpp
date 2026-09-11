#pragma once
#include "Grid.hpp"
#include "Utils.hpp"
#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

class Ruleset
{
public:
    Ruleset(RulesetType type);

    const std::string &getName() const;
    const RulesetType getType() const;

    const std::vector<Tile> &getTiles() const;
    std::vector<Tile> getAllowedNeighbors(Tile tile) const;

    const std::map<std::vector<Tile>, std::vector<int>> &getWeights() const;
    std::vector<int> &getWeights(const std::vector<Tile> &possibilities);
    
    void setWeights(
        const std::vector<Tile> &tile,
        const std::vector<int> &weights);
    void setType(RulesetType newType);

    static const std::vector<RulesetType>& getAllTypes();

private:
    void loadRuleset(RulesetType type);

    RulesetType m_type;
    std::string m_name;
    std::vector<Tile> m_tiles;
    std::map<Tile, std::vector<Tile>> m_allowedNeighbors;
    std::map<std::vector<Tile>, std::vector<int>> m_weights;
};