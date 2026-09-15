#include "Utils.hpp"

const char *tileToString(Tile tile)
{
    switch (tile)
    {
    case Tile::Water:
        return "Water";

    case Tile::Sand:
        return "Sand";

    case Tile::Grass:
        return "Grass";

    case Tile::Forest:
        return "Forest";

    case Tile::Rock:
        return "Rock";

    case Tile::Snow:
        return "Snow";

    case Tile::Lava:
        return "Lava";

    case Tile::Unknown:
        return "Unknown";

    case Tile::Contradiction:
        return "Contradiction";
        
    case Tile::Boundary:
        return "Boundary";
    }

    return "Unknown";
}

Tile tileFromString(const std::string &name)
{
    if (name == "Unknown")
        return Tile::Unknown;
    if (name == "Boundary")
        return Tile::Boundary;
    if (name == "Water")
        return Tile::Water;
    if (name == "Sand")
        return Tile::Sand;
    if (name == "Grass")
        return Tile::Grass;
    if (name == "Forest")
        return Tile::Forest;
    if (name == "Rock")
        return Tile::Rock;
    if (name == "Snow")
        return Tile::Snow;
    if (name == "Lava")
        return Tile::Lava;
    if (name == "Contradiction")
        return Tile::Contradiction;
    throw std::invalid_argument("Unknown tile: " + name);
}

ImU32 tileToColor(Tile tile)
{
    switch (tile)
    {
    case Tile::Water:
        return IM_COL32(50, 120, 220, 255);

    case Tile::Sand:
        return IM_COL32(220, 190, 120, 255);

    case Tile::Grass:
        return IM_COL32(80, 180, 80, 255);

    case Tile::Forest:
        return IM_COL32(30, 110, 50, 255);

    case Tile::Rock:
        return IM_COL32(100, 100, 100, 255);

    case Tile::Snow:
        return IM_COL32(230, 230, 240, 255);

    case Tile::Lava:
        return IM_COL32(220, 70, 30, 255);

    default:
        return IM_COL32(150, 150, 150, 255);
    }
}

const char *rulesetToString(RulesetType type)
{
    switch (type)
    {
    case RulesetType::Tropical:
        return "Tropical";

    case RulesetType::Desert:
        return "Desert";

    case RulesetType::Forest:
        return "Forest";

    case RulesetType::Volcanic:
        return "Volcanic";

    case RulesetType::Count:
        return "Count";
    }

    return "Unknown";
}

RulesetType rulesetFromString(const std::string &name)
{
    if (name == "Tropical")
        return RulesetType::Tropical;
    if (name == "Desert")
        return RulesetType::Desert;
    if (name == "Forest")
        return RulesetType::Forest;
    if (name == "Volcanic")
        return RulesetType::Volcanic;
    throw std::invalid_argument("Unknown ruleset: " + name);
}

const char *stateToString(GenerationState state)
{
    switch (state)
    {
    case GenerationState::Empty:
        return "Empty";

    case GenerationState::Shape:
        return "Shape";

    case GenerationState::BoundariesDefined:
        return "Boundaries Defines";

    case GenerationState::Generating:
        return "Generating";

    case GenerationState::GeneratingInstantly:
        return "GenerateInstant";

    case GenerationState::Generated:
        return "Generated";
    }

    return "Unknown";
}