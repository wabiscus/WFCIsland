#include "Ruleset.hpp"

Ruleset::Ruleset(RulesetType type)
    : m_type(type)
{
    switch (type)
    {
    case RulesetType::Tropical:
        m_name = "Tropical";
        m_tiles = {
            Tile::Water,
            Tile::Sand,
            Tile::Grass,
            // Tile::Forest
        };
        m_weights[{Tile::Water, Tile::Sand}] = {8, 2};
        m_weights[{Tile::Water, Tile::Sand, Tile::Grass}] = {1, 4, 5};
        m_weights[{Tile::Sand, Tile::Grass}] = {4, 6};
        break;

    case RulesetType::Desert:
        m_name = "Desert";
        m_tiles = {
            Tile::Water,
            Tile::Sand,
            // Tile::Rock
        };
        m_weights[{Tile::Water, Tile::Sand}] = {5, 6};
        break;

    case RulesetType::Volcanic:
        m_name = "Volcanic";
        m_tiles = {
            Tile::Water,
            // Tile::Rock,
            // Tile::Lava
        };
        break;

    default:
        break;
    }
}

std::vector<Tile> Ruleset::getAllowedNeighbors(Tile tile) const
{
    switch (tile)
    {
    case Tile::Water:
        return {Tile::Water, Tile::Sand};

    case Tile::Sand:
        return {Tile::Water, Tile::Sand, Tile::Grass};

    case Tile::Grass:
        return {Tile::Sand, Tile::Grass};

    default:
        return {};
    }
}

const std::vector<Tile> &Ruleset::getTiles() const
{
    return m_tiles;
}

const std::string &Ruleset::getName() const
{
    return m_name;
}

const std::map<std::vector<Tile>, std::vector<int>> &Ruleset::getWeights() const
{
    return m_weights;
}

std::vector<int> &Ruleset::getWeights(const std::vector<Tile> &possibilities)
{
    return m_weights.at(possibilities);
}

void Ruleset::setWeights(
    const std::vector<Tile> &tile,
    const std::vector<int> &weights)
{
    m_weights[tile] = weights;
}