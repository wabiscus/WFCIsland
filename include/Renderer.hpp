#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>

class Grid;

class Renderer
{
public:
    explicit Renderer(SDL_Renderer* renderer);

    void render(const Grid& grid, int offsetX, int offsetY, bool showGrid, bool showPossibilities);

    std::vector<Uint8> exportImage(const Grid& grid, int scale);

private:
    SDL_Renderer* m_renderer;
};
