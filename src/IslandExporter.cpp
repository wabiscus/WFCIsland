#include "IslandExporter.hpp"

#include "Grid.hpp"
#include "Utils.hpp"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>

using json = nlohmann::json;

bool IslandExporter::exportJson(
    const Grid &grid,
    unsigned int seed,
    const std::string &rulesetName,
    const std::string &filename) const
{
    json island;

    island["version"] = 1;
    island["seed"] = seed;
    island["ruleset"] = rulesetName;

    island["grid"]["width"] = grid.getWidth();
    island["grid"]["height"] = grid.getHeight();

    island["cells"] = json::array();

    for (int y = 0; y < grid.getHeight(); ++y)
    {
        json row = json::array();

        for (int x = 0; x < grid.getWidth(); ++x)
        {
            const Tile tile = grid.get(x, y).tile;

            row.push_back(tileToString(tile));
        }

        island["cells"].push_back(row);
    }

    std::filesystem::create_directories("exports");

    const std::string path =
        "exports/" + filename;

    std::ofstream file(
        path,
        std::ios::out);

    if (!file)
        return false;

    file << island.dump(4);

    return file.good();
}