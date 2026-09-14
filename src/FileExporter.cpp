#include "FileExporter.hpp"

#include <filesystem>
#include <fstream>

#include "JsDownload.hpp"

#include "NativeFileDialog.hpp"

bool FileExporter::save(
    const std::vector<Uint8> &data,
    const std::string &filename)
{
    if (data.empty())
        return false;

#ifdef __EMSCRIPTEN__
    js_download_file(filename.c_str(), data.data(), (int)data.size(), "image/png");
    return true;
#elif defined(_WIN32)
    std::string path = ShowSaveDialog(
        filename,
        "PNG Files (*.png)\0*.png\0All Files (*.*)\0*.*\0",
        "png");

    if (path.empty())
        return false; // annulé

    std::ofstream file(path, std::ios::binary);
    if (!file)
        return false;

    file.write(reinterpret_cast<const char *>(data.data()), (std::streamsize)data.size());
    return file.good();
#else
    std::filesystem::create_directories("exports");
    const std::string path = "exports/" + filename;

    std::ofstream file(path, std::ios::binary);
    if (!file)
        return false;

    file.write(reinterpret_cast<const char *>(data.data()), (std::streamsize)data.size());
    return file.good();
#endif
}