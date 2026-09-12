#include "Ruleset.hpp"
#include <iostream>

Ruleset::Ruleset(RulesetType type)
{
    loadRuleset(type);
}

void Ruleset::loadRuleset(RulesetType type)
{
    m_type = type;

    m_name.clear();
    m_tiles.clear();
    m_allowedNeighbors.clear();
    m_weights.clear();

    std::string filename;

    switch (type)
    {
    case RulesetType::Tropical:
        filename = "tropical.json";
        break;

    case RulesetType::Desert:
        filename = "desert.json";
        break;

    case RulesetType::Forest:
        filename = "forest.json";
        break;

    case RulesetType::Volcanic:
        filename = "volcanic.json";
        break;

    default:
        throw std::runtime_error("Invalid ruleset type");
    }

    std::ifstream file("rulesets/" + filename);

    if (!file)
        throw std::runtime_error(
            "Unable to open ruleset: " + filename);

    json data;
    file >> data;

    for (const auto &tileName : data.at("tiles"))
    {
        m_tiles.push_back(
            tileFromString(tileName.get<std::string>()));
    }

    for (const auto &[tileName, neighborsJson] : data.at("rules").items())
    {
        Tile tile = tileFromString(tileName);

        std::vector<Tile> neighbors;

        for (const auto &neighborName : neighborsJson)
        {
            neighbors.push_back(
                tileFromString(neighborName.get<std::string>()));
        }

        m_allowedNeighbors[tile] = neighbors;
    }

    for (const auto &weightData : data.at("weights"))
    {
        std::vector<Tile> tiles;
        std::vector<int> values;

        for (const auto &tileName : weightData.at("tiles"))
        {
            tiles.push_back(
                tileFromString(tileName.get<std::string>()));
        }

        for (const auto &value : weightData.at("values"))
        {
            values.push_back(value.get<int>());
        }

        m_weights[tiles] = values;
    }

    m_name = data.at("name").get<std::string>();

    for (Tile tile : m_tiles)
    {
        if (!m_allowedNeighbors.contains(tile))
        {
            throw std::runtime_error(
                "Missing rules for tile: " +
                std::string(tileToString(tile)));
        }
    }
}

std::vector<Tile> Ruleset::getAllowedNeighbors(Tile tile) const
{
    auto it = m_allowedNeighbors.find(tile);

    if (it == m_allowedNeighbors.end())
        return {};

    return it->second;
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

std::vector<int> &Ruleset::getWeights(
    const std::vector<Tile> &possibilities)
{
    auto it = m_weights.find(possibilities);

    if (it == m_weights.end())
    {
        std::cout << "Missing weights: { ";

        for (Tile tile : possibilities)
            std::cout << tileToString(tile) << " ";

        std::cout << "}\n";

        std::cout << "Available weights:\n";

        for (const auto &[tiles, weights] : m_weights)
        {
            std::cout << "{ ";

            for (Tile tile : tiles)
                std::cout << tileToString(tile) << " ";

            std::cout << "}\n";
        }

        throw std::out_of_range("Missing weights");
    }

    return it->second;
}

void Ruleset::setWeights(
    const std::vector<Tile> &tile,
    const std::vector<int> &weights)
{
    m_weights[tile] = weights;
}

const RulesetType Ruleset::getType() const
{
    return m_type;
}

void Ruleset::setType(RulesetType newType)
{
    loadRuleset(newType);
}