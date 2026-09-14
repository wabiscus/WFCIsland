#include "IslandExporter.hpp"

#include "Grid.hpp"
#include "Utils.hpp"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>

#include "JsDownload.hpp"
#include "NativeFileDialog.hpp"

using json = nlohmann::json;

bool IslandExporter::exportJson(
    const Grid &grid,
    unsigned int seed,
    const std::string &rulesetName,
    const std::string &filename) const
{
    json island;
    // ... construction du json inchangée ...

    const std::string dump = island.dump(4);

#ifdef __EMSCRIPTEN__
    js_download_file(
        filename.c_str(),
        reinterpret_cast<const uint8_t *>(dump.data()),
        (int)dump.size(),
        "application/json");
    return true;
#elif defined(_WIN32)
    std::string path = ShowSaveDialog(
        filename,
        "JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0",
        "json");

    if (path.empty())
        return false; // annulé

    std::ofstream file(path, std::ios::out);
    if (!file)
        return false;

    file << dump;
    return file.good();
#else
    std::filesystem::create_directories("exports");
    const std::string path = "exports/" + filename;

    std::ofstream file(path, std::ios::out);
    if (!file)
        return false;

    file << dump;
    return file.good();
#endif
}