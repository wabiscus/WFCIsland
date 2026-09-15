#pragma once
#include "Grid.hpp"
#include "Utils.hpp"
#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

/** @brief Alias for the JSON type */
using json = nlohmann::json;

/** @brief Class representing a ruleset */
class Ruleset
{
public:
    /** @brief Construct a new ruleset */
    Ruleset(RulesetType type);

    /** @brief Get the name of the ruleset */
    const std::string &getName() const;
    /** @brief Get the type of the ruleset */
    const RulesetType getType() const;

    /** @brief Get the tiles in the ruleset */
    const std::vector<Tile> &getTiles() const;
    /** @brief Get the allowed neighbors for a tile */
    std::vector<Tile> getAllowedNeighbors(Tile tile) const;

    /** @brief Get the weights for the ruleset */
    const std::map<std::vector<Tile>, std::vector<int>> &getWeights() const;
    /** @brief Get the weights for a set of possibilities */
    std::vector<int> &getWeights(const std::vector<Tile> &possibilities);

    /** @brief Set the weights for a set of possibilities */
    void setWeights(const std::vector<Tile> &tile, const std::vector<int> &weights);
    /** @brief Set the type of the ruleset */
    void setType(RulesetType newType);

    /** @brief Get all available ruleset types */
    static const std::vector<RulesetType> &getAllTypes();

private:
    /** @brief Load the ruleset from a file */
    void loadRuleset(RulesetType type);

    /** @brief The type of the ruleset */
    RulesetType m_type;
    /** @brief The name of the ruleset */
    std::string m_name;
    /** @brief The tiles in the ruleset */
    std::vector<Tile> m_tiles;
    /** @brief The allowed neighbors for each tile */
    std::map<Tile, std::vector<Tile>> m_allowedNeighbors;
    /** @brief The weights for each set of possibilities */
    std::map<std::vector<Tile>, std::vector<int>> m_weights;
};