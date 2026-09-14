#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>

class FileExporter
{
public:
    bool save(
        const std::vector<Uint8>& data,
        const std::string& filename);
};