#include "FileExporter.hpp"

#include <filesystem>
#include <fstream>

bool FileExporter::save(
    const std::vector<Uint8>& data,
    const std::string& filename)
{
    if (data.empty())
        return false;

    std::filesystem::create_directories("exports");

    const std::string path =
        "exports/" + filename;

    std::ofstream file(
        path,
        std::ios::binary);

    if (!file)
        return false;

    file.write(
        reinterpret_cast<const char*>(data.data()),
        static_cast<std::streamsize>(data.size()));

    return file.good();
}