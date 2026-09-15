#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>

class Grid;

/** @brief Class representing a renderer */
class Renderer
{
public:
    /** @brief Construct a new renderer */
    explicit Renderer(SDL_Renderer* renderer);

    /** @brief Render the grid */
    void render(const Grid& grid, int offsetX, int offsetY, bool showGrid, bool showPossibilities);

    /** @brief Export the grid as an image */
    std::vector<Uint8> exportImage(const Grid& grid, int scale);

private:
    /** @brief The SDL renderer */
    SDL_Renderer* m_renderer;
};
