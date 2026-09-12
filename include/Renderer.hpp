#pragma once

#include <SDL3/SDL.h>
#include <string>

class Grid;

class Renderer
{
public:
    explicit Renderer(SDL_Renderer* renderer);

    void render(const Grid& grid, int offsetX, int offsetY, bool showGrid, bool showPossibilities);

    bool exportImage(const Grid& grid, const std::string& path, int scale);

private:
    SDL_Renderer* m_renderer;
};
