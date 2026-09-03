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
        break;

    case RulesetType::Desert:
        m_name = "Desert";
        m_tiles = {
            Tile::Water,
            Tile::Sand,
            // Tile::Rock
        };
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