#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>

/** @brief Class representing a file exporter */
class FileExporter
{
public:
    /** @brief Save data to a file
     * @param data The data to save
     * @param filename The filename to save the data as
     * @return True if the save was successful, false otherwise
     */
    bool save(
        const std::vector<Uint8>& data,
        const std::string& filename);
};